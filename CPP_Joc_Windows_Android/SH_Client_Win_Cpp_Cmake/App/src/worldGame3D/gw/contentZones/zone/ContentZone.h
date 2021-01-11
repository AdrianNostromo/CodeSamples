#pragma once

#include <base/gh.h>
#include "CZ_Base.h"

namespace worldGame3D {
class ContentZone : public CZ_Base {priv typedef CZ_Base super;pub dCtor(ContentZone);
    pub explicit ContentZone(std::string& id);

	pub ~ContentZone() override;
};
};
