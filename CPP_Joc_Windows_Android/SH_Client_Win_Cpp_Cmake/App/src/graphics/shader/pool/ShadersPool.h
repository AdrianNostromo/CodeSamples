#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include <base/object/Object.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <graphics/shader/IShader.h>

namespace graphics {
	class IRenderable;
	class IEnvironment;
};

namespace graphics {
class ShadersPool : public base::Object {priv typedef base::Object super; pub dCtor(ShadersPool);
	priv ArrayList<IShader*> shadersList;

	pub explicit ShadersPool();

	pub IShader* getOrCreateShader(
		IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList);
	
	prot virtual IShader* createNewShader(
		IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList) = 0;

	prot void dispose() override;
	pub ~ShadersPool() override;

};
};
