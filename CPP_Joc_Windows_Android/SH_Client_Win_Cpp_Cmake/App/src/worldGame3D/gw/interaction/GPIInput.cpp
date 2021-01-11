#include "GPIInput.h"

using namespace base;

GPIInput::GPIInput(base::IGameWorld* gw)
    : super(gw)
{
    //void
}

void GPIInput::createMain() {
    super::createMain();

    input = newInput();
    input->reservedCreate();
}

void GPIInput::createBMain() {
    super::createBMain();

    input->reservedCreateB();
}

ManagedDeviceInput* GPIInput::newInput() {
    return new ManagedDeviceInput(gw);
}

IManagedDeviceInput* GPIInput::getInput() {
    return input;
}

void GPIInput::disposePre() {
    if(input != nullptr) {
        input->reservedDisposePre();
    }

    super::disposePre();
}

void GPIInput::disposeMain() {
    if(input != nullptr) {
        input->reservedDispose();
        delete input;

        input = nullptr;
    }

    super::disposeMain();
}

GPIInput::~GPIInput() {
    //void
}
