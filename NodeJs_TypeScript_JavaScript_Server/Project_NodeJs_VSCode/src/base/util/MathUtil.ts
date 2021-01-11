import { Point2D } from "base/util/Point2D";
import { LogicError } from "base/exception/LogicError";

export class MathUtil {

  /** multiply by this to convert from radians to degrees */
  public static readonly radiansToDegrees: number = 180 / Math.PI;
  
  /** multiply by this to convert from degrees to radians */
  public static readonly degreesToRadians: number = Math.PI / 180;
  
  public static readonly FLOAT_ROUNDING_ERROR: number = 0.000001; // 32 bits

  public static readonly PI: number = 3.1415927;
  public static readonly PI2: number = MathUtil.PI * 2;

  public static randomIntInRange(min: number, max: number): number {
    return Math.floor(Math.random() * (max - min + 1)) + min;
  }

  public static normalizeAngleDeg(angDeg: number): number {
    let newAngle: number = angDeg;
    while (newAngle <= -180) {
        newAngle += 360;
    }
    while (newAngle > 180) {
        newAngle -= 360;
    }
    return newAngle;
  }
  
  public static dist(xA: number, yA: number, xB: number, yB: number): number {
    let dist: number = Math.sqrt((xB - xA)*(xB - xA) + (yB - yA)*(yB - yA));

    return dist;
  }
  
	public static toRadians(angdeg: number): number {
		return angdeg / 180 * Math.PI;
	}

	public static toDegrees(angrad: number): number {
		return angrad * 180 / Math.PI;
	}

  public static cartesianGridCoordsToUlamSpiralGridCoords(x: number, y: number): number {
    if(x == 0 && y == 0) {
      return 1;
    }

    let b: number;
    let c: number;
    let unsignedBaseAxis: number;
    let signedOffsetAxis: number;

    // The spiral is not simetric on all sides, the ">" check must be used.
    if(Math.abs(x) > Math.abs(y)) {
      unsignedBaseAxis = Math.abs(x);

      if(x > 0) {
        signedOffsetAxis = y;

        b = -3;
        c = 1;
      }else {
        signedOffsetAxis = -y;

        b = 1;
        c = 1;
      }
    }else {
      unsignedBaseAxis = Math.abs(y);

      if(y > 0) {
        signedOffsetAxis = -x;

        b = -1;
        c = 1;
      }else {
        signedOffsetAxis = x;

        b = 3;
        c = 1;
      }
    }

    let n: number = (4 * Math.pow(unsignedBaseAxis, 2)) + (b * unsignedBaseAxis) + c;
    n += signedOffsetAxis;

    return n;
  }
  
  public static ulamSpiralGridCoordsToCartesianGridCoords(n: number): Point2D {
    if(n < 0) {
      throw new LogicError("MathUtil.UlamSpiralGridCoordsTocartesianGridCoords.1.");
    }

    if(n == 1) {
      return new Point2D(
        0,
        0
      );
    }
    // Diagonal is: 4*Math.pow(n, 2) (4,14,36,...)
    let layer_wrongDiagonal: number = Math.floor(Math.sqrt(n/4));
    
    // Layer on the corect starter diagonal.
    // Uses the same logic as above but applies a offset to the value to move it to the corect diagonal starter value.
    // Diagonal is: 4*Math.pow(n, 2)-4*n-2 (2,10,26,...)
    let layer: number = Math.floor(Math.sqrt((n+4*layer_wrongDiagonal+2)/4))

    // Base cartesian coordonate values.
    // The coordonates of the current diagonal starter value.
    let x: number;
    let y: number;

    let starterLayerNumber: number = 4*Math.pow(layer, 2)-4*layer+2
    let numOffset: number = n - starterLayerNumber;

    // The current layer is made out of 4 
    let segmentSize: number = 2 * layer;

    if(numOffset < segmentSize * 1) {
      // Number is on the right side (except lower right corner).
      x = layer;
      y = -layer + 1;

      y += numOffset;
    }else if(numOffset < segmentSize * 2) {
      // Number is on the top side (except top right corner).
      x = layer - 1;
      y = layer;

      x -= numOffset - segmentSize;
    }else if(numOffset < segmentSize * 3) {
      // Number is on the left side (except top left corner).
      x = -layer;
      y = layer - 1;

      y -= numOffset - segmentSize * 2;
    }else {
      // Number is on the bottom side (except bottom right corner).
      x = -layer + 1;
      y = -layer;
      
      x += numOffset - segmentSize * 3;
    }

    let ret: Point2D = new Point2D(x, y);

    return ret;
  }
  
	/** Returns true if a is nearly equal to b. The function uses the default floating error tolerance.
	 * @param a the first value.
	 * @param b the second value. */
	public static isEqualNumberNumber(a: number, b: number): boolean {
		return Math.abs(a - b) <= MathUtil.FLOAT_ROUNDING_ERROR;
	}

	/** Returns true if the value is zero (using the default tolerance as upper bound) */
	public static isZeroNumber(value: number): boolean {
		return Math.abs(value) <= MathUtil.FLOAT_ROUNDING_ERROR;
	}

	public static clampValMinMax(value: number, min: number, max: number): number {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

}
