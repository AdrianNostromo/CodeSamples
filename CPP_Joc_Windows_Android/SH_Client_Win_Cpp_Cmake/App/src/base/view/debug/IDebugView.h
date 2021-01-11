#pragma once

#include <base/gh.h>
#include <base/view/IUIView.h>
#include <memory>

namespace base {
	class IDebugEntryUser;
};

namespace base {
class IDebugView : virtual public IUIView {
	pub virtual std::shared_ptr<base::IDebugEntryUser> addDebugEntry(int linesCount = 1, bool hasToggleSwitch = false, int pageIndex = -1, bool hasBtn = false) = 0;

    pub ~IDebugView() override = default;
};
};
