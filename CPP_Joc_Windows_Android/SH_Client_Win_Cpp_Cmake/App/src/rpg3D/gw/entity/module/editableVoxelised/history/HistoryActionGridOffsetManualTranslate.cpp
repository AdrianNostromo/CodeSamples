#include "HistoryActionGridOffsetManualTranslate.h"

using namespace rpg3D;
int HistoryActionGridOffsetManualTranslate::TYPE = getNewTypeId();

HistoryActionGridOffsetManualTranslate::HistoryActionGridOffsetManualTranslate(Vector3& translate)
	: super(TYPE),
	translate(translate)
{
	rawSubclassPointer = this;
}

HistoryActionGridOffsetManualTranslate::~HistoryActionGridOffsetManualTranslate() {
	//void
}
