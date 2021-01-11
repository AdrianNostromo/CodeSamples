#include "SizedDrawable2D.h"
#include <base/math/util/BoolUtil.h>
#include <assert.h>
#include <base/math/Math.h>

SizedDrawable2D::SizedDrawable2D()
        :super()
{
    //void
}

void SizedDrawable2D::setWidth(float width) {
    if (this->width == width) {
        return;
    }

    this->width = width;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
        InvalidationFlags::transform
    ));
}

void SizedDrawable2D::setHeight(float height) {
    if (this->height == height) {
        return;
    }

    this->height = height;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
        InvalidationFlags::transform
    ));
}

void SizedDrawable2D::setSize(float width, float height) {
    if (this->width == width && this->height == height) {
        return;
    }

    this->width = width;
    this->height = height;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
        InvalidationFlags::transform
    ));
}

void SizedDrawable2D::setAnchorX(float anchorX) {
    if (this->anchorX == anchorX) {
        return ;
    }

    this->anchorX = anchorX;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
        InvalidationFlags::transform
    ));
}

void SizedDrawable2D::setAnchorY(float anchorY) {
    if (this->anchorY == anchorY) {
        return;
    }

    this->anchorY = anchorY;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
        InvalidationFlags::transform
    ));
}

void SizedDrawable2D::setAnchor(float anchorX, float anchorY) {
    if (this->anchorX == anchorX && this->anchorY == anchorY) {
        return;
    }

    this->anchorX = anchorX;
    this->anchorY = anchorY;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
        InvalidationFlags::transform
    ));
}

void SizedDrawable2D::syncRenderingTransformSizing(Affine2& renderingTransformAffine2) {
    super::syncRenderingTransformSizing(renderingTransformAffine2);

    renderingTransformAffine2.translate(
        width * ((invertedRenderingAxisX) ? 1 : 0) - width * getAnchorX(),
        height * ((invertedRenderingAxisY) ? 1 : 0) - height * getAnchorY()
    );
}

std::shared_ptr<base::IWrappedValue> SizedDrawable2D::getPropertyValue(std::string property) {
    if(property == "width") {
        return base::IWrappedValue::new_float(width);
    }else if(property == "height") {
        return base::IWrappedValue::new_float(height);
    }

    else if(property == "anchorX") {
        return base::IWrappedValue::new_float(anchorX);
    }else if(property == "anchorY") {
        return base::IWrappedValue::new_float(anchorY);
    }

    return super::getPropertyValue(property);
}

void SizedDrawable2D::setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) {
    if(property == "width") {
        setWidth(value->getDirectAs_float());
    }else if(property == "height") {
        setHeight(value->getDirectAs_float());
    }

    else if(property == "anchorX") {
        setAnchorX(value->getDirectAs_float());
    }else if(property == "anchorY") {
        setAnchorY(value->getDirectAs_float());
    }

    else {
        super::setPropertyValue(property, value);
    }
}

void SizedDrawable2D::disposeMain() {
    //void

    super::disposeMain();
}

SizedDrawable2D::~SizedDrawable2D() {
    //void
}
