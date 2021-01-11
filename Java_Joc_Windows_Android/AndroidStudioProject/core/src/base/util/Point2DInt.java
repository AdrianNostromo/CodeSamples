package base.util;

import base.json.JSONObject;

public class Point2DInt {

    public int x;
    public int y;

    public Point2DInt(int x, int y) {
        super();

	    this.x = x;
        this.y = y;
    }
    
    public void set(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public JSONObject toJSON() {
        JSONObject jsonObject = new JSONObject();

        jsonObject.put("x", x);
        jsonObject.put("y", y);

        return jsonObject;
    }

    public static final Point2DInt fromJSON(JSONObject jsonObject) {
        int x = jsonObject.getInt("x");
        int y = jsonObject.getInt("y");

        Point2DInt ret = new Point2DInt(x, y);

        return ret;
    }

}
