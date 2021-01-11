#pragma once

#include <base/gh.h>
#include "GPLCC_DummyStyle.h"

namespace startarea_a_v1 {
class GamePartLocalCacheContent : public GPLCC_DummyStyle {priv typedef GPLCC_DummyStyle super;pub dCtor(GamePartLocalCacheContent);
	pub explicit GamePartLocalCacheContent(base::IGameWorld* gw);

    pub ~GamePartLocalCacheContent() override;
};
};
