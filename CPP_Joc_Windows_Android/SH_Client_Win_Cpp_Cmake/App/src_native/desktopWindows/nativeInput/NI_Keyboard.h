#pragma once

#include "NI_Mouse.h"

class NI_Keyboard : public NI_Mouse {
private: typedef NI_Mouse super;
private:
    static NI_Keyboard* globalRef;

public:
    NI_Keyboard(NI_Keyboard const&) = delete;
    NI_Keyboard(NI_Keyboard &&) = default;
    NI_Keyboard& operator=(NI_Keyboard const&) = delete;
    NI_Keyboard& operator=(NI_Keyboard &&) = default;

    explicit NI_Keyboard(GLFWwindow* window);

    ~NI_Keyboard() override;
protected:
    void createMain() override;

private:
    static void global_onKeyEvent(GLFWwindow* window, int keyCode, int deviceSpecificScancode, int actionType, int modifierBits);

    void onKeyEvent(GLFWwindow* window, int keyCode, int deviceSpecificScancode, int actionType, int modifierBits);
};
