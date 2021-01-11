package base.util;

import base.json.JSONObject;

public class Point2D {

    public float x;
    public float y;

    public Point2D(float x, float y) {
        super();

	    this.x = x;
        this.y = y;
    }

    public Point2D() {
        this(0, 0);
    }

    public void set(float x, float y) {
        this.x = x;
        this.y = y;
    }

    public float computeDist() {
        return MathUtil.dist(0f, 0f, x, y);
    }

    public JSONObject toJSON() {
        JSONObject jsonObject = new JSONObject();

        jsonObject.put("x", x);
        jsonObject.put("y", y);

        return jsonObject;
    }

    public static final Point2D fromJSON(JSONObject jsonObject) {
        float x = (float)jsonObject.getDouble("x");
        float y = (float)jsonObject.getDouble("y");

        Point2D ret = new Point2D(x, y);

        return ret;
    }

}
