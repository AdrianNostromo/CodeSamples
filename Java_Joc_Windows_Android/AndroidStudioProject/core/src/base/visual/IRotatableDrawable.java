package base.visual;

// Interface for visuals that can interact with touch events.
public interface IRotatableDrawable extends IDrawable2D {

    float getRotationD();
    void setRotationD(float rotationD);

}
