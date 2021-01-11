#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/GeometryDrawable2D.h>
#include <graphics/visual2d/drawable/ISizedDrawable2D.h>

class SizedDrawable2D : public GeometryDrawable2D, virtual public ISizedDrawable2D {priv typedef GeometryDrawable2D super;pub dCtor(SizedDrawable2D);
    priv float width = 0;
    priv float height = 0;

    priv float anchorX = 0.0f;
    priv float anchorY = 0.0f;

    prot bool invertedRenderingAxisX = false;
    prot bool invertedRenderingAxisY = false;

    pub explicit SizedDrawable2D();

    pub float getWidth() override { return width; };
    pub void setWidth(float width) override;

    pub float getHeight() override { return height; }
    pub void setHeight(float height) override;

    pub void setSize(float width, float height) override;

    pub float getAnchorX() override { return anchorX; }
    pub void setAnchorX(float anchorX) override;
    
    pub float getAnchorY() override { return anchorY; }
    pub void setAnchorY(float anchorY) override;
    pub void setAnchor(float anchorX, float anchorY) override;

    prot void syncRenderingTransformSizing(Affine2& renderingTransformAffine2) override;

    pub std::shared_ptr<base::IWrappedValue> getPropertyValue(std::string property) override;
    pub void setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) override;

    prot void disposeMain() override;
    pub ~SizedDrawable2D() override;
};
