#pragma once

#include <base/gh.h>
#include <string>
#include <graphics/visual/Color.h>
#include <base/view/listener/IAutoListener.h>

namespace base {
	class IDialogView;
	class DynamicTextButtonConfig;
};

namespace base {
class IAutoListenerDialogView : virtual public IAutoListener {
	pub virtual void onDynamicButtonActivated(IDialogView* view, DynamicTextButtonConfig* dynamicButtonConfig) = 0;
	pub virtual void onCloseRequested(IDialogView* view) = 0;
	pub virtual void onContentAreaTouched(IDialogView* view) = 0;

    pub virtual ~IAutoListenerDialogView() = default;
};
};
