#include "StaticsInit.h"

using namespace base;

bool StaticsInit::IsIncludeInitialised = true;

ArrayList<ArrayList<std::function<void()>>>* StaticsInit::get_callbacksList2D_General() {
	static ArrayList<ArrayList<std::function<void()>>>* callbacksList2D_General = new ArrayList<ArrayList<std::function<void()>>>();

	return callbacksList2D_General;
}

void StaticsInit::InitStatics() {
	ArrayList<ArrayList<std::function<void()>>>* callbacksList2D_General = get_callbacksList2D_General();

	for(int i=0;i< callbacksList2D_General->count();i++) {
		ArrayList<std::function<void()>>* layerList = callbacksList2D_General->getPointer(i);

		for(int j=0;j<layerList->size();j++) {
			std::function<void()> cb = layerList->getDirect(j);

			cb();
		}

		layerList->clear();
	}

	callbacksList2D_General->clear();
}
