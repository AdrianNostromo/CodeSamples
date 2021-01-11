"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const Vector3_1 = require("base/util/Vector3");
const MathUtil_1 = require("base/util/MathUtil");
class Quaternion3D {
    static fromXYZW(x, y, z, w) {
        return new Quaternion3D().setXYZW(x, y, z, w);
    }
    static fromQ(q) {
        return new Quaternion3D().setQ(q);
    }
    static fromIdt() {
        return new Quaternion3D();
    }
    static fromAxisAngle(axis, angle) {
        return new Quaternion3D().setAxisAngle(axis, angle);
    }
    constructor() {
        this.idt();
    }
    setXYZW(x, y, z, w) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
        return this;
    }
    setQ(q) {
        this.x = q.x;
        this.y = q.y;
        this.z = q.z;
        this.w = q.w;
        return this;
    }
    setAxisAngle(axis, angle) {
        return this.setFromAxis(axis.x, axis.y, axis.z, angle);
    }
    setFromAxis(x, y, z, degrees) {
        return this.setFromAxisRad(x, y, z, degrees * MathUtil_1.MathUtil.degreesToRadians);
    }
    /** Sets the quaternion components from the given axis and angle around that axis.
     * @param x X direction of the axis
     * @param y Y direction of the axis
     * @param z Z direction of the axis
     * @param radians The angle in radians
     * @return This quaternion for chaining. */
    setFromAxisRad(x, y, z, radians) {
        let d = Vector3_1.Vector3.len(x, y, z);
        if (d == 0)
            return this.idt();
        d = 1 / d;
        let l_ang = radians < 0 ? MathUtil_1.MathUtil.PI2 - (-radians % MathUtil_1.MathUtil.PI2) : radians % MathUtil_1.MathUtil.PI2;
        let l_sin = Math.sin(l_ang / 2);
        let l_cos = Math.cos(l_ang / 2);
        return this.setXYZW(d * x * l_sin, d * y * l_sin, d * z * l_sin, l_cos).nor();
    }
    idt() {
        return this.setXYZW(0, 0, 0, 1);
    }
    /** Normalizes this quaternion to unit length
     * @return the quaternion for chaining */
    nor() {
        let len = this.len2();
        if (len != 0. && !MathUtil_1.MathUtil.isEqualNumberNumber(len, 1)) {
            len = Math.sqrt(len);
            this.w /= len;
            this.x /= len;
            this.y /= len;
            this.z /= len;
        }
        return this;
    }
    /** @return the length of this quaternion without square root */
    len2() {
        return this.x * this.x + this.y * this.y + this.z * this.z + this.w * this.w;
    }
    equalsQ(q) {
        if (this.x != q.x || this.y != q.y || this.z != q.z || this.w != q.w) {
            return false;
        }
        return true;
    }
    /** Get the angle in radians of the rotation around the specified axis. The axis must be normalized.
     * @param axisX the x component of the normalized axis for which to get the angle
     * @param axisY the y component of the normalized axis for which to get the angle
     * @param axisZ the z component of the normalized axis for which to get the angle
     * @return the angle in radians of the rotation around the specified axis */
    getAngleAroundRadXYZ(axisX, axisY, axisZ) {
        let d = Vector3_1.Vector3.dotXYZXYZ(this.x, this.y, this.z, axisX, axisY, axisZ);
        let l2 = Quaternion3D.len2XYZXYZ(axisX * d, axisY * d, axisZ * d, this.w);
        return MathUtil_1.MathUtil.isZeroNumber(l2) ? 0 : (2.0 * Math.acos(MathUtil_1.MathUtil.clampValMinMax(((d < 0 ? -this.w : this.w) / Math.sqrt(l2)), -1, 1)));
    }
    /** Get the angle in radians of the rotation around the specified axis. The axis must be normalized.
     * @param axis the normalized axis for which to get the angle
     * @return the angle in radians of the rotation around the specified axis */
    getAngleAroundRadAxis(axis) {
        return this.getAngleAroundRadXYZ(axis.x, axis.y, axis.z);
    }
    /** Get the angle in degrees of the rotation around the specified axis. The axis must be normalized.
     * @param axisX the x component of the normalized axis for which to get the angle
     * @param axisY the y component of the normalized axis for which to get the angle
     * @param axisZ the z component of the normalized axis for which to get the angle
     * @return the angle in degrees of the rotation around the specified axis */
    getAngleAroundXYZ(axisX, axisY, axisZ) {
        return this.getAngleAroundRadXYZ(axisX, axisY, axisZ) * MathUtil_1.MathUtil.radiansToDegrees;
    }
    /** Get the angle in degrees of the rotation around the specified axis. The axis must be normalized.
     * @param axis the normalized axis for which to get the angle
     * @return the angle in degrees of the rotation around the specified axis */
    getAngleAroundAxis(axis) {
        return this.getAngleAroundXYZ(axis.x, axis.y, axis.z);
    }
    static len2XYZXYZ(x, y, z, w) {
        return x * x + y * y + z * z + w * w;
    }
}
exports.Quaternion3D = Quaternion3D;
//# sourceMappingURL=Quaternion3D.js.map