class Point2D {
    #x;
    #y;

    constructor(x = 0, y = 0) {
        this.#x = x;
        this.#y = y;
    }

    get x() { return this.#x; }
    get y() { return this.#y; }
    
    set x(value) { this.#x = value; }
    set y(value) { this.#y = value; }

    distanceTo(other) {
        const dx = this.#x - other.x;
        const dy = this.#y - other.y;
        return Math.sqrt(dx * dx + dy * dy);
    }

    clone() {
        return new Point2D(this.#x, this.#y);
    }
}

class Vector2D extends Point2D {
    constructor(x = 0, y = 0) {
        super(x, y);
    }

    add(vector) {
        return new Vector2D(this.x + vector.x, this.y + vector.y);
    }

    subtract(vector) {
        return new Vector2D(this.x - vector.x, this.y - vector.y);
    }

    multiply(scalar) {
        return new Vector2D(this.x * scalar, this.y * scalar);
    }

    magnitude() {
        return Math.sqrt(this.x * this.x + this.y * this.y);
    }

    normalize() {
        const mag = this.magnitude();
        return mag > 0 ? new Vector2D(this.x / mag, this.y / mag) : new Vector2D(0, 0);
    }
}

// ✅ NUEVO: Sistema avanzado de detección de cruces y evasión
class PathManager {
    static DETECTION_RADIUS = 80; // Radio de detección de cruces
    static MIN_SEPARATION = 25; // Distancia mínima entre drones
    static VERTICAL_OFFSET = 60; // Desviación vertical para evitar cruces
    static DETECTION_ANGLE = 45; // Ángulo de detección (grados)

    /**
     * Detecta si dos trayectorias se van a cruzar
     */
    static willPathsCross(drone1, drone2) {
        // Si alguno está quieto, no hay cruce
        if (drone1.state === Drone.STATES.IDLE || drone2.state === Drone.STATES.IDLE) {
            return false;
        }

        const p1 = drone1.currentPosition;
        const t1 = drone1.targetPosition;
        const p2 = drone2.currentPosition;
        const t2 = drone2.targetPosition;

        // Calcular vectores de dirección
        const dir1 = new Vector2D(t1.x - p1.x, t1.y - p1.y);
        const dir2 = new Vector2D(t2.x - p2.x, t2.y - p2.y);

        // Si las direcciones son muy similares, no hay cruce frontal
        const angle = this.getAngleBetweenVectors(dir1, dir2);
        if (Math.abs(angle) < 30) return false;

        // Calcular punto de intersección aproximado
        const intersection = this.lineIntersection(p1, t1, p2, t2);
        
        if (intersection) {
            // Verificar si el cruce está dentro del radio de detección
            const dist1 = p1.distanceTo(intersection);
            const dist2 = p2.distanceTo(intersection);
            
            return dist1 < this.DETECTION_RADIUS && dist2 < this.DETECTION_RADIUS;
        }

        return false;
    }

    /**
     * Calcula el punto de intersección de dos líneas
     */
    static lineIntersection(p1, p2, p3, p4) {
        const x1 = p1.x, y1 = p1.y;
        const x2 = p2.x, y2 = p2.y;
        const x3 = p3.x, y3 = p3.y;
        const x4 = p4.x, y4 = p4.y;

        const denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        
        if (Math.abs(denom) < 0.001) return null; // Líneas paralelas

        const t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
        const u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denom;

        if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
            return new Point2D(
                x1 + t * (x2 - x1),
                y1 + t * (y2 - y1)
            );
        }

        return null;
    }

    /**
     * Calcula el ángulo entre dos vectores
     */
    static getAngleBetweenVectors(v1, v2) {
        const dot = v1.x * v2.x + v1.y * v2.y;
        const mag1 = Math.sqrt(v1.x * v1.x + v1.y * v1.y);
        const mag2 = Math.sqrt(v2.x * v2.x + v2.y * v2.y);
        
        if (mag1 === 0 || mag2 === 0) return 0;
        
        const cos = dot / (mag1 * mag2);
        return Math.acos(Math.max(-1, Math.min(1, cos))) * 180 / Math.PI;
    }

    /**
     * Genera puntos de waypoint para evitar cruces
     */
    static generateAvoidanceWaypoint(drone, obstacle, canvasHeight) {
        const current = drone.currentPosition;
        const target = drone.targetPosition;
        const obstaclePos = obstacle.currentPosition;

        // Calcular punto medio entre posición actual y objetivo
        const midX = (current.x + target.x) / 2;
        const midY = (current.y + target.y) / 2;

        // Determinar si desviarse arriba o abajo
        const offset = obstaclePos.y < midY ? this.VERTICAL_OFFSET : -this.VERTICAL_OFFSET;
        
        // Crear waypoint con desviación vertical
        const waypointY = Math.max(50, Math.min(canvasHeight - 50, midY + offset));
        
        return new Point2D(midX, waypointY);
    }

    /**
     * Detecta drones en ruta de colisión
     */
    static detectCrossingDrones(drone, allDrones) {
        const crossing = [];

        for (const other of allDrones) {
            if (drone.id === other.id) continue;
            if (other.state === Drone.STATES.IDLE || other.state === Drone.STATES.ARRIVED) continue;

            if (this.willPathsCross(drone, other)) {
                crossing.push(other);
            }
        }

        return crossing;
    }
}

// ================= CLASES AVANZADAS (POO II) =================

class Drone extends Point2D {
    #startPos;
    #targetPos;
    #currentPos;
    #homePos;
    #progress;
    #state;
    #color;
    #size;
    #glow;
    #id;
    #waypoint; // ✅ NUEVO: Punto intermedio para evasión
    #hasWaypoint; // ✅ NUEVO: Flag de waypoint activo
    #baseSpeed; // ✅ NUEVO: Velocidad base variable
    static #nextId = 0;

    static STATES = {
        IDLE: 'IDLE',
        MOVING: 'MOVING',
        ARRIVED: 'ARRIVED',
        RETURNING: 'RETURNING',
        TO_WAYPOINT: 'TO_WAYPOINT' // ✅ NUEVO: Moviéndose a waypoint
    };

    constructor(x, y, color) {
        super(x, y);
        this.#id = Drone.#nextId++;
        this.#homePos = new Point2D(x, y);
        this.#startPos = new Point2D(x, y);
        this.#currentPos = new Point2D(x, y);
        this.#targetPos = new Point2D(x, y);
        this.#waypoint = null;
        this.#hasWaypoint = false;
        this.#progress = 0;
        this.#state = Drone.STATES.IDLE;
        this.#color = color;
        this.#size = 8;
        this.#glow = 0;
        // ✅ NUEVO: Velocidad base aleatoria para variedad
        this.#baseSpeed = 4000 + Math.random() * 2000;
    }

    get id() { return this.#id; }
    get state() { return this.#state; }
    get color() { return this.#color; }
    get currentPosition() { return this.#currentPos.clone(); }
    get homePosition() { return this.#homePos.clone(); }
    get targetPosition() { return this.#targetPos.clone(); }

    #easeInOutCubic(t) {
        return t < 0.5
            ? 4 * t * t * t
            : 1 - Math.pow(-2 * t + 2, 3) / 2;
    }

    setTarget(x, y) {
        this.#startPos = this.#currentPos.clone();
        this.#targetPos = new Point2D(x, y);
        this.#progress = 0;
        this.#state = Drone.STATES.MOVING;
        this.#hasWaypoint = false;
        this.#waypoint = null;
    }

    returnHome() {
        this.#startPos = this.#currentPos.clone();
        this.#targetPos = this.#homePos.clone();
        this.#progress = 0;
        this.#state = Drone.STATES.RETURNING;
        this.#hasWaypoint = false;
        this.#waypoint = null;
    }

    // ✅ NUEVO: Establecer waypoint de evasión
    setAvoidanceWaypoint(waypoint) {
        if (this.#state === Drone.STATES.MOVING) {
            this.#waypoint = waypoint.clone();
            this.#hasWaypoint = true;
            
            // Guardar el objetivo final
            const finalTarget = this.#targetPos.clone();
            
            // Primero ir al waypoint
            this.#startPos = this.#currentPos.clone();
            this.#targetPos = this.#waypoint;
            this.#progress = 0;
            this.#state = Drone.STATES.TO_WAYPOINT;
            
            // Guardar el objetivo final para después
            this.finalTarget = finalTarget;
        }
    }

    update(deltaTime) {
        if (this.#state === Drone.STATES.MOVING || 
            this.#state === Drone.STATES.RETURNING || 
            this.#state === Drone.STATES.TO_WAYPOINT) {
            
            // ✅ Usar velocidad base variable
            this.#progress += deltaTime / this.#baseSpeed;

            if (this.#progress >= 1) {
                this.#progress = 1;
                
                // ✅ NUEVO: Si llegó al waypoint, continuar al objetivo final
                if (this.#state === Drone.STATES.TO_WAYPOINT && this.finalTarget) {
                    this.#currentPos.x = this.#targetPos.x;
                    this.#currentPos.y = this.#targetPos.y;
                    
                    // Ahora ir al objetivo final
                    this.#startPos = this.#currentPos.clone();
                    this.#targetPos = this.finalTarget;
                    this.#progress = 0;
                    this.#state = Drone.STATES.MOVING;
                    this.finalTarget = null;
                } else {
                    // Llegó al destino final
                    this.#state = this.#state === Drone.STATES.RETURNING ? Drone.STATES.IDLE : Drone.STATES.ARRIVED;
                    this.#currentPos.x = this.#targetPos.x;
                    this.#currentPos.y = this.#targetPos.y;
                }
            } else {
                const eased = this.#easeInOutCubic(this.#progress);
                this.#currentPos.x = this.#startPos.x + (this.#targetPos.x - this.#startPos.x) * eased;
                this.#currentPos.y = this.#startPos.y + (this.#targetPos.y - this.#startPos.y) * eased;
            }
        }

        this.#glow = Math.sin(Date.now() * 0.005) * 8 + 15;
    }

    render(ctx) {
        ctx.save();
        
        ctx.shadowBlur = this.#glow;
        ctx.shadowColor = this.#color;
        
        ctx.fillStyle = this.#color;
        ctx.beginPath();
        ctx.arc(this.#currentPos.x, this.#currentPos.y, this.#size, 0, Math.PI * 2);
        ctx.fill();

        // ✅ NUEVO: Dibujar línea de trayectoria si tiene waypoint (debug opcional)
        // Descomenta para visualizar las rutas de evasión
        /*
        if (this.#state === Drone.STATES.TO_WAYPOINT && this.#waypoint) {
            ctx.strokeStyle = this.#color;
            ctx.globalAlpha = 0.3;
            ctx.lineWidth = 1;
            ctx.beginPath();
            ctx.moveTo(this.#currentPos.x, this.#currentPos.y);
            ctx.lineTo(this.#waypoint.x, this.#waypoint.y);
            if (this.finalTarget) {
                ctx.lineTo(this.finalTarget.x, this.finalTarget.y);
            }
            ctx.stroke();
            ctx.globalAlpha = 1;
        }
        */

        ctx.restore();
    }
}

/**
 * Definición manual de letras
 */
class LetterFactory {
    static GRID_SIZE = 5;
    static GRID_WIDTH = 4;
    
    static LETTERS = {
        'A': [
            [0, 1, 1, 0],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 1, 1, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1]
        ],
        'B': [
            [1, 1, 1, 0],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 1, 1, 0],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 1, 1, 0]
        ],
        'C': [
            [0, 1, 1, 1],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [0, 1, 1, 1]
        ],
        'D': [
            [1, 1, 1, 0],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 1, 1, 0]
        ],
        'E': [
            [1, 1, 1, 1],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 1, 1, 0],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 1, 1, 1]
        ],
        'F': [
            [1, 1, 1, 1],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 1, 1, 0],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 0, 0, 0]
        ],
        'G': [
            [0, 1, 1, 1],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 0, 1, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [0, 1, 1, 1]
        ],
        'H': [
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 1, 1, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1]
        ],
        'I': [
            [1, 1, 1, 1],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [1, 1, 1, 1]
        ],
        'J': [
            [0, 0, 1, 1],
            [0, 0, 0, 1],
            [0, 0, 0, 1],
            [0, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [0, 1, 1, 0]
        ],
        'K': [
            [1, 0, 0, 1],
            [1, 0, 1, 0],
            [1, 1, 0, 0],
            [1, 0, 0, 0],
            [1, 1, 0, 0],
            [1, 0, 1, 0],
            [1, 0, 0, 1]
        ],
        'L': [
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 1, 1, 1]
        ],
        'M': [
            [1, 0, 0, 1],
            [1, 1, 1, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1]
        ],
        'N': [
            [1, 0, 0, 1],
            [1, 1, 0, 1],
            [1, 1, 0, 1],
            [1, 0, 1, 1],
            [1, 0, 1, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1]
        ],
        'O': [
            [0, 1, 1, 0],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [0, 1, 1, 0]
        ],
        'P': [
            [1, 1, 1, 0],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 1, 1, 0],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 0, 0, 0]
        ],
        'Q': [
            [0, 1, 1, 0],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 1, 1],
            [1, 0, 0, 1],
            [0, 1, 1, 1]
        ],
        'R': [
            [1, 1, 1, 0],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 1, 1, 0],
            [1, 0, 1, 0],
            [1, 0, 0, 1],
            [1, 0, 0, 1]
        ],
        'S': [
            [0, 1, 1, 1],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [0, 1, 1, 0],
            [0, 0, 0, 1],
            [0, 0, 0, 1],
            [1, 1, 1, 0]
        ],
        'T': [
            [1, 1, 1, 1],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [0, 1, 1, 0]
        ],
        'U': [
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [0, 1, 1, 0]
        ],
        'V': [
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [0, 1, 1, 0],
            [0, 1, 1, 0]
        ],
        'W': [
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 1, 1, 1],
            [1, 0, 0, 1]
        ],
        'X': [
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [1, 0, 0, 1],
            [1, 0, 0, 1]
        ],
        'Y': [
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [0, 1, 1, 0]
        ],
        'Z': [
            [1, 1, 1, 1],
            [0, 0, 0, 1],
            [0, 0, 1, 0],
            [0, 1, 0, 0],
            [0, 1, 0, 0],
            [1, 0, 0, 0],
            [1, 1, 1, 1]
        ],
        ' ': [
            [0, 0, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0]
        ],
        '0': [
            [0, 1, 1, 0],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [0, 1, 1, 0]
        ],
        '1': [
            [0, 1, 1, 0],
            [1, 1, 1, 0],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [1, 1, 1, 1]
        ],
        '2': [
            [0, 1, 1, 0],
            [1, 0, 0, 1],
            [0, 0, 0, 1],
            [0, 0, 1, 0],
            [0, 1, 0, 0],
            [1, 0, 0, 0],
            [1, 1, 1, 1]
        ],
        '3': [
            [1, 1, 1, 0],
            [0, 0, 0, 1],
            [0, 0, 0, 1],
            [0, 1, 1, 0],
            [0, 0, 0, 1],
            [0, 0, 0, 1],
            [1, 1, 1, 0]
        ],
        '4': [
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [1, 1, 1, 1],
            [0, 0, 0, 1],
            [0, 0, 0, 1],
            [0, 0, 0, 1]
        ],
        '5': [
            [1, 1, 1, 1],
            [1, 0, 0, 0],
            [1, 1, 1, 0],
            [0, 0, 0, 1],
            [0, 0, 0, 1],
            [0, 0, 0, 1],
            [1, 1, 1, 0]
        ],
        '6': [
            [0, 1, 1, 1],
            [1, 0, 0, 0],
            [1, 0, 0, 0],
            [1, 1, 1, 0],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [0, 1, 1, 0]
        ],
        '7': [
            [1, 1, 1, 1],
            [0, 0, 0, 1],
            [0, 0, 0, 1],
            [0, 0, 1, 0],
            [0, 1, 0, 0],
            [0, 1, 0, 0],
            [0, 1, 0, 0]
        ],
        '8': [
            [0, 1, 1, 0],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [0, 1, 1, 0],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [0, 1, 1, 0]
        ],
        '9': [
            [0, 1, 1, 0],
            [1, 0, 0, 1],
            [1, 0, 0, 1],
            [0, 1, 1, 1],
            [0, 0, 0, 1],
            [0, 0, 0, 1],
            [1, 1, 1, 0]
        ],
        'Ñ': [
            [0, 1, 1, 1],
            [0, 0, 0, 0],
            [1, 0, 0, 1],
            [1, 1, 0, 1],
            [1, 0, 1, 1],
            [1, 0, 0, 1],
            [1, 0, 0, 1]
        ],
        '!': [
            [0, 1, 0, 1, 0],
            [1, 0, 1, 0, 1],
            [1, 0, 0, 0, 1],
            [0, 1, 0, 1, 0],
            [0, 0, 1, 0, 0],
            [0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0]
        ],
        '"': [
            [1, 0, 1, 0],
            [0, 1, 1, 0],
            [1, 1, 1, 1],
            [0, 1, 1, 0],
            [1, 1, 1, 1],
            [0, 1, 1, 0],
            [1, 0, 1, 0]
        ]
    };

    static getLetterPoints(letter, startX, startY, scale) {
        const pattern = this.LETTERS[letter.toUpperCase()] || this.LETTERS[' '];
        const points = [];

        for (let row = 0; row < pattern.length; row++) {
            for (let col = 0; col < pattern[row].length; col++) {
                if (pattern[row][col] === 1) {
                    points.push(new Point2D(
                        startX + col * scale,
                        startY + row * scale
                    ));
                }
            }
        }

        return points;
    }

    static getLetterWidth(letter) {
        const pattern = this.LETTERS[letter.toUpperCase()] || this.LETTERS[' '];
        return pattern[0].length;
    }
}

/**
 * Gestor de drones con sistema de evasión mejorado
 */
class DroneManager {
    #drones;
    #canvas;
    #ctx;
    #lastTime;
    #isAnimating;
    #colorPalette;

    static CONFIG = {
        TOTAL_DRONES: 300,
        GRID_SPACING: 20,
        GRID_MARGIN: 30,
        LETTER_SCALE: 25,
        LETTER_SPACING: 15
    };

    constructor(canvasId) {
        this.#canvas = document.getElementById(canvasId);
        this.#ctx = this.#canvas.getContext('2d');
        this.#drones = [];
        this.#lastTime = 0;
        this.#isAnimating = false;
        
        this.#colorPalette = [
            '#FF0080', '#00FF88', '#FFD700', '#00D4FF', '#FF4500',
            '#9D00FF', '#00FF00', '#FF1493', '#00CED1', '#FF6347',
            '#FF69B4', '#32CD32', '#FF8C00', '#8A2BE2', '#00FA9A'
        ];

        this.#setupCanvas();
        this.#initializeDrones();
        window.addEventListener('resize', () => {
            this.#setupCanvas();
            this.#repositionGrid();
        });
    }

    #setupCanvas() {
        this.#canvas.width = this.#canvas.offsetWidth;
        this.#canvas.height = this.#canvas.offsetHeight;
    }

    #initializeDrones() {
        const spacing = DroneManager.CONFIG.GRID_SPACING;
        const margin = DroneManager.CONFIG.GRID_MARGIN;
        
        const availableWidth = this.#canvas.width - (margin * 2);
        const dronesPerRow = Math.floor(availableWidth / spacing);
        const totalRows = Math.ceil(DroneManager.CONFIG.TOTAL_DRONES / dronesPerRow);
        
        const gridHeight = totalRows * spacing;
        const startY = this.#canvas.height - gridHeight - margin;

        let droneIndex = 0;
        
        for (let row = 0; row < totalRows && droneIndex < DroneManager.CONFIG.TOTAL_DRONES; row++) {
            const dronesInThisRow = Math.min(dronesPerRow, DroneManager.CONFIG.TOTAL_DRONES - droneIndex);
            const rowWidth = dronesInThisRow * spacing;
            const startX = (this.#canvas.width - rowWidth) / 2;
            
            for (let col = 0; col < dronesInThisRow; col++) {
                const x = startX + col * spacing + spacing / 2;
                const y = startY + row * spacing;
                
                const color = this.#colorPalette[droneIndex % this.#colorPalette.length];
                const drone = new Drone(x, y, color);
                this.#drones.push(drone);
                
                droneIndex++;
            }
        }

        this.#updateDroneStats();
        this.#startAnimation();
    }

    #repositionGrid() {
        const spacing = DroneManager.CONFIG.GRID_SPACING;
        const margin = DroneManager.CONFIG.GRID_MARGIN;
        
        const availableWidth = this.#canvas.width - (margin * 2);
        const dronesPerRow = Math.floor(availableWidth / spacing);
        const totalRows = Math.ceil(this.#drones.length / dronesPerRow);
        
        const gridHeight = totalRows * spacing;
        const startY = this.#canvas.height - gridHeight - margin;

        let droneIndex = 0;
        
        for (let row = 0; row < totalRows && droneIndex < this.#drones.length; row++) {
            const dronesInThisRow = Math.min(dronesPerRow, this.#drones.length - droneIndex);
            const rowWidth = dronesInThisRow * spacing;
            const startX = (this.#canvas.width - rowWidth) / 2;
            
            for (let col = 0; col < dronesInThisRow; col++) {
                const x = startX + col * spacing + spacing / 2;
                const y = startY + row * spacing;
                
                this.#drones[droneIndex].homePosition.x = x;
                this.#drones[droneIndex].homePosition.y = y;
                
                if (this.#drones[droneIndex].state === Drone.STATES.IDLE) {
                    this.#drones[droneIndex].currentPosition.x = x;
                    this.#drones[droneIndex].currentPosition.y = y;
                }
                
                droneIndex++;
            }
        }
    }

    #textToPoints(text) {
        const points = [];
        const scale = DroneManager.CONFIG.LETTER_SCALE;
        const spacing = DroneManager.CONFIG.LETTER_SPACING;
        
        let totalWidth = 0;
        for (let i = 0; i < text.length; i++) {
            totalWidth += LetterFactory.getLetterWidth(text[i]) * scale;
            if (i < text.length - 1) totalWidth += spacing;
        }
        
        let currentX = (this.#canvas.width - totalWidth) / 2;
        const startY = (this.#canvas.height - (7 * scale)) / 2;
        
        for (let i = 0; i < text.length; i++) {
            const letter = text[i];
            const letterPoints = LetterFactory.getLetterPoints(letter, currentX, startY, scale);
            points.push(...letterPoints);
            
            currentX += LetterFactory.getLetterWidth(letter) * scale + spacing;
        }
        
        return points;
    }

    formText(text) {
        if (!text || text.trim() === '') return;

        const targetPoints = this.#textToPoints(text.toUpperCase());
        const requiredDrones = Math.min(targetPoints.length, DroneManager.CONFIG.TOTAL_DRONES);

        for (let i = 0; i < requiredDrones; i++) {
            this.#drones[i].setTarget(targetPoints[i].x, targetPoints[i].y);
        }

        for (let i = requiredDrones; i < this.#drones.length; i++) {
            if (this.#drones[i].state !== Drone.STATES.IDLE) {
                this.#drones[i].returnHome();
            }
        }

        this.#updateUI(text);
    }

    #animate(currentTime) {
        const deltaTime = currentTime - this.#lastTime;
        this.#lastTime = currentTime;

        this.#ctx.fillStyle = '#0a0e27';
        this.#ctx.fillRect(0, 0, this.#canvas.width, this.#canvas.height);

        // ✅ NUEVO: Detectar y resolver cruces de trayectorias
        this.#detectAndAvoidCrossings();

        let inFormation = 0;
        let waiting = 0;
        let allArrived = true;

        for (const drone of this.#drones) {
            drone.update(deltaTime);
            drone.render(this.#ctx);
            
            if (drone.state === Drone.STATES.ARRIVED) {
                inFormation++;
            } else if (drone.state === Drone.STATES.IDLE) {
                waiting++;
            } else if (drone.state === Drone.STATES.MOVING || 
                       drone.state === Drone.STATES.RETURNING || 
                       drone.state === Drone.STATES.TO_WAYPOINT) {
                allArrived = false;
            }
        }

        document.getElementById('inFormation').textContent = inFormation;
        document.getElementById('waiting').textContent = waiting;
        document.getElementById('status').textContent = allArrived ? 'Completado' : 'Animando...';

        requestAnimationFrame(t => this.#animate(t));
    }

    // ✅ NUEVO: Sistema principal de detección y evasión de cruces
    #detectAndAvoidCrossings() {
        const movingDrones = this.#drones.filter(d => 
            d.state === Drone.STATES.MOVING || d.state === Drone.STATES.RETURNING
        );

        // Detectar cruces entre drones en movimiento
        for (let i = 0; i < movingDrones.length; i++) {
            const drone = movingDrones[i];
            const crossingDrones = PathManager.detectCrossingDrones(drone, movingDrones);

            // Si hay drones en ruta de colisión, aplicar evasión
            if (crossingDrones.length > 0) {
                // Solo el drone con ID más bajo se desvía (evita que ambos se desvíen)
                for (const other of crossingDrones) {
                    if (drone.id < other.id) {
                        const waypoint = PathManager.generateAvoidanceWaypoint(
                            drone, 
                            other, 
                            this.#canvas.height
                        );
                        drone.setAvoidanceWaypoint(waypoint);
                        break; // Solo una desviación a la vez
                    }
                }
            }
        }
    }

    #startAnimation() {
        if (!this.#isAnimating) {
            this.#lastTime = performance.now();
            this.#isAnimating = true;
            requestAnimationFrame(t => this.#animate(t));
        }
    }

    #updateUI(text) {
        document.getElementById('currentText').textContent = text;
    }

    #updateDroneStats() {
        document.getElementById('droneCount').textContent = this.#drones.length;
        document.getElementById('waiting').textContent = this.#drones.length;
    }
}

// ================= INICIALIZACIÓN =================

const droneManager = new DroneManager('droneCanvas');

function formText() {
    const text = document.getElementById('textInput').value;
    droneManager.formText(text);
}

document.getElementById('textInput').addEventListener('keypress', (e) => {
    if (e.key === 'Enter') {
        formText();
    }
});

window.addEventListener('load', () => {
    document.getElementById('status').textContent = 'Listo';
});