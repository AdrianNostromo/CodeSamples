#include "HistoryActionMaterialChange.h"

using namespace rpg3D;

int HistoryActionMaterialChange::TYPE = getNewTypeId();

HistoryActionMaterialChange::HistoryActionMaterialChange(int editorMatIndex, Color& oldMatColor, Color& newMatColor)
	: super(TYPE),
	editorMatIndex(editorMatIndex), oldMatColor(oldMatColor), newMatColor(newMatColor)
{
	rawSubclassPointer = this;
}

HistoryActionMaterialChange::~HistoryActionMaterialChange() {
	//void
}
