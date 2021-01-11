"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class Point3D {
    constructor(x, y, z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }
    set(value) {
        this.x = value.x;
        this.y = value.y;
        this.z = value.z;
    }
    setXYZ(x, y, z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }
    equals(value) {
        if (this.x != value.x || this.y != value.y || this.z != value.z) {
            return false;
        }
        return true;
    }
    equalsXYZ(x, y, z) {
        if (this.x != x || this.y != y || this.z != z) {
            return false;
        }
        return true;
    }
}
exports.Point3D = Point3D;
//# sourceMappingURL=Point3D.js.map