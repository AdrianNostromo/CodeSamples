#pragma once

#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IGamePartCustomisedContent.h"

namespace rpg3D {
class GPCC_Base : public GamePart, virtual public IGamePartCustomisedContent {priv typedef GamePart super;pub dCtor(GPCC_Base);
    priv rpg3D::ICustomisedContentHandler* customisedContentHandler = nullptr;
    
    pub explicit GPCC_Base(base::IGameWorld* gw);

    pub rpg3D::ICustomisedContentHandler* getCustomisedContentHandler() final;
    pub void setCustomisedContentHandler(rpg3D::ICustomisedContentHandler* customisedContentHandler) final;
    
    pub ~GPCC_Base() override;
};
};
