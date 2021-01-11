#pragma once

#include <base/gh.h>
#include <string>
#include <memory>
#include <base/math/Vector3.h>

namespace base {
class GWEventDataBase {pub dCtor(GWEventDataBase);
	pub std::string paramS;
	pub std::shared_ptr<std::string> paramSPS;
	// This is the unit offset from the teleporter entity position.
	/// This is used to avoid teleporter jumping error (it is visually distacting).
	pub Vector3 sourceTeleporterOffset{};

	pub explicit GWEventDataBase();

    pub virtual ~GWEventDataBase();
};
};
