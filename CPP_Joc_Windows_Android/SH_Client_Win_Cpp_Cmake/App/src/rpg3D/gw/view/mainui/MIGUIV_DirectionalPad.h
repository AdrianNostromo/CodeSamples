#pragma once

#include <graphics/visual2d/drawable/IInteractiveDrawable2D.h>
#include <base/interaction/CbTouchListener.h>
#include "MIGUIV_Listener.h"

namespace rpg3D {
class MIGUIV_DirectionalPad : public MIGUIV_Listener {priv typedef MIGUIV_Listener super;pub dCtor(MIGUIV_DirectionalPad);
	prot static MenuItemConfig* new_viewItemConfig_dPad();

	priv IInteractiveDrawable2D* dp_taBg;
	priv IDrawableNode2D* dp_knobHolder;

	// This is used in case a drag is instantly ended the same frame. This ensures it is processed.
	priv bool isTouchDownPulse = false;
	priv bool isDragActive = false;
	priv int currentDragCursorIndex = -1;
	priv base::Touch::ButtonCode* currentDragButtonId = nullptr;
	priv bool isDragEndProcessed = true;
	priv Vector2 lastDragPos{0.0f, 0.0f};
	priv Vector2 currentDragLocalPos{0.0f, 0.0f};
	priv Vector2 currentDragWorldPos{0.0f, 0.0f};

	priv Vector2 v1{};
	priv Vector2 lastMoveDirVector{};

	priv CbTouchListener dPadTouchListener{};

	priv IListenerDPad* listenerDPad = nullptr;

	pub explicit MIGUIV_DirectionalPad(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw);

	prot void createPost() override;

	prot void onSelectedStateChanged() override;

	prot void cancelActiveDrag();

	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	pub void setListenerDPad(IListenerDPad* listenerDPad) final;

	pub void onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) override;

	priv bool dPad_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);
	priv void dPad_onCursorDrag(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos,
		Vector2& deltaPosW);
	priv void dPad_onCursorUp(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);
	priv void dPad_onCursorUpOutside(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	pub ~MIGUIV_DirectionalPad() override;
};
};
