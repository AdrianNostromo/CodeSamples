#include "ISnackbarView.h"
#include <base/view/event/ViewEvent.h>

using namespace base;
using namespace base::snackbar;

ViewEvent* ISnackbarView::ViewEvent_ContentAreaTouched = new ViewEvent("ViewEvent_ContentAreaTouched");
ViewEvent* ISnackbarView::ViewEvent_DynamicButtonActivated = new ViewEvent("ViewEvent_DynamicButtonActivated");
