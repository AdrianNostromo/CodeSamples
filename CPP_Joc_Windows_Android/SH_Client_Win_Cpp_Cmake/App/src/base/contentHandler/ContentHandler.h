#pragma once

#include <base/gh.h>
#include "CH_Base.h"

namespace base {
class ContentHandler : public contentHandler::CH_Base {priv typedef contentHandler::CH_Base super;pub dCtor(ContentHandler);
    pub explicit ContentHandler(IApp* app);

    pub ~ContentHandler() override;
};
};
