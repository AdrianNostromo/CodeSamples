#include "ICRB_Vertices.h"

using namespace graphics;

ICRB_Vertices::ICRB_Vertices()
	: super()
{
	rawSubclassPointer = this;
}

InCombinedRenderableBlock* ICRB_Vertices::GetNew() {
	return new ICRB_Vertices();;
}

ICRB_Vertices::~ICRB_Vertices() {
	//void
}
