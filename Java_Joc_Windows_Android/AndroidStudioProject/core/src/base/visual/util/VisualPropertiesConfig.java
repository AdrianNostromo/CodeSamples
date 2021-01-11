package base.visual.util;

import java.util.Vector;

import base.menu.util.MenuVisualAnimationProperty;

public class VisualPropertiesConfig {

    public Vector<VisualPropertyValueConfig> propertyConfigs = new Vector<VisualPropertyValueConfig>();

	public VisualPropertiesConfig() {
		super();

        //void
	}

    public VisualPropertiesConfig put(VisualPropertyValueConfig composedValue) {
        propertyConfigs.add(composedValue);

        return this;
    }

    public VisualPropertiesConfig put(String property,
                                      Object valueA, String[] multiplierSourceIDA, String multiplierPropertyA,
                                      int operationType,
                                      Object valueB, String[] multiplierSourceIDB, String multiplierPropertyB)
    {
        return put(new VisualPropertyValueConfig(property,
                new VisualValueConfig(
                        valueA, multiplierSourceIDA, multiplierPropertyA,
                        operationType,
                        new VisualValueConfig(valueB, multiplierSourceIDB, multiplierPropertyB)
                )
        ));
    }

    public VisualPropertiesConfig putAnimating(String property, MenuVisualAnimationProperty value) {
        return put(new VisualPropertyValueConfig(property, new VisualValueConfig(value, null, null)));
    }

    public VisualPropertiesConfig putManualValue(String property, VisualValueConfig smartValue) {
        return put(new VisualPropertyValueConfig(property, smartValue));
    }

    public VisualPropertiesConfig put(String property, Object value) {
        return put(new VisualPropertyValueConfig(property, new VisualValueConfig(value, null, null)));
    }

    public VisualPropertiesConfig put(String property, Object value, String[] multiplierSource, String multiplierParameter) {
        return put(new VisualPropertyValueConfig(property, new VisualValueConfig(value, multiplierSource, multiplierParameter)));
    }

    public VisualPropertiesConfig put(String property, Object value, String multiplierSource, String multiplierParameter) {
        return put(property, value, new String[] {multiplierSource}, multiplierParameter);
    }

}
