#pragma once

#include <base/gh.h>
#include <string>
#include <memory>
#include <base/exceptions/LogicException.h>
#include <base/math/Vector3.h>
#include <base/sp.h>

namespace worldGame3D {
class ZoneStartExtraData {pub dCtor(ZoneStartExtraData);
	// Both or one or none of these 2 can be provided.
	pub std::shared_ptr<std::string> teleportTargetEntityInstanceID;
	pub sp<Vector3> teleportTargetOffset;

    pub explicit ZoneStartExtraData(std::shared_ptr<std::string> teleportTargetEntityInstanceID, sp<Vector3> teleportTargetOffset);

	pub template <typename T>
	T getAs();
	
	pub template <typename T>
	T getAsOptional();

    pub virtual ~ZoneStartExtraData();
};

template <typename T>
T ZoneStartExtraData::getAs() {
	T cVal = dynamic_cast<T>(this);
	if (cVal == nullptr) {
		throw LogicException(LOC);
	}

	return cVal;
}

template <typename T>
T ZoneStartExtraData::getAsOptional() {
	T cVal = dynamic_cast<T>(this);
	
	return cVal;
}

};
