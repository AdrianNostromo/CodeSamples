#include "HistoryActionMaterialRemove.h"

using namespace rpg3D;

int HistoryActionMaterialRemove::TYPE = getNewTypeId();

HistoryActionMaterialRemove::HistoryActionMaterialRemove(int editorMatIndex, Color& col)
	: super(TYPE),
	editorMatIndex(editorMatIndex), col(col)
{
	rawSubclassPointer = this;
}

HistoryActionMaterialRemove::~HistoryActionMaterialRemove() {
	//void
}
