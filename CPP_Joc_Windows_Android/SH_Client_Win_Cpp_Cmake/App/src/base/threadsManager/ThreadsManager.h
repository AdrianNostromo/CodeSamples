#pragma once

#include <base/gh.h>
#include "TM_ThreadsList.h"

namespace base {
class ThreadsManager : public TM_ThreadsList {priv typedef TM_ThreadsList super;pub dCtor(ThreadsManager);
    pub explicit ThreadsManager();

    pub ~ThreadsManager() override;
};
};
