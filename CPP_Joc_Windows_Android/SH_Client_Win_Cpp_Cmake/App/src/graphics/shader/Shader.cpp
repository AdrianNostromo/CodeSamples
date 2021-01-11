#include "Shader.h"
#include <graphics/shader/program/ShaderProgram.h>
#include <base/util/DeviceInfo.h>
#include <sstream>
#include <graphics/geometry/data/MeshPart.h>
#include <graphics/geometry/Mesh.h>
#include <graphics/model/NodePart.h>
#include <graphics/model/Node.h>
#include <graphics/geometry/data/VertexBufferObject.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/geometry/vertexAttribute/VertexAttribute.h>
#include <graphics/material/attributeTypes/MADiffuseTexture.h>
#include <graphics/material/attributeTypes/MADiffuseTexturesArray.h>
#include <graphics/material/Material.h>
#include <base/math/Math.h>
#include <graphics/renderer/renderPhase/util/IRenderable.h>
#include <graphics/geometry/data/UniformBufferObject.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_ModelTransforms.h>
#include <graphics/visual/TextureArray.h>

using namespace graphics;

Shader::Shader()
	: super()
{
	//void
}

void Shader::init(
	IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList,
	std::string& baseCode_vs, std::string& baseCode_fs)
{

	// This allows for basic shaders that can be used on the fly.
	if (renderable != nullptr && environment != nullptr) {
		VertexAttributesList* vertexAttributesList = renderable != nullptr ? renderable->getGeometryProvider_Vertices()->getVertexAttributesList().get() : nullptr;
		std::shared_ptr<graphics::Material> material = renderable != nullptr ? renderable->getGeometryProvider_Vertices()->getMaterial() : nullptr;
		
		worldTransformType = getPrefferredWorldTransformType(renderable, vertexAttributesList, material.get());
		diffuseTextureType = getPrefferredDiffuseTextureType(renderable, vertexAttributesList, material.get());
	} else {
		worldTransformType = UniformCountType::None;
		diffuseTextureType = UniformCountType::None;
	}

	vertexAttributesMaskAll = 0;
	materialAttributesMaskAll = 0;

	if (worldTransformType == Array) {
		vertexAttributesMaskAll |= VertexAttributesList::Type::ModelAndNormalTransformIndex->typeBitGroup;

		maxWorldTransformArrayCount = renderable->getGeometryProvider_ModelTransforms()->getModelTransformsUBO()->getBuffer().capacity();
		UniformBufferObject* remapingModelTransformsIndicesUBO = renderable->getGeometryProvider_ModelTransforms()->getRemapingModelTransformsIndicesUBOOptional();
		if (remapingModelTransformsIndicesUBO != nullptr) {
			remapingModelTransformsArrayCount = remapingModelTransformsIndicesUBO->getBuffer().capacity();
		} else {
			remapingModelTransformsArrayCount = -1;
		}
	} else if (worldTransformType == Single) {
		maxWorldTransformArrayCount = -1;
		remapingModelTransformsArrayCount = -1;
	} else {
		maxWorldTransformArrayCount = -1;
		remapingModelTransformsArrayCount = -1;
	}

	if (diffuseTextureType == Array) {
		// Array;
		vertexAttributesMaskAll |= VertexAttributesList::Type::DiffuseTextureIndex->typeBitGroup;
	} else if (diffuseTextureType == Single) {
		// Single.
		//void
	} else {
		//void
	}

	std::string sourcePrefix = generatePrefix();
	std::string bakedVSSource = sourcePrefix + baseCode_vs;
	std::string bakedFSSource = sourcePrefix + baseCode_fs;

	shaderProgram = new ShaderProgram(
		bakedVSSource, bakedFSSource
	);
	shaderProgram->reservedCreate();
}

std::string Shader::generatePrefix() {
	std::stringstream sourcePrefixSS;

	// Versions: https://en.wikipedia.org/wiki/OpenGL_Shading_Language
	if(DeviceInfo::deviceType == DeviceInfo::Type::DESKTOP_LINUX || DeviceInfo::deviceType == DeviceInfo::Type::DESKTOP_WINDOWS) {
		//sourcePrefixSS << "#version 130\n";// OpenGL 3.0
		sourcePrefixSS << "#version 140\n";// OpenGL 3.1
	}else if(DeviceInfo::deviceType == DeviceInfo::Type::MOBILE_ANDROID) {
		sourcePrefixSS << "#version 300 es\n";// OpenGL ES 3.0; (Limited version of OpenGL 3.3);
	}else {
		throw LogicException(LOC);
	}

	if (worldTransformType == Array) {
		sourcePrefixSS << "#define worldTransform_indexedArrayFlag\n";
		
		// Use the max value to avoid many shaders creation and shader switches if they are checked in the wrong order.
		sourcePrefixSS << "#define indexedWorldTransformsCount " << maxWorldTransformArrayCount << "\n";

		if (remapingModelTransformsArrayCount >= 0) {
			sourcePrefixSS << "#define remapingModelTransformsArrayFlag\n";
			sourcePrefixSS << "#define remapingModelTransformsArrayCount " << remapingModelTransformsArrayCount << "\n";
		}
	} else if (worldTransformType == Single) {
		sourcePrefixSS << "#define worldTransform_singleFlag\n";
	} else {
		//void
	}

	if (diffuseTextureType == Array) {
		sourcePrefixSS << "#define diffuseTexture_indexedArrayFlag\n";
		sourcePrefixSS << "#define TEXTURE_ARRAY_CAPACITY " << graphics::TextureArray::MAX_TEXTURES_COUNT << "\n";
		sourcePrefixSS << "#define TEXTURE_ARRAYS_COUNT " << graphics::TextureArray::GLOBAL_TEXTURE_ARRAYS_COUNT << "\n";
	} else if (diffuseTextureType == Single) {
		sourcePrefixSS << "#define diffuseTexture_singleFlag\n";
	} else {
		//void
	}

	return sourcePrefixSS.str();
}

bool Shader::canRender(
	IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList)
{
	VertexAttributesList* vertexAttributesList = renderable->getGeometryProvider_Vertices()->getVertexAttributesList().get();
	std::shared_ptr<graphics::Material> material = renderable->getGeometryProvider_Vertices()->getMaterial();

	// Check vertexAttributesMaskAll, materialAttributesMaskAll.
	if (!vertexAttributesList->hasAll(vertexAttributesMaskAll) || !material->hasAll(materialAttributesMaskAll)) {
		return false;
	}
	
	UniformCountType o_worldTransformType = getPrefferredWorldTransformType(renderable, vertexAttributesList, material.get());
	UniformCountType o_diffuseTextureType = getPrefferredDiffuseTextureType(renderable, vertexAttributesList, material.get());

	if (worldTransformType != o_worldTransformType || diffuseTextureType != o_diffuseTextureType) {
		return false;
	}

	// Also check if enough slots exists for world_transforms diffuse_textures slots array sizes if used.
	// Check if more slots required than available.
	if (worldTransformType == UniformCountType::Array) {
		UniformBufferObject* modelTransformsUBO = renderable->getGeometryProvider_ModelTransforms()->getModelTransformsUBO();
		if (modelTransformsUBO == nullptr || modelTransformsUBO->getBuffer().count() > maxWorldTransformArrayCount) {
			throw LogicException(LOC);
		}
	}

	return true;
}

IShader::UniformCountType Shader::getPrefferredWorldTransformType(IRenderable* renderable, VertexAttributesList* vertexAttributesList, Material* material) {
	if (vertexAttributesList->hasAll(VertexAttributesList::Type::ModelAndNormalTransformIndex->typeBitGroup) && renderable->getGeometryProvider_ModelTransforms()->getModelTransformsUBO() != nullptr) {
		return UniformCountType::Array;
	} else if (renderable->getGeometryProvider_ModelTransforms()->getWorldTransformOptional() != nullptr) {
		return UniformCountType::Single;
	}
	
	return UniformCountType::None;
}

IShader::UniformCountType Shader::getPrefferredDiffuseTextureType(IRenderable* renderable, VertexAttributesList* vertexAttributesList, Material* material) {
	bool hasVATexCoords = vertexAttributesList->hasAll(VertexAttributesList::Type::TexCoord0->typeBitGroup);

	if (hasVATexCoords && vertexAttributesList->hasAll(VertexAttributesList::Type::DiffuseTextureIndex->typeBitGroup) && material->hasAll(MADiffuseTexturesArray::TypeBitGroup)) {
		return UniformCountType::Array;
	} else if (hasVATexCoords && material->hasAll(MADiffuseTexture::TypeBitGroup)) {
		return UniformCountType::Single;
	}
	
	return UniformCountType::None;
}

void Shader::dispose() {
	if (shaderProgram != nullptr) {
		shaderProgram->reservedDispose();
		delete shaderProgram;
		shaderProgram = nullptr;
	}

	super::dispose();
}

ShaderProgram* Shader::getProgram() {
	return shaderProgram;
}

void Shader::begin() {
	//asd_r;// implement more like libgdx does.

	shaderProgram->bind();
}

void Shader::end() {
	//asd_r;// implement more like libgdx does.
}

Shader::~Shader() {
	//void
}
