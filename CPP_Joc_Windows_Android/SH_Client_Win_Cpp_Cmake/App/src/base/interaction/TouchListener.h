#pragma once

#include "ITouchListener.h"

class TouchListener : virtual public ITouchListener {
public:
	TouchListener();

	bool onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos) override;

	void onCursorUp(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos) override;

	void onCursorUpOutside(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos) override;

	void onCursorTap(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos) override;

	void onCursorDrag(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos,
		Vector2& deltaPosW) override;

	void onCursorMove(
		IInteractiveDrawable2D* target,
		int cursorIndex,
		Vector2& screenPos, Vector2& localPos,
		Vector2& deltaPosW) override;

	void onCursorOver(
		IInteractiveDrawable2D* target,
		int cursorIndex,
		Vector2& screenPos, Vector2& localPos) override;
	void onCursorOut(
		IInteractiveDrawable2D* target,
		int cursorIndex,
		Vector2& screenPos, Vector2& localPos) override;

	~TouchListener() override;
};
