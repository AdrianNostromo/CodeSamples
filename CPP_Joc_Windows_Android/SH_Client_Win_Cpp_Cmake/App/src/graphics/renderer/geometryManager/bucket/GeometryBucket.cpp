#include "GeometryBucket.h"

using namespace graphics;

GeometryBucket::GeometryBucket(sp<GeometryBucketConfig> config)
	: super(config)
{
	rawSubclassPointer = this;
}

GeometryBucket::~GeometryBucket() {
	//void
}
