export class Point3D {

  public x: number;
  public y: number;
  public z: number;

  public constructor(
      x: number,
      y: number,
      z: number
  ) {
    this.x = x;
    this.y = y;
    this.z = z;
  }
  
  public set(value: Point3D): void {
    this.x = value.x;
    this.y = value.y;
    this.z = value.z;
  }
  
  public setXYZ(x: number, y: number, z: number): void {
    this.x = x;
    this.y = y;
    this.z = z;
  }
  
  public equals(value: Point3D): boolean {
    if(this.x != value.x || this.y != value.y || this.z != value.z) {
      return false;
    }

    return true;
  }
  
  public equalsXYZ(x: number, y: number, z: number): boolean {
    if(this.x != x || this.y != y || this.z != z) {
      return false;
    }

    return true;
  }
  
}
