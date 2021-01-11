"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const Point2D_1 = require("base/util/Point2D");
const LogicError_1 = require("base/exception/LogicError");
class MathUtil {
    static randomIntInRange(min, max) {
        return Math.floor(Math.random() * (max - min + 1)) + min;
    }
    static normalizeAngleDeg(angDeg) {
        let newAngle = angDeg;
        while (newAngle <= -180) {
            newAngle += 360;
        }
        while (newAngle > 180) {
            newAngle -= 360;
        }
        return newAngle;
    }
    static dist(xA, yA, xB, yB) {
        let dist = Math.sqrt((xB - xA) * (xB - xA) + (yB - yA) * (yB - yA));
        return dist;
    }
    static toRadians(angdeg) {
        return angdeg / 180 * Math.PI;
    }
    static toDegrees(angrad) {
        return angrad * 180 / Math.PI;
    }
    static cartesianGridCoordsToUlamSpiralGridCoords(x, y) {
        if (x == 0 && y == 0) {
            return 1;
        }
        let b;
        let c;
        let unsignedBaseAxis;
        let signedOffsetAxis;
        // The spiral is not simetric on all sides, the ">" check must be used.
        if (Math.abs(x) > Math.abs(y)) {
            unsignedBaseAxis = Math.abs(x);
            if (x > 0) {
                signedOffsetAxis = y;
                b = -3;
                c = 1;
            }
            else {
                signedOffsetAxis = -y;
                b = 1;
                c = 1;
            }
        }
        else {
            unsignedBaseAxis = Math.abs(y);
            if (y > 0) {
                signedOffsetAxis = -x;
                b = -1;
                c = 1;
            }
            else {
                signedOffsetAxis = x;
                b = 3;
                c = 1;
            }
        }
        let n = (4 * Math.pow(unsignedBaseAxis, 2)) + (b * unsignedBaseAxis) + c;
        n += signedOffsetAxis;
        return n;
    }
    static ulamSpiralGridCoordsToCartesianGridCoords(n) {
        if (n < 0) {
            throw new LogicError_1.LogicError("MathUtil.UlamSpiralGridCoordsTocartesianGridCoords.1.");
        }
        if (n == 1) {
            return new Point2D_1.Point2D(0, 0);
        }
        // Diagonal is: 4*Math.pow(n, 2) (4,14,36,...)
        let layer_wrongDiagonal = Math.floor(Math.sqrt(n / 4));
        // Layer on the corect starter diagonal.
        // Uses the same logic as above but applies a offset to the value to move it to the corect diagonal starter value.
        // Diagonal is: 4*Math.pow(n, 2)-4*n-2 (2,10,26,...)
        let layer = Math.floor(Math.sqrt((n + 4 * layer_wrongDiagonal + 2) / 4));
        // Base cartesian coordonate values.
        // The coordonates of the current diagonal starter value.
        let x;
        let y;
        let starterLayerNumber = 4 * Math.pow(layer, 2) - 4 * layer + 2;
        let numOffset = n - starterLayerNumber;
        // The current layer is made out of 4 
        let segmentSize = 2 * layer;
        if (numOffset < segmentSize * 1) {
            // Number is on the right side (except lower right corner).
            x = layer;
            y = -layer + 1;
            y += numOffset;
        }
        else if (numOffset < segmentSize * 2) {
            // Number is on the top side (except top right corner).
            x = layer - 1;
            y = layer;
            x -= numOffset - segmentSize;
        }
        else if (numOffset < segmentSize * 3) {
            // Number is on the left side (except top left corner).
            x = -layer;
            y = layer - 1;
            y -= numOffset - segmentSize * 2;
        }
        else {
            // Number is on the bottom side (except bottom right corner).
            x = -layer + 1;
            y = -layer;
            x += numOffset - segmentSize * 3;
        }
        let ret = new Point2D_1.Point2D(x, y);
        return ret;
    }
    /** Returns true if a is nearly equal to b. The function uses the default floating error tolerance.
     * @param a the first value.
     * @param b the second value. */
    static isEqualNumberNumber(a, b) {
        return Math.abs(a - b) <= MathUtil.FLOAT_ROUNDING_ERROR;
    }
    /** Returns true if the value is zero (using the default tolerance as upper bound) */
    static isZeroNumber(value) {
        return Math.abs(value) <= MathUtil.FLOAT_ROUNDING_ERROR;
    }
    static clampValMinMax(value, min, max) {
        if (value < min)
            return min;
        if (value > max)
            return max;
        return value;
    }
}
/** multiply by this to convert from radians to degrees */
MathUtil.radiansToDegrees = 180 / Math.PI;
/** multiply by this to convert from degrees to radians */
MathUtil.degreesToRadians = Math.PI / 180;
MathUtil.FLOAT_ROUNDING_ERROR = 0.000001; // 32 bits
MathUtil.PI = 3.1415927;
MathUtil.PI2 = MathUtil.PI * 2;
exports.MathUtil = MathUtil;
//# sourceMappingURL=MathUtil.js.map