package base.menu;

import java.util.Map;
import java.util.Set;

/**
 * Created by Adrin on 8/16/2016.
 */
public interface IComposedVisual {

    void create();

    void appLoop(float deltaS, int deltaMS);

    void dispose();

    Object getLocalItem(String itemIDFragment);
    void setLocalItem(String itemIDFragment, Object item);

    Set<Map.Entry<String, Object>> entrySet();

}
