#pragma once

#include <base/gh.h>
#include "SVIODH_Views.h"

namespace base {
namespace snackbar {
class SnackbarViewsInOrderDisplayHandler : public SVIODH_Views {priv typedef SVIODH_Views super;pub dCtor(SnackbarViewsInOrderDisplayHandler);
    pub explicit SnackbarViewsInOrderDisplayHandler(IApp* app);

    pub ~SnackbarViewsInOrderDisplayHandler() override;
};
};
};
