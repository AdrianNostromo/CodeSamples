#include "InCombinedRenderableBlock.h"
#include <base/exceptions/LogicException.h>
#include <cassert>

using namespace graphics;

InCombinedRenderableBlock::InCombinedRenderableBlock() {
	//void
}

InCombinedRenderableBlock::~InCombinedRenderableBlock() {
	if (memBlock != nullptr) {
		// This entry is still connected in the cRenderable. A mem leak bug probably exists.
		// If inGMan_toRemove_listEntry removal is used, copy to a external var and make the local var nullptr and then call remove() on the list entry.
		assert(false);
	}
}
