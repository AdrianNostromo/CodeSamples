#include "ICRB_ModelTransforms.h"
#include <base/exceptions/LogicException.h>
#include <cassert>

using namespace graphics;

ICRB_ModelTransforms::ICRB_ModelTransforms()
	: super()
{
	rawSubclassPointer = this;
}

InCombinedRenderableBlock* ICRB_ModelTransforms::GetNew() {
	return new ICRB_ModelTransforms();;
}

ICRB_ModelTransforms::~ICRB_ModelTransforms() {
	if (memBlock_transformsIndexRemaping != nullptr) {
		// This entry is still connected in the cRenderable. A mem leak bug probably exists.
		assert(false);
	}
	if (inGBucket_toRemoveUnused_listEntry != nullptr) {
		// This entry is still connected in the cRenderable. A mem leak bug probably exists.
		// If inGMan_toRemove_listEntry removal is used, copy to a external var and make the local var nullptr and then call remove() on the list entry.
		assert(false);
	}
}
