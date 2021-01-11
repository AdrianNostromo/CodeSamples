#pragma once

#include <base/gh.h>
#include "IAutoListenerDialogView.h"
#include <base/view/listener/AutoListener.h>
#include <functional>

namespace base {
class CbAutoListenerDialogView : public AutoListener, virtual public IAutoListenerDialogView {pub dCtor(CbAutoListenerDialogView);
	pub std::function<void(
		IDialogView * view, DynamicTextButtonConfig * dynamicButtonConfig)> cb_onDynamicButtonActivated;

	pub std::function<void(
		IDialogView * view)> cb_onCloseRequested;

	pub std::function<void(
		IDialogView * view)> cb_onContentAreaTouched;

	pub explicit CbAutoListenerDialogView();

	pub void onDynamicButtonActivated(IDialogView* view, DynamicTextButtonConfig* dynamicButtonConfig) override;
	pub void onCloseRequested(IDialogView* view) override;
	pub void onContentAreaTouched(IDialogView* view) override;

    pub ~CbAutoListenerDialogView() override;
};
};
