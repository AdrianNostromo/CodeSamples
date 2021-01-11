#pragma once

#include <base/list/ArrayList.h>
#include <base/exceptions/LogicException.h>
#include <functional>
#include <cassert>

namespace base {
class StaticsInit {
private:
	static bool IsIncludeInitialised;

	static ArrayList<ArrayList<std::function<void()>>>* get_callbacksList2D_General();

public:
	static void InitStatics();

	// Note. Return a bool so it can be called from static code more easily.
	template <typename T>
	static T AddCbGeneral(int initLayerIndex, std::function<void()> cb);

};

template <typename T>
T StaticsInit::AddCbGeneral(int initLayerIndex, std::function<void()> cb) {
	if (!IsIncludeInitialised) {
		throw LogicException(LOC);
	}
	if (get_callbacksList2D_General() == nullptr) {
		throw LogicException(LOC);
	}

	while (get_callbacksList2D_General()->size() <= initLayerIndex) {
		get_callbacksList2D_General()->append_emplace();
	}

	ArrayList<std::function<void()>>* layerList = get_callbacksList2D_General()->getPointer(initLayerIndex);
	layerList->appendReference(cb);

	return 0;
}

};
