#pragma once

#include <GLFW/glfw3.h>
#include <base/nativeInput/NativeInput.h>

class NI_Base : public base::NativeInput {
public:
    GLFWwindow* window;

protected:
    IInputProcessor* rawInputProcessor = nullptr;

public:
    NI_Base(NI_Base const&) = delete;
    NI_Base(NI_Base &&) = default;
    NI_Base& operator=(NI_Base const&) = delete;
    NI_Base& operator=(NI_Base &&) = default;

    explicit NI_Base(GLFWwindow* window);
    void reservedCreate();

    IInputProcessor* getInputProcessor() override;
    void setInputProcessor(IInputProcessor* rawInputProcessor) override;

    ~NI_Base() override;
protected:
    virtual void createMain();

};
