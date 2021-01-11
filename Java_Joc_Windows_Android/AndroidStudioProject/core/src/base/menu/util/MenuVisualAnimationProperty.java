package base.menu.util;

import java.util.Vector;

import base.visual.util.VisualValueConfig;

public class MenuVisualAnimationProperty {

    public class StateComposedValue {

        // String or String[]. In case of String[], all entries must pass against the menu stateID for the baseValue to be used.
        public Object stateIDFragments;

        public EaseConfig easeConfig;

        public VisualValueConfig smartValue;

        public StateComposedValue(Object stateIDFragments, EaseConfig easeConfig, VisualValueConfig smartValue) {
            super();

            this.stateIDFragments = stateIDFragments;

            this.easeConfig = easeConfig;

            this.smartValue = smartValue;
        }
    }

    public Vector<StateComposedValue> stateValuesList = new Vector<StateComposedValue>();

	public MenuVisualAnimationProperty() {
		super();

        //void
	}

    public MenuVisualAnimationProperty putState(Object stateIDFragments, Object value, String[] multiplierSourceID, String multiplierProperty, EaseConfig easeConfig) {
        stateValuesList.add(new StateComposedValue(stateIDFragments, easeConfig, new VisualValueConfig(value, multiplierSourceID, multiplierProperty)));

        return this;
    }

    public MenuVisualAnimationProperty putState(Object stateIDFragments, VisualValueConfig smartValue, EaseConfig easeConfig) {
        stateValuesList.add(new StateComposedValue(stateIDFragments, easeConfig, smartValue));

        return this;
    }

    public MenuVisualAnimationProperty putState(Object stateIDFragments,
                                                Object baseValue, String[] baseMultiplierSourceID, String baseMultiplierProperty,
                                                int operationType,
                                                Object secondaryValue, String[] secondaryMultiplierSourceID, String secondaryMultiplierProperty,
                                                EaseConfig easeConfig
    ) {
        stateValuesList.add(new StateComposedValue(stateIDFragments, easeConfig, new VisualValueConfig(
                baseValue, baseMultiplierSourceID, baseMultiplierProperty,
                operationType,
                secondaryValue, secondaryMultiplierSourceID, secondaryMultiplierProperty
        )));

        return this;
    }

}
