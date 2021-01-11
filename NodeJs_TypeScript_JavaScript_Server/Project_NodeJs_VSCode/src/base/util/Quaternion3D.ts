import { Vector3 } from "base/util/Vector3";
import { MathUtil } from "base/util/MathUtil";

export class Quaternion3D {

  public static fromXYZW(x: number, y: number, z: number, w: number): Quaternion3D {
    return new Quaternion3D().setXYZW(x, y, z, w);
  }

  public static fromQ(q: Quaternion3D): Quaternion3D {
    return new Quaternion3D().setQ(q);
  }

  public static fromIdt(): Quaternion3D {
    return new Quaternion3D();
  }

  public static fromAxisAngle(axis: Vector3, angle: number): Quaternion3D {
    return new Quaternion3D().setAxisAngle(axis, angle);
  }

  public x: number;
  public y: number;
  public z: number;
  public w: number;

  public constructor() {
    this.idt();
  }
  
  public setXYZW(x: number, y: number, z: number, w: number): Quaternion3D {
    this.x = x;
		this.y = y;
		this.z = z;
    this.w = w;
    
		return this;
  }
  
  public setQ(q: Quaternion3D): Quaternion3D {
    this.x = q.x;
		this.y = q.y;
		this.z = q.z;
    this.w = q.w;
    
		return this;
  }
  
  public setAxisAngle(axis: Vector3, angle: number): Quaternion3D {
    return this.setFromAxis(axis.x, axis.y, axis.z, angle);
  }
  
  public setFromAxis(x: number, y: number, z: number, degrees: number): Quaternion3D {
    return this.setFromAxisRad(x, y, z, degrees * MathUtil.degreesToRadians);
  }

	/** Sets the quaternion components from the given axis and angle around that axis.
	 * @param x X direction of the axis
	 * @param y Y direction of the axis
	 * @param z Z direction of the axis
	 * @param radians The angle in radians
	 * @return This quaternion for chaining. */
	public setFromAxisRad(x: number, y: number, z: number, radians: number) {
		let d: number = Vector3.len(x, y, z);
		if (d == 0) return this.idt();
		d = 1 / d;
		let l_ang: number = radians < 0 ? MathUtil.PI2 - (-radians % MathUtil.PI2) : radians % MathUtil.PI2;
		let l_sin: number = Math.sin(l_ang / 2);
		let l_cos: number = Math.cos(l_ang / 2);
		return this.setXYZW(d * x * l_sin, d * y * l_sin, d * z * l_sin, l_cos).nor();
	}

  public idt(): Quaternion3D {
    return this.setXYZW(0, 0, 0, 1);
  }

	/** Normalizes this quaternion to unit length
	 * @return the quaternion for chaining */
  public nor(): Quaternion3D {
    let len: number = this.len2();
		if (len != 0. && !MathUtil.isEqualNumberNumber(len, 1)) {
			len = Math.sqrt(len);
			this.w /= len;
			this.x /= len;
			this.y /= len;
			this.z /= len;
		}
		return this;
  }

	/** @return the length of this quaternion without square root */
	public len2 (): number {
		return this.x * this.x + this.y * this.y + this.z * this.z + this.w * this.w;
	}

  public equalsQ(q: Quaternion3D): boolean {
    if(this.x != q.x || this.y != q.y || this.z != q.z || this.w != q.w) {
      return false;
    }

    return true;
  }

	/** Get the angle in radians of the rotation around the specified axis. The axis must be normalized.
	 * @param axisX the x component of the normalized axis for which to get the angle
	 * @param axisY the y component of the normalized axis for which to get the angle
	 * @param axisZ the z component of the normalized axis for which to get the angle
	 * @return the angle in radians of the rotation around the specified axis */
	public getAngleAroundRadXYZ (axisX: number, axisY: number, axisZ: number): number {
		let d: number = Vector3.dotXYZXYZ(this.x, this.y, this.z, axisX, axisY, axisZ);
		let l2: number = Quaternion3D.len2XYZXYZ(axisX * d, axisY * d, axisZ * d, this.w);
		return MathUtil.isZeroNumber(l2) ? 0 : (2.0 * Math.acos(MathUtil.clampValMinMax(
			((d < 0 ? -this.w : this.w) / Math.sqrt(l2)), -1, 1)));
	}

	/** Get the angle in radians of the rotation around the specified axis. The axis must be normalized.
	 * @param axis the normalized axis for which to get the angle
	 * @return the angle in radians of the rotation around the specified axis */
	public getAngleAroundRadAxis (axis: Vector3): number {
		return this.getAngleAroundRadXYZ(axis.x, axis.y, axis.z);
	}

	/** Get the angle in degrees of the rotation around the specified axis. The axis must be normalized.
	 * @param axisX the x component of the normalized axis for which to get the angle
	 * @param axisY the y component of the normalized axis for which to get the angle
	 * @param axisZ the z component of the normalized axis for which to get the angle
	 * @return the angle in degrees of the rotation around the specified axis */
	public getAngleAroundXYZ (axisX: number, axisY: number, axisZ: number): number {
		return this.getAngleAroundRadXYZ(axisX, axisY, axisZ) * MathUtil.radiansToDegrees;
	}

	/** Get the angle in degrees of the rotation around the specified axis. The axis must be normalized.
	 * @param axis the normalized axis for which to get the angle
	 * @return the angle in degrees of the rotation around the specified axis */
	public getAngleAroundAxis(axis: Vector3): number {
		return this.getAngleAroundXYZ(axis.x, axis.y, axis.z);
  }
  
	public static len2XYZXYZ(x: number, y: number, z: number, w: number): number {
		return x * x + y * y + z * z + w * w;
	}

}
