#include "HistoryAction_stackable_haulOffsetZM.h"

using namespace rpg3D;
int HistoryAction_stackable_haulOffsetZM::TYPE = getNewTypeId();

HistoryAction_stackable_haulOffsetZM::HistoryAction_stackable_haulOffsetZM(
	float new_haulOffsetZM, float old_haulOffsetZM)
	: super(TYPE),
	new_haulOffsetZM(new_haulOffsetZM), old_haulOffsetZM(old_haulOffsetZM)
{
	rawSubclassPointer = this;
}

HistoryAction_stackable_haulOffsetZM::~HistoryAction_stackable_haulOffsetZM() {
	//void
}
