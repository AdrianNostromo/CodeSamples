"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class Vector3 {
    constructor() {
        this.setXYZ(0, 0, 0);
        //void
    }
    static fromXYZ(x, y, z) {
        return new Vector3().setXYZ(x, y, z);
    }
    /** @return The euclidean length */
    static len(x, y, z) {
        return Math.sqrt(x * x + y * y + z * z);
    }
    setXYZ(x, y, z) {
        this.x = x;
        this.y = y;
        this.z = z;
        return this;
    }
    setV(v) {
        this.x = v.x;
        this.y = v.y;
        this.z = v.z;
        return this;
    }
    equalsQ(q) {
        if (this.x != q.x || this.y != q.y || this.z != q.z) {
            return false;
        }
        return true;
    }
    /** @return The dot product between the two vectors */
    static dotXYZXYZ(x1, y1, z1, x2, y2, z2) {
        return x1 * x2 + y1 * y2 + z1 * z2;
    }
}
Vector3.X = Vector3.fromXYZ(1, 0, 0);
Vector3.Y = Vector3.fromXYZ(0, 1, 0);
Vector3.Z = Vector3.fromXYZ(0, 0, 1);
Vector3.Zero = Vector3.fromXYZ(0, 0, 0);
exports.Vector3 = Vector3;
//# sourceMappingURL=Vector3.js.map