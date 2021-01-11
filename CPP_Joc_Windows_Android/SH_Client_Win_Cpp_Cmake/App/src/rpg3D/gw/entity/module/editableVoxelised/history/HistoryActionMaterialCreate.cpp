#include "HistoryActionMaterialCreate.h"

using namespace rpg3D;

int HistoryActionMaterialCreate::TYPE = getNewTypeId();

HistoryActionMaterialCreate::HistoryActionMaterialCreate(int editorMatIndex, Color& diffuseColor)
	: super(TYPE),
	editorMatIndex(editorMatIndex), diffuseColor(diffuseColor)
{
	rawSubclassPointer = this;
}

HistoryActionMaterialCreate::~HistoryActionMaterialCreate() {
	//void
}
