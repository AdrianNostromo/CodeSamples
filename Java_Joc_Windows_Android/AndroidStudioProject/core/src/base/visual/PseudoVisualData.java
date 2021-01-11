package base.visual;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Adrin on 6/4/2016.
 */
public class PseudoVisualData implements IDynamicPropertiesObject {

    private Map<String, Object> propertiesMap;

    private Map<String, Boolean> staticAnimatingPropertiesMap = new HashMap<String, Boolean>();

    public PseudoVisualData() {
        super();

        propertiesMap = new HashMap<String, Object>();
    }

    @Override
    public Object getPropertyValue(String property) {
        return propertiesMap.get(property);
    }

    @Override
    public void setPropertyValue(String property, Object value) {
        propertiesMap.put(property, value);
    }

    public Boolean getStaticAnimatingPropertyStatus(String property) {
        return staticAnimatingPropertiesMap.get(property);
    }

    public void putStaticAnimatingPropertyStatus(String property, Boolean status) {
        staticAnimatingPropertiesMap.put(property, status);
    }

}
