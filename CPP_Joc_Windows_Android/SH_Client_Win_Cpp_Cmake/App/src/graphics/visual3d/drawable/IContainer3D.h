#pragma once

#include <graphics/visual3d/drawable/IDrawableNode3D.h>

class IContainer3D : virtual public IDrawableNode3D {
public:
	virtual int getChildrensCount() = 0;
	virtual IDrawableNode3D* getChildAt(int index) = 0;
	virtual void addChild(IDrawableNode3D* drawable) = 0;
    virtual void removeChild(IDrawableNode3D* drawable) = 0;

    virtual void childSyncDrawSpecsRequested() = 0;

	~IContainer3D() override = default;
};
