#pragma once

#include <graphics/visual3d/drawable/DrawableNode3D.h>
#include <graphics/visual3d/drawable/IContainer3D.h>
#include <base/list/ArrayList.h>

class Container3D : public DrawableNode3D, virtual public IContainer3D {
private: typedef DrawableNode3D super;
private:
	ArrayList<IDrawableNode3D*> drawablesList{};

	bool isAnyChildSyncDrawSpecsRequired = true;
public:
	Container3D(Container3D const&) = delete;
	Container3D(Container3D &&) = default;
	Container3D& operator=(Container3D const&) = delete;
	Container3D& operator=(Container3D &&) = default;

	explicit Container3D();

	void childSyncDrawSpecsRequested() override;
	int getChildrensCount() override;
	IDrawableNode3D* getChildAt(int index) override;
	void addChild(IDrawableNode3D* drawable) override;
	void addChildAt(IDrawableNode3D* drawable, unsigned int childIndex);
	void removeChild(IDrawableNode3D* drawable) override;
    void onGlobal_isVisibleChanged() override;
    void onRootChanged() override;

	int syncDrawSpecsWhereNeeded(const int parentInvalidationBitMap, graphics::IRenderer& renderer) override;

	float getGlobalAlpha() override;
	bool isSyncDrawSpecsRequired() override;

	~Container3D() override;
protected:
	void disposeMain() override;

};
