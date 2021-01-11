#pragma once

#include <base/gh.h>
#include "IShader.h"
#include <base/object/Object.h>
#include <string>

namespace graphics {
	class ShaderProgram;
	class VertexAttributesList;
	class Material;
};

namespace graphics {
class Shader : public base::Object, public virtual IShader {priv typedef base::Object super;pub dCtor(Shader);
	// worldTransformIndex, diffuseTextureIndex;
	priv long vertexAttributesMaskAll = 0;
	// diffuseTexture, diffuseTexturesArray
	priv long materialAttributesMaskAll = 0;

	prot UniformCountType worldTransformType;
	prot int maxWorldTransformArrayCount;
	prot int remapingModelTransformsArrayCount;

	prot UniformCountType diffuseTextureType;

	prot ShaderProgram* shaderProgram = nullptr;

	pub explicit Shader();
	pub void init(
		IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList,
		std::string& baseCode_vs, std::string& baseCode_fs);

	// This is needed because shaders have a hard limit on the number of uniform vectors (mat3 and ma4 are made of vectors) allowed in a shader.
	/// This was used for direct shader transforms array. The new UBO transform arrays don't have a limit.
	/// This should not be used anymore, use a UBO instead for large datas.
	// For UBO maxWorldTransformArrayCount use the below replacement function.
	pub static int ComputeMaxModelMat4AndNormalMat3TransformsCount();

	prot virtual std::string generatePrefix();

	pub bool canRender(
		IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList) override;

	pub ShaderProgram* getProgram() final;

	pub void begin() final;
	pub void end() final;

	// Extendable for overriding the value (eg. shadow maps don't use either).
	priv virtual UniformCountType getPrefferredWorldTransformType(IRenderable* renderable, VertexAttributesList* vertexAttributesList, Material* material);
	priv virtual UniformCountType getPrefferredDiffuseTextureType(IRenderable* renderable, VertexAttributesList* vertexAttributesList, Material* material);

    prot void dispose() override;
    pub ~Shader() override;
};
};
