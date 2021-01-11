package base.util;

public class Rectangle2D {

    public float x;
    public float y;

    public float width;
    public float height;

    public Rectangle2D(float x, float y, float width, float height) {
        super();

	    this.x = x;
        this.y = y;

        this.width = width;
        this.height = height;
    }

    public float getHCenter() {
        return this.x + this.width / 2f;
    }

    public float getVCenter() {
        return this.y + this.height / 2f;
    }

    public float getRight() {
        return this.x + this.width;
    }

    public float getBottom() {
        return this.y + this.height;
    }

}
