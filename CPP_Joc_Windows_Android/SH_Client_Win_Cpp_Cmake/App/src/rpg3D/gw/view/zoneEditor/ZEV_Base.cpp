#include "ZEV_Base.h"

using namespace rpg3D;

ZEV_Base::ZEV_Base(
	IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
	IAppUtils* appUtils)
	: super(app, viewItemConfigs, viewEaseDurationsSList, nullptr/*handlerAutoDisposal*/),
	appUtils(appUtils)
{
	//void
}

void ZEV_Base::setHandlerAndListener(IZoneEditorM_Edit* entityEditor, IListenerZoneEditorView* listener) {
	this->entityEditor = entityEditor;
	this->listener = listener;
}

base::IBlockingInputLayer* ZEV_Base::getBlockingStreamLayer() {
	return blockingStreamLayer;
}

void ZEV_Base::setBlockingStreamLayer(base::IBlockingInputLayer* blockingStreamLayer) {
	this->blockingStreamLayer = blockingStreamLayer;
}

ZEV_Base::~ZEV_Base() {
	//void
}
