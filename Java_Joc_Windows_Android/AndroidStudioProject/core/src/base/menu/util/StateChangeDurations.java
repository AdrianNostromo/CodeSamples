package base.menu.util;

/**
 * Created by Adrin on 6/23/2016.
 */
public class StateChangeDurations {

    // String or String[]
    public Object startComposedStateID;
    // String or String[]
    public Object endComposedStateID;

    public float durationS;

    public StateChangeDurations(Object startComposedStateID, Object endComposedStateID, float durationS) {
        super();

        this.startComposedStateID = startComposedStateID;
        this.endComposedStateID = endComposedStateID;

        this.durationS = durationS;
    }

}
