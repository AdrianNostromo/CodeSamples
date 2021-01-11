#include "ICRB_Indices.h"

using namespace graphics;

ICRB_Indices::ICRB_Indices()
	: super()
{
	rawSubclassPointer = this;
}

InCombinedRenderableBlock* ICRB_Indices::GetNew() {
	return new ICRB_Indices();;
}

ICRB_Indices::~ICRB_Indices() {
	//void
}
