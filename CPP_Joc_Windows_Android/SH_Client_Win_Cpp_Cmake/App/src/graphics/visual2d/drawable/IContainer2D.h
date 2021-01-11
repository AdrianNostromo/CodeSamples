#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/IDrawableNode2D.h>
#include <graphics/visual2d/util/CursorHandlingData.h>
#include <base/input/util/TouchEventType.h>

class IContainer2D : virtual public IDrawableNode2D {
	pub virtual void addChild(IDrawableNode2D* drawable) = 0;
	pub virtual void removeChild(IDrawableNode2D* drawable) = 0;

	pub virtual IDrawableNode2D* onChildAddingToRendererAndGetPrevDrawableWithRenderData(IDrawableNode2D* leafDrawableOrFirstLevelLocalChild) = 0;
	pub virtual void onChildRemovedFromRenderer(IDrawableNode2D* drawable) = 0;

	pub virtual bool getIsInputInteractive() = 0;
	pub virtual void onChildIsInputInteractiveChanged(IDrawableNode2D* drawable, bool childIsInputInteractive) = 0;

	pub virtual void onTouchBoxChanged() = 0;
	pub virtual void clearIsTouchBoxChanged() = 0;

	pub virtual void childSyncDrawSpecsRequested() = 0;

	pub virtual bool worldTouchEvent(
		TouchEventType* eventType,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2* deltaPosW,
		base::CursorHandlingData* cursorHD, base::CursorHandlingData::ButtonHD* cursorButtonHD,
		int eventIndex) = 0;

	pub ~IContainer2D() override = default;
};
