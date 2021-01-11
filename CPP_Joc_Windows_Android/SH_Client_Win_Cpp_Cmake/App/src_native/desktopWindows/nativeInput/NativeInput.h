#pragma once

#include "NI_Keyboard.h"

class NativeInput : public NI_Keyboard {
private: typedef NI_Keyboard super;
public:
    NativeInput(NativeInput const&) = delete;
    NativeInput(NativeInput &&) = default;
    NativeInput& operator=(NativeInput const&) = delete;
    NativeInput& operator=(NativeInput &&) = default;

    explicit NativeInput(GLFWwindow* window);

    ~NativeInput() override;

};
