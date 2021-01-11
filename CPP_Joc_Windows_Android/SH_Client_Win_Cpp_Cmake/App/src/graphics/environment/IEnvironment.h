#pragma once

#include <base/gh.h>
#include <graphics/attribute/IAttributesList.h>

class Camera;

namespace graphics {
class IEnvironment : public virtual IAttributesList {
	pub virtual void syncIfNeeded() = 0;
	
	pub virtual Camera* peekCamera() = 0;

    pub virtual ~IEnvironment() = default;
};
};
