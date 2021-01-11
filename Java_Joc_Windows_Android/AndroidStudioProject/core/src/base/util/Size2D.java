package base.util;

import base.json.JSONObject;

public class Size2D {

    public float width;
    public float height;

    public Size2D(float width, float height) {
        super();

	    this.width = width;
        this.height = height;
    }

    public JSONObject toJSON() {
        JSONObject jsonObject = new JSONObject();

        jsonObject.put("width", width);
        jsonObject.put("height", height);

        return jsonObject;
    }

    public static final Size2D fromJSON(JSONObject jsonObject) {
        float width = (float)jsonObject.getDouble("width");
        float height = (float)jsonObject.getDouble("height");

        Size2D ret = new Size2D(width, height);

        return ret;
    }

}
