#include "RenderPhase.h"

using namespace graphics;

RenderPhase::RenderPhase(
	int renderOrder, bool useDepthTest,
	base::IScreen* screen,
	long supportedAutomaticAttributesBitMask, long ignorredAutomaticAttributesBitMask)
	: super(
		renderOrder, useDepthTest,
		screen,
		supportedAutomaticAttributesBitMask, ignorredAutomaticAttributesBitMask)
{
	//void
}

RenderPhase::~RenderPhase() {
	//void
}
