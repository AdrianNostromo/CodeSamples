#pragma once

#include <base/gh.h>
#include <base/menu/IAnimatedComposedStiledVisual.h>
#include <memory>

class IListEntry;

namespace base {
namespace snackbar {
class IInOrderView {
	pub virtual bool getIsViewShowing() = 0;

	pub virtual void showView(
		bool doAnimation) = 0;

	pub virtual IListEntry*& getInDisplayOrder_listEntryRef() = 0;


    pub virtual ~IInOrderView()  = default;
};
};
};
