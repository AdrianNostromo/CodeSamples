#pragma once

#include "NI_Base.h"
#include <base/input/util/Touch.h>
#include <base/math/Vector2.h>
#include <base/list/ArrayList.h>

class NI_Mouse : public NI_Base {
private: typedef NI_Base super;
private:
	class CursorHandlingData {
	public:
	class ButtonHD {
	public:
		CursorHandlingData* cursorHandlingData;

		base::Touch::ButtonCode* buttonId;

		bool isPressed = false;

	public:
		ButtonHD(ButtonHD const&) = delete;
		ButtonHD(ButtonHD &&) = default;
		ButtonHD& operator=(ButtonHD const&) = delete;
		ButtonHD& operator=(ButtonHD &&) = default;

		explicit ButtonHD(CursorHandlingData* cursorHandlingData, base::Touch::ButtonCode* buttonId);

		virtual ~ButtonHD();
	};
	public:
		int cursorIndex;

		// This is updated always. For touch, it should be updated on all events that contain a pos.
		Vector2 pos{};

		ArrayList<std::shared_ptr<ButtonHD>> buttonsHandlingDataList{};

		CursorHandlingData(CursorHandlingData const&) = delete;
		CursorHandlingData(CursorHandlingData &&) = default;
		CursorHandlingData& operator=(CursorHandlingData const&) = delete;
		CursorHandlingData& operator=(CursorHandlingData &&) = default;

		explicit CursorHandlingData();

		ButtonHD* getButtonHandlingData(base::Touch::ButtonCode* buttonId);

		virtual ~CursorHandlingData();
	};
private:
	static const int MOUSE_CURSOR_INDEX;
	
    static NI_Mouse* globalRef;

    // This affects only the first cursor.
	base::Touch::CursorMode* cursorMode = base::Touch::CursorMode::Normal;

	ArrayList<std::shared_ptr<CursorHandlingData>> cursorHandlingDataList{};

public:
    NI_Mouse(NI_Mouse const&) = delete;
    NI_Mouse(NI_Mouse &&) = default;
    NI_Mouse& operator=(NI_Mouse const&) = delete;
    NI_Mouse& operator=(NI_Mouse &&) = default;

    explicit NI_Mouse(GLFWwindow* window);

	base::Touch::CursorMode* getCursorMode() override;
	bool getIsCursorModeSupported(base::Touch::CursorMode* cursorMode) override;
    void setCursorMode(base::Touch::CursorMode* cursorMode) override;

    ~NI_Mouse() override;
protected:
    void createMain() override;

private:
    static void global_onMouseMoveEvent(GLFWwindow* window, double xpos, double ypos);
    static void global_onMouseButtonEvent(GLFWwindow* window, int nativeButtonId, int action, int mods);

    void onMouseMoveEvent(GLFWwindow* window, double xpos, double ypos);
    void onMouseButtonEvent(GLFWwindow* window, base::Touch::ButtonCode* _buttonId, int action, int mods);

	CursorHandlingData* getCursorHandlingData(int cursorIndex);

};
