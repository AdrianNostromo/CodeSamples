#pragma once

#include <base/gh.h>

namespace base {
namespace snackbar {
    class IInOrderView;
};
};

namespace base {
namespace snackbar {
class ISnackbarViewsInOrderDisplayHandler {
    pub virtual void addInOrderView(IInOrderView* view, bool doAnimation) = 0;
    pub virtual void removeInOrderView(IInOrderView* view) = 0;

    pub virtual ~ISnackbarViewsInOrderDisplayHandler() = default;
};
};
};
