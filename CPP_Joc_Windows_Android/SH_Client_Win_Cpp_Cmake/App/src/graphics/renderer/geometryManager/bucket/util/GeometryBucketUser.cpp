#include "GeometryBucketUser.h"
#include <base/exceptions/LogicException.h>
#include <cassert>

using namespace graphics;

GeometryBucketUser::GeometryBucketUser(sp<GeometryBucketConfig> config)
	: config(config)
{
	//void
}

GeometryBucketUser::~GeometryBucketUser() {
	if (bucket != nullptr) {
		// The bucket should be disconnected before this gets deleted.
		assert(false);
	}
}
