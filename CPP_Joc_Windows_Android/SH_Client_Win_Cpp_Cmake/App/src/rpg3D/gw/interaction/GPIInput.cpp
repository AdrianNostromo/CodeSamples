#include "GPIInput.h"

using namespace rpg3D;

GPIInput::GPIInput(base::IGameWorld* gw)
    : super(gw)
{
    //void
}

ManagedDeviceInput* GPIInput::newInput() {
    return new ManagedDeviceInput(gw);
}

GPIInput::~GPIInput() {
    //void
}
