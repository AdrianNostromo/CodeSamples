package base.util;

public class Bounds2DInt {

    public int left;
    public int top;

    public int right;
    public int bottom;

    public Bounds2DInt(int left, int top, int right, int bottom) {
        super();

	    this.left = left;
        this.top = top;

        this.right = right;
        this.bottom = bottom;
    }

    public int getWidth() {
        return right - left;
    }

    public int getHeight() {
        return top - bottom;
    }

}
