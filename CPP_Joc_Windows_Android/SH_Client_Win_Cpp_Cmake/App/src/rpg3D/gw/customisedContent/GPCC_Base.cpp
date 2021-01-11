#include "GPCC_Base.h"

using namespace rpg3D;

GPCC_Base::GPCC_Base(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

rpg3D::ICustomisedContentHandler* GPCC_Base::getCustomisedContentHandler() {
    if (customisedContentHandler == nullptr) {
        throw LogicException(LOC);
    }
    return customisedContentHandler;
}

void GPCC_Base::setCustomisedContentHandler(rpg3D::ICustomisedContentHandler* customisedContentHandler) {
    this->customisedContentHandler = customisedContentHandler;
}

GPCC_Base::~GPCC_Base() {
	//void
}
