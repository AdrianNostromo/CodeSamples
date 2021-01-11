#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/DrawableNode2D.h>
#include <graphics/visual2d/drawable/IContainer2D.h>
#include <base/list/ArrayList.h>
#include <base/list/ListDL.h>
#include <base/MM.h>

class Container2D : public DrawableNode2D, virtual public IContainer2D {priv typedef DrawableNode2D super;pub dCtor(Container2D);
    // This is the number of buttons that can be used (in order: LMB, RMB, MMB, 4, 5, ...) with u.i. elements.
    /// This limitation exists because containers need to use a fast conter system for each button type for each child.
    prot static constexpr int USER_INTERFACE_INTERRACTIONS_BUTTONS_COUNT = 3;

    priv ListDL<IDrawableNode2D*> drawablesList{LOC};
    // This contains drawables (Container2D or GeometryDrawable2D) that are in the renderer or containers that have drawables in renderer leafs.
    priv ListDL<IDrawableNode2D*> drawablesWithRenderDataList{LOC};
    priv ListDL<IDrawableNode2D*> drawablesWithInputInterractionList{LOC};

    priv bool isAnyChildSyncDrawSpecsRequired = true;

    priv bool isTouchBoxChanged = false;

    pub explicit Container2D();

    pub void childSyncDrawSpecsRequested() override;

    pub void addChild(IDrawableNode2D* drawable) override;
    pub void removeChild(IDrawableNode2D* drawable) override;

    pub IDrawableNode2D* onChildAddingToRendererAndGetPrevDrawableWithRenderData(IDrawableNode2D* leafDrawableOrFirstLevelLocalChild) final;
    pub void onChildRemovedFromRenderer(IDrawableNode2D* drawable) final;

    pub void onGlobal_isVisibleChanged() override;
    pub void onRootChanged() override;

    pub void onIsVisibleChanged() override;

    pub bool getIsInputInteractive() override;
    pub void onChildIsInputInteractiveChanged(IDrawableNode2D* drawable, bool childIsInputInteractive) override;

    pub void onTouchBoxChanged() override;
    pub void clearIsTouchBoxChanged() override;

    pub IDrawableNode2D* getDisplayTreeMinLeafDrawableWithRenderData() final;
    pub IDrawableNode2D* getDisplayTreeMaxLeafDrawableWithRenderData() final;

    pub int syncDrawSpecsWhereNeeded(const int parentInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D** preRenderData) override;

    pub Affine2& getGlobalTransform(bool updateTransformStackIfDirty) override;
    pub float getGlobalAlpha() override;
    pub Color& getGlobalTintColor() override;
    pub bool isSyncDrawSpecsRequired() override;

    pub bool worldTouchEvent(
        TouchEventType* eventType,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2* deltaPosW,
        base::CursorHandlingData* cursorHD, base::CursorHandlingData::ButtonHD* cursorButtonHD,
        int eventIndex) override;

    prot void onParentChangePre() override;
    prot void onParentChanged() override;

    prot virtual void onIsInputInteractiveChanged();

    prot bool getIsTouchBoxChanged();

    // Events are dispatch from this container because there is some processing on certain event types.
    prot void callTouchListener(
        base::CursorHandlingData* cursorHD, base::CursorHandlingData::ButtonHD* cursorButtonHD,
        TouchEventType* eventType, ArrayList<WrappedTouchListener2D*>* wrappedTouchListenersList, IInteractiveDrawable2D* entry,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& localPos,
        Vector2* deltaPosW,
        int eventIndex
    );

    priv void dispatchTouchBoxChangedIsPossible(bool skipVisibleCheck);

    prot void disposeMain() override;
    pub ~Container2D() override;
};
