
export class Vector3 {

  public static fromXYZ(x: number, y: number, z: number): Vector3 {
    return new Vector3().setXYZ(x, y, z);
  }

	public static readonly X: Vector3 = Vector3.fromXYZ(1, 0, 0);
	public static readonly Y: Vector3 = Vector3.fromXYZ(0, 1, 0);
	public static readonly Z: Vector3 = Vector3.fromXYZ(0, 0, 1);
	public static readonly Zero: Vector3 = Vector3.fromXYZ(0, 0, 0);

	/** @return The euclidean length */
	public static len (x: number, y: number, z: number): number {
		return Math.sqrt(x * x + y * y + z * z);
	}

  public x: number;
  public y: number;
  public z: number;

  public constructor() {
    this.setXYZ(0, 0, 0);

    //void
  }
  
  public setXYZ(x: number, y: number, z: number): Vector3 {
    this.x = x;
		this.y = y;
		this.z = z;
    
		return this;
  }
  
  public setV(v: Vector3): Vector3 {
    this.x = v.x;
		this.y = v.y;
		this.z = v.z;
    
		return this;
  }

  public equalsQ(q: Vector3): boolean {
    if(this.x != q.x || this.y != q.y || this.z != q.z) {
      return false;
    }

    return true;
  }

	/** @return The dot product between the two vectors */
	public static dotXYZXYZ(x1: number, y1: number, z1: number, x2: number, y2: number, z2: number): number {
		return x1 * x2 + y1 * y2 + z1 * z2;
	}

}
