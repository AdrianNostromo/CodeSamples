#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/IDrawableNode2D.h>
#include <graphics/visual/Color.h>
#include <string>
#include <base/map/Map1D.h>
#include <graphics/visual/Texture.h>
#include <graphics/visual2d/drawable/IContainer2D.h>
#include <base/list/ListDL.h>

class DrawableNode2D : public base::Object, virtual public IDrawableNode2D {priv typedef base::Object super;pub dCtor(DrawableNode2D);
    pub class InvalidationFlags {
        pub static const unsigned int empty = 0;

        pub static const unsigned int transform = 1;
        pub static const unsigned int alpha = 2;
        pub static const unsigned int tintColor = 3;
		// This is invalidated when the render data should be removed.
		// This is used so than the parent container will do a sync and cache the correct last_child or self render_data.
		// This is an empty, it is used just to make the parent sync.
        pub static const unsigned int childRenderDataCacheSystem = 4;

        pub static const unsigned int LAST_USED_BIT_INDEX = childRenderDataCacheSystem;
    };

    priv float x = 0.0f;
    priv float y = 0.0f;

    priv float scaleX = 1.0f;
    priv float scaleY = 1.0f;

    priv float rotationD = 0.0f;

    priv Color tintColor{1.0f, 1.0f, 1.0f, 1.0f};
    priv float alpha = 1.0f;

	// This is a soft visibility value.
	// This just sets isVisibleCounter as needed.
	// Don't check this value.
    priv bool isVisiblePseudo = true;
	// 1: visible;
	// <1: hidden;
	// >1: logic error;
    priv int isVisibleCounter = 1;
    priv bool parentGlobal_isVisible = true;

    priv IContainer2D* parent = nullptr;
    priv ListDL<IDrawableNode2D*>::Entry* inParent_drawablesListEntry = nullptr;
    // Keep this here because containers are also added to parents with this sistem.
    /// It is a requirement to find previous drawable with renderData.
    priv ListDL<IDrawableNode2D*>::Entry* inParent_drawablesWithRenderDataListEntry = nullptr;
    priv ListDL<IDrawableNode2D*>::Entry* inParent_drawablesWithInputInterractionListEntry = nullptr;

    priv IRoot2D* root = nullptr;

    priv Affine2 localTransform;
	// This is used because of transform preupdate logic.
    priv bool isLocalTransformDirty = true;

    priv Affine2 globalTransform;
	// globalTransform can be used only if this is true;
    priv bool isGlobalTransformInit = false;
	// This is used for the transform preupdate logic.
    priv int globalTransformPreupdateIndex = -1;

    priv float globalAlpha = 1.0f;
    priv Color globalTintColor{1.0f, 1.0f, 1.0f, 1.0f};

    priv Affine2 t1{};
    priv Affine2 t2{};
    priv Vector2 v1{};

    // Pseudo Boolean.
    // -1 : Not set;
    // 0 : false;
    // 1 : true;
    priv Map1D<std::string, int> staticAnimatingPropertiesMap;

    priv bool isSyncDrawSpecsPreInProgress = false;
    priv bool isSyncDrawSpecsInProgress = false;

    prot int invalidationBitMap = 0;
    // This bit map caches invalidation flags when not visible or no parent.
    // This cache is activated on parent set and visible set.
    prot int inactiveInvalidationBitMap = -1;

    //asd_01;// the stage list is not used in the new system, remove it.
    priv ListDL<IRenderOrderDrawableNode2D*>::Entry* inStage_renderOrderDrawablesListEntry = nullptr;
    IRenderOrderDrawableNode2D* previousDrawableWithRenderable = nullptr;

    pub explicit DrawableNode2D();
    pub void reservedCreate();
    prot virtual void createMain();

    pub void invalidateDrawSpecs(int invalidationBitMap) override;

    pub float getX() override { return x; };
    pub void setX(float x) override;
    pub float getY() override {return y; };
    pub void setY(float y) override;
    pub void setPosition(float x, float y) override;

    pub float getScaleX() override { return scaleX; };
    pub void setScaleX(float scaleX) override;
    pub float getScaleY() override { return scaleY; };
    pub void setScaleY(float scaleY) override;

    pub void setScale(float scaleX, float scaleY) override;

    pub float getRotationD() override { return rotationD; };
    pub void setRotationD(float rotationD) override;

    pub Color* getTintColor() override { return &tintColor; };
    pub void setTintColor(Color& tintColor) override;
    pub float getAlpha() override { return alpha; };
    pub void setAlpha(float alpha) override;

    pub bool getIsVisible() override;
    pub void setIsVisibleIncremental(bool isVisible) final;
    pub void setIsVisible(bool isVisible) final;
    pub virtual void onIsVisibleChangePre(int old_isVisibleCounter, int new_isVisibleCounter);
    pub virtual void onIsVisibleChanged();

    pub bool getGlobal_isVisible();
    pub void setParentGlobal_isVisible(bool parentGlobal_isVisible) override;
    pub virtual void onGlobal_isVisibleChanged();

    pub IContainer2D* getParent() override { return parent; };

    pub float getGlobalAlpha() override { return globalAlpha; };
    pub Color& getGlobalTintColor() override { return globalTintColor; }

    pub Affine2& getGlobalTransform(bool updateTransformStackIfDirty) override;
    pub virtual bool getIsGlobalTransformInit() override;

    pub std::shared_ptr<base::IWrappedValue> getPropertyValue(std::string property) override;
    pub void setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) override;

    pub IRoot2D* getRoot() override { return root; };
    pub bool isSyncDrawSpecsRequired() override;
    pub int syncDrawSpecsWhereNeeded(const int parentInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D** preRenderData) override;

    pub bool isInit_StaticAnimatingPropertyStatus(std::string& property) override;
    pub bool isTrue_StaticAnimatingPropertyStatus(std::string& property) override;
    pub void putStaticAnimatingPropertyStatus(std::string& property, int status) override;

    // In some cases (fire a projectile after an instant rot on the same tick frame) the transform for a certain visual branch must be
    // updated instantly, call this in a controlled way to achieve that. The transform will still be updated again.
    priv int update_transform();

    // Returns true if update_transform layer found and update started.
    priv static bool DoRecursiveInverseOptionalTransformSoftUpdate(DrawableNode2D* currentLayerDrawable);

    prot void setParent(IContainer2D* parent) final;
    prot virtual void onParentChangePre();
    prot virtual void onParentChanged();

    pub ListDL<IDrawableNode2D*>::Entry*& getInParent_drawablesListEntryRef() final;
    pub ListDL<IDrawableNode2D*>::Entry*& getInParent_drawablesWithRenderDataListEntryRef() final;
    pub ListDL<IDrawableNode2D*>::Entry*& getInParent_drawablesWithInputInterractionListEntryRef() final;
    
    prot void setRoot(IRoot2D* root) final;
    prot virtual void onRootChanged_pre();
    prot virtual void onRootChanged();

    // This can cause invalidations to occur.
    prot virtual int syncDrawSpecsPre(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData);
    prot virtual int syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData);

    prot void worldPosToLocalPos(Vector2& screenPos, Vector2& oLocalPos) override;

    pub ListDL<IRenderOrderDrawableNode2D*>::Entry*& getInStage_renderOrderDrawablesListEntryRef() final;
    pub IRenderOrderDrawableNode2D*& getPreviousDrawableWithRenderableRef() final;

    pub IRenderOrderDrawableNode2D* computePreviousOrParentDrawableInStageRenderOrder() final;

    // Fully extended by SizedDrawable2D;
    pub bool getHasRenderable() override;
    // Fully extended by SizedDrawable2D;
    pub graphics::RenderableLiteInstanceVI* getRenderData() override;

    prot void disposeMain() override;
    pub ~DrawableNode2D() override;
};

/*asdB;

    private Rectangle maskRectangle = null;

    public Rectangle getMaskRectangle() {
        return maskRectangle;
    }

    public void setMaskRectangle(Rectangle maskRectangle) {
        this.maskRectangle = maskRectangle;
    }

    // Extendable.
    abstract protected void drawVisual(IBatch2D batch, OrthographicCamera camera, float deltaS);

    private Affine2 t1 = new Affine2();
    private Affine2 t2 = new Affine2();
    private Vector2 v1 = new Vector2();
    public Vector2 worldPosToLocalPos(float screenX, float screenY) {
        t1.idt();
        t1.translate(screenX, screenY);

        t2.set(getGlobalTransform(false));
        t2.inv();
        t1.mul(t2);

        v1 = t1.getTranslation(v1);

        return v1;
    }

}
*/
