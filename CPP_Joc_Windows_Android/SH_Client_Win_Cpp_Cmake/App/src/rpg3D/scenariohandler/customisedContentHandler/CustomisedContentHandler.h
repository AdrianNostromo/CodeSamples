#pragma once

#include <base/gh.h>
#include "CCH_PlayerUnitPaint.h"

namespace rpg3D {
class CustomisedContentHandler : public CCH_PlayerUnitPaint {priv typedef CCH_PlayerUnitPaint super;pub dCtor(CustomisedContentHandler);
    pub explicit CustomisedContentHandler(IApp* app);

    pub ~CustomisedContentHandler() override;
};
};
