#include "MDI_Base.h"
#include <base/app/IAppUtils.h>
#include <base/input/util/BlockingInputLayer.h>

using namespace rpg3D;

MDI_Base::MDI_Base(base::IGameWorld* gw)
    : super(gw)
{
    //void
}

void MDI_Base::createMain() {
    super::createMain();

    blockingInputLayer = new base::BlockingInputLayer();

    appUtils->getInputHandler()->addBlockingInputLayer(blockingInputLayer);
}

base::IBlockingInputLayer* MDI_Base::getBlockingInputLayer() {
    return blockingInputLayer;
}

void MDI_Base::clearInput() {
    //void
}

void MDI_Base::disableUnitControlType(int old_unitControlType) {
    //void
}

void MDI_Base::enableUnitControlType(int unitControlType) {
    //void
}

void MDI_Base::disposePre() {
    appUtils->getInputHandler()->removeBlockingInputLayer(blockingInputLayer);
    if(blockingInputLayer != nullptr) {
        delete blockingInputLayer;
        blockingInputLayer = nullptr;
    }

    super::disposePre();
}

MDI_Base::~MDI_Base() {
    //void
}
