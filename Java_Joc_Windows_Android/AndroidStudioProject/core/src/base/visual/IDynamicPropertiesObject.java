package base.visual;

// Interface for visual elements that are drawn on the screen.
public interface IDynamicPropertiesObject {

    Object getPropertyValue(String property);
    void setPropertyValue(String property, Object value);

    // null, false, true;
    Boolean getStaticAnimatingPropertyStatus(String property);
    void putStaticAnimatingPropertyStatus(String property, Boolean status);

}
