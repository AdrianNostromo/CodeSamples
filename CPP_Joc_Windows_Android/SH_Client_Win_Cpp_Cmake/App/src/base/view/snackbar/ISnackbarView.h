#pragma once

#include <base/gh.h>
#include <base/menu/IAnimatedComposedStiledVisual.h>
#include <string>
#include <memory>
#include <graphics\visual\Color.h>
#include <base/list/ArrayList.h>
#include <base/view/IUIView.h>
#include "util/DynamicTextButtonConfig.h"
#include <base/sp.h>

class ViewEvent;
namespace base {
namespace snackbar {
	class WrappedSnackbarView;
	class Config;
};
};

namespace base {
namespace snackbar {
class ISnackbarView : virtual public IUIView {priv typedef IUIView super;
	pub static ViewEvent* ViewEvent_ContentAreaTouched;
	pub static ViewEvent* ViewEvent_DynamicButtonActivated;

	pub virtual void showInOrderManaged(
		bool doAnimation,
		std::shared_ptr<Config> config) = 0;
	
	pub virtual sp<WrappedSnackbarView> getWrapper() = 0;

	pub virtual ~ISnackbarView() = default;
};
};
};
