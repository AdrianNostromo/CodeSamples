#pragma once

#include <base/gh.h>


namespace base {
namespace snackbar {
	class ISnackbarView;
};
};

namespace base {
namespace snackbar {
class WrappedSnackbarView {pub dCtor(WrappedSnackbarView);
	// This is used to get a pointer to this entity that "knows" if the entity exists or was deleted or stuff.
	pub ISnackbarView* view;

	pub explicit WrappedSnackbarView(ISnackbarView* view);

	pub virtual ~WrappedSnackbarView();
};
};
};
