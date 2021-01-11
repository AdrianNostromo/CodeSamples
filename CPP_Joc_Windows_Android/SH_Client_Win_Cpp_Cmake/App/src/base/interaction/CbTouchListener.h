#pragma once

#include "ITouchListener.h"
#include <functional>

class CbTouchListener : virtual public ITouchListener {
public:
    std::function<bool(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos)> cb_onCursorDown;

    std::function<void(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos)> cb_onCursorUp;
    std::function<void(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos)> cb_onCursorUpOutside;

    std::function<void(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos)> cb_onCursorTap;

    std::function<void(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos,
		Vector2& deltaPosW)> cb_onCursorDrag;

    std::function<void(
        IInteractiveDrawable2D* target,
        int cursorIndex,
		Vector2& screenPos, Vector2& localPos,
		Vector2& deltaPosW)> cb_onCursorMove;

    std::function<void(
        IInteractiveDrawable2D* target,
        int cursorIndex,
		Vector2& screenPos, Vector2& localPos)> cb_onCursorOver;
    std::function<void(
        IInteractiveDrawable2D* target,
        int cursorIndex,
		Vector2& screenPos, Vector2& localPos)> cb_onCursorOut;

public:
    CbTouchListener(CbTouchListener const&) = delete;
    CbTouchListener(CbTouchListener &&) = default;
    CbTouchListener& operator=(CbTouchListener const&) = delete;
    CbTouchListener& operator=(CbTouchListener &&) = default;

    explicit CbTouchListener();

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

    ~CbTouchListener() override;
};
