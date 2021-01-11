#pragma once

#include <base/gh.h>
#include <base/object/IObject.h>

namespace graphics {
	class NodePart;
	class ShaderProgram;
	class IRenderable;
	class IEnvironment;
	class IAttributesList;
};

namespace graphics {
class IShader : virtual public base::IObject {
	pub enum UniformCountType { None, Single, Array };

	pub virtual bool canRender(
		IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList) = 0;

	pub virtual ShaderProgram* getProgram() = 0;

	pub virtual void begin() = 0;
	pub virtual void end() = 0;

    pub ~IShader() override = default;
};
};
