#include "RP_Base.h"
#include <graphics/shader/pool/ShadersPool.h>

using namespace graphics;

RP_Base::RP_Base(int renderOrder, bool useDepthTest)
	: super(),
	renderOrder(renderOrder),
	useDepthTest(useDepthTest)
{
	//void
}

int RP_Base::getRenderOrder() {
	return renderOrder;
}

bool RP_Base::getIsEnabled() {
	return isEnabled;
}

void RP_Base::setIsEnabled(bool isEnabled) {
	if (this->isEnabled == isEnabled) {
		return;
	}

	if (renderer == nullptr) {
		// Enable allowed only if in a renderer.
		throw LogicException(LOC);
	}

	this->isEnabled = isEnabled;

	onIsEnabledChanged();
}

void RP_Base::onIsEnabledChanged() {
	//void
}


IListEntry*& RP_Base::getInHandlerListEntryRef() {
	return inHandlerListEntry;
}

IRenderer* RP_Base::getRenderer() {
	return renderer;
}

void RP_Base::setRenderer(IRenderer* renderer) {
	if ((this->renderer == nullptr && renderer == nullptr)
		|| (this->renderer != nullptr && renderer != nullptr))
	{
		// Renderer change may only occur as a set or clear, no change or re-set.
		throw LogicException(LOC);
	}

	if (getIsEnabled()) {
		// Renderer change cannont occur while the rPhase is enabled.
		throw LogicException(LOC);
	}

	if (this->renderer != nullptr) {
		onRemoveFromRendererPre();

		this->renderer = nullptr;
	}

	if (renderer != nullptr) {
		this->renderer = renderer;

		onAddedToRenderer();
	}
}

void RP_Base::onAddedToRenderer() {
	//void
}

void RP_Base::onRemoveFromRendererPre() {
	//void
}

void RP_Base::dispose() {
	if (shadersPool != nullptr) {
		shadersPool->reservedDispose();
		delete shadersPool;
		shadersPool = nullptr;
	}

	super::dispose();
}

RP_Base::~RP_Base() {
	//void
}
