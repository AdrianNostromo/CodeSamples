package base.visual.util;

public class VisualValueConfig {

    public Object baseValue;
    public String[] baseMultiplierSourceID;
    public String baseMultiplierProperty;

    public int operationType;

    public VisualValueConfig secondarySmartValue;

    public static final int OPERATION_NONE = 0;

    // Single baseValue operations.
    public static final int OPERATION_ROUND = 1;
    public static final int OPERATION_FLOOR = 2;
    public static final int OPERATION_CEIL = 3;

    // Dual baseValue operations.
    public static final int OPERATION_ADD = 4;
    public static final int OPERATION_SUBTRACT = 5;
    public static final int OPERATION_MULTIPLY = 6;
    public static final int OPERATION_DIVIDE = 7;
    public static final int OPERATION_MIN = 8;
    public static final int OPERATION_MAX = 9;

    public VisualValueConfig(Object baseValue, String[] baseMultiplierSourceID, String baseMultiplierProperty, int operationType, VisualValueConfig secondarySmartValue) {
        super();

        this.baseValue = baseValue;
        this.baseMultiplierSourceID = baseMultiplierSourceID;
        this.baseMultiplierProperty = baseMultiplierProperty;

        this.operationType = operationType;

        this.secondarySmartValue = secondarySmartValue;
    }

    public VisualValueConfig(Object baseValue, String[] baseMultiplierSourceID, String baseMultiplierProperty, int operationType, Object secondaryValue, String[] secondaryMultiplierSourceID, String secondaryMultiplierProperty) {
        this(baseValue, baseMultiplierSourceID, baseMultiplierProperty, operationType, new VisualValueConfig(secondaryValue, secondaryMultiplierSourceID, secondaryMultiplierProperty));
    }

    public VisualValueConfig(Object baseValue, String[] baseMultiplierSourceID, String baseMultiplierProperty, int operationType) {
        this(baseValue, baseMultiplierSourceID, baseMultiplierProperty, operationType, null);
    }
    public VisualValueConfig(Object baseValue, String[] baseMultiplierSourceID, String baseMultiplierProperty) {
        this(baseValue, baseMultiplierSourceID, baseMultiplierProperty, OPERATION_NONE, null);
    }

    public VisualValueConfig(Object baseValue, int operationType) {
        this(baseValue, null, null, operationType, null);
    }

    public VisualValueConfig(Object baseValue) {
        this(baseValue, null, null, OPERATION_NONE, null);
    }

}
