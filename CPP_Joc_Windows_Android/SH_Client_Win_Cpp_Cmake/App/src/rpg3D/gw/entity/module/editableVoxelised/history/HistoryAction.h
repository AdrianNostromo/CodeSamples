#pragma once

#include <base/gh.h>
#include <base/exceptions/LogicException.h>

namespace rpg3D {
class HistoryAction {
public:
	prot static int getNewTypeId() {
		static int typeIdIncrementor = 0;

		typeIdIncrementor++;
		return typeIdIncrementor;
	}

	pub int type;

	// This is used to avoid dynamic cast.
	// It contains the subclass void*;
	prot void* rawSubclassPointer = nullptr;

   pub dCtor(HistoryAction);
    pub explicit HistoryAction(int type);

	template <typename T>
	T castAs(int validationType);

    pub virtual ~HistoryAction();
};

template <typename T>
T HistoryAction::castAs(int validationType) {
	if (this->type != validationType || rawSubclassPointer == nullptr) {
		throw LogicException(LOC);
	}

	T cVal = static_cast<T>(rawSubclassPointer);
	
	return cVal;
}

};
