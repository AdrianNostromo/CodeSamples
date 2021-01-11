#pragma once

#include <base/gh.h>

namespace mem {
class ISelfSharedPointerContainer {
	// This class is used with shared pointers so initialise a weak pointer inside the object instance.
	// To use this class, extend from it and create a function like the one below. The sp gets automatically static_cast to the selfSP type;
	//priv sp<Foo> selfWSP{ true/*isWeakPointer*/ };
	//pub sp<Foo>& getSelfWSPRef() { return selfWSP; }

	pub virtual ~ISelfSharedPointerContainer() = default;
};
};
