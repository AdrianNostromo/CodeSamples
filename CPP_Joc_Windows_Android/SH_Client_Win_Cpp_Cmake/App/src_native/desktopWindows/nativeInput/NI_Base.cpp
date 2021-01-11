#include "NI_Base.h"

NI_Base::NI_Base(GLFWwindow* window)
    : window(window)
{
    //void
}

void NI_Base::reservedCreate() {
    createMain();
}

void NI_Base::createMain() {
    //void

}

IInputProcessor* NI_Base::getInputProcessor() {
    return rawInputProcessor;
}

void NI_Base::setInputProcessor(IInputProcessor* rawInputProcessor) {
    this->rawInputProcessor = rawInputProcessor;
}

NI_Base::~NI_Base() {
    //void
}
