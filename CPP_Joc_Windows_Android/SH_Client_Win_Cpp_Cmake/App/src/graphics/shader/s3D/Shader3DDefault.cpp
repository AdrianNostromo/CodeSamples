#include "Shader3DDefault.h"
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>
#include <memory>
#include <base/fileSystem/Util_FileSystem.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <graphics/material/attributeTypes/MADirectionalLightsList.h>
#include <graphics/material/attributeTypes/MAPointLightsList.h>
#include <graphics/material/attributeTypes/MASpotLightsList.h>
#include <graphics/material/attributeTypes/MABool.h>
#include <graphics/environment/IEnvironment.h>
#include <graphics/renderer/renderPhase/util/IRenderable.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/geometry/vertexAttribute/VertexAttribute.h>
#include <graphics/renderer/renderPhase/rp3DShadowMap/rendererAttributes/RABool.h>
#include <graphics/attribute/IAttributesList.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Vertices.h>

using namespace graphics;

bool Shader3DDefault::isDefaultSourceLoaded = false;
std::string Shader3DDefault::defaultFileVSSource = "";
std::string Shader3DDefault::defaultFileFSSource = "";

Shader3DDefault::Shader3DDefault()
	: super()
{
	//void
}

void Shader3DDefault::init(IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList) {
	if (!isDefaultSourceLoaded) {
		std::string* sourceData;
		std::string path = "shaders/default3D.vert";
		std::shared_ptr<base::IFileHandlerBlocking> fh = base::Util_FileSystem::fs->getFileHandlerBlocking(path, base::Util_FileSystem::FSTarget::EmbeddedAssets);
		try {
			sourceData = fh->readCppStringP();
		} catch (Exception& x) {
			throw LogicException(LOC);
		}
		if (sourceData == nullptr) {
			throw LogicException(LOC);
		}
		defaultFileVSSource.assign(sourceData->c_str(), sourceData->size());
		delete sourceData;

		path = "shaders/default3D.frag";
		fh = base::Util_FileSystem::fs->getFileHandlerBlocking(path, base::Util_FileSystem::FSTarget::EmbeddedAssets);
		try {
			sourceData = fh->readCppStringP();
		} catch (Exception& x) {
			throw LogicException(LOC);
		}
		if (sourceData == nullptr) {
			throw LogicException(LOC);
		}
		defaultFileFSSource.assign(sourceData->c_str(), sourceData->size());
		delete sourceData;

		isDefaultSourceLoaded = true;
	}
	
	hasNormal3D = renderable->getGeometryProvider_Vertices()->getVertexAttributesList()->hasAll(VertexAttributesList::Type::Normal3D->typeBitGroup);

	RABool* useShaderDepthMapBorder = rendererAttributesList->getOptionalAs<RABool*>(RABool::TypeBitGroup_UseShaderDepthMapBorder);
	isDoInShaderDepthMapTextureBorderToMaxDistanceFlag = (useShaderDepthMapBorder != nullptr && useShaderDepthMapBorder->value == true) ? true : false;

	MABool* maLightingStatus = environment->getOptionalAs<MABool*>(MABool::TypeBitGroup_LightingStatus);
	isLightingEnabled = (maLightingStatus != nullptr && maLightingStatus->value) ? true : false;

	MADirectionalLightsList* maDirectionalLightsList = environment->getOptionalAs<MADirectionalLightsList*>(MADirectionalLightsList::TypeBitGroup);
	if (maDirectionalLightsList != nullptr && maDirectionalLightsList->peekLightsListManaged().count() > 0) {
		numDirectionalLights = maDirectionalLightsList->peekLightsListManaged().count();
	}

	MAPointLightsList* maPointLightsList = environment->getOptionalAs<MAPointLightsList*>(MAPointLightsList::TypeBitGroup);
	if (maPointLightsList != nullptr && maPointLightsList->peekLightsListManaged().count() > 0) {
		numPointLights = maPointLightsList->peekLightsListManaged().count();
	}

	MASpotLightsList* maSpotLightsList = environment->getOptionalAs<MASpotLightsList*>(MASpotLightsList::TypeBitGroup);
	if (maSpotLightsList != nullptr && maSpotLightsList->peekLightsListManaged().count() > 0) {
		numSpotLights = maSpotLightsList->peekLightsListManaged().count();
	}

	super::init(renderable, environment, rendererAttributesList, defaultFileVSSource, defaultFileFSSource);
}

bool Shader3DDefault::canRender(IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList) {
	bool b = super::canRender(renderable, environment, rendererAttributesList);
	if (!b) {
		return b;
	}
	
	bool o_hasNormal3D = renderable->getGeometryProvider_Vertices()->getVertexAttributesList()->hasAll(VertexAttributesList::Type::Normal3D->typeBitGroup);
	if (hasNormal3D != o_hasNormal3D) {
		return false;
	}

	RABool* useShaderDepthMapBorder = rendererAttributesList->getOptionalAs<RABool*>(RABool::TypeBitGroup_UseShaderDepthMapBorder);
	bool o_isDoInShaderDepthMapTextureBorderToMaxDistanceFlag = (useShaderDepthMapBorder != nullptr && useShaderDepthMapBorder->value == true) ? true : false;
	if (isDoInShaderDepthMapTextureBorderToMaxDistanceFlag != o_isDoInShaderDepthMapTextureBorderToMaxDistanceFlag) {
		return false;
	}

	MABool* maLightingStatus = environment->getOptionalAs<MABool*>(MABool::TypeBitGroup_LightingStatus);
	bool o_isLightingEnabled = (maLightingStatus != nullptr && maLightingStatus->value) ? true : false;
	if (isLightingEnabled != o_isLightingEnabled) {
		return false;
	}

	int o_numDirectionalLights = 0;
	MADirectionalLightsList* maDirectionalLightsList = environment->getOptionalAs<MADirectionalLightsList*>(MADirectionalLightsList::TypeBitGroup);
	if (maDirectionalLightsList != nullptr && maDirectionalLightsList->peekLightsListManaged().count() > 0) {
		o_numDirectionalLights = maDirectionalLightsList->peekLightsListManaged().count();
	}
	if (numDirectionalLights != o_numDirectionalLights) {
		return false;
	}

	int o_numPointLights = 0;
	MAPointLightsList* maPointLightsList = environment->getOptionalAs<MAPointLightsList*>(MAPointLightsList::TypeBitGroup);
	if (maPointLightsList != nullptr && maPointLightsList->peekLightsListManaged().count() > 0) {
		o_numPointLights = maPointLightsList->peekLightsListManaged().count();
	}
	if (numPointLights != o_numPointLights) {
		return false;
	}

	int o_numSpotLights = 0;
	MASpotLightsList* maSpotLightsList = environment->getOptionalAs<MASpotLightsList*>(MASpotLightsList::TypeBitGroup);
	if (maSpotLightsList != nullptr && maSpotLightsList->peekLightsListManaged().count() > 0) {
		o_numSpotLights = maSpotLightsList->peekLightsListManaged().count();
	}
	if (numSpotLights != o_numSpotLights) {
		return false;
	}

	return true;
}

std::string Shader3DDefault::generatePrefix() {
	std::string superPrefix = super::generatePrefix();

	std::stringstream sourcePrefixSS;

	sourcePrefixSS << superPrefix;

	if (hasNormal3D) {
		sourcePrefixSS << "#define normal3DFLag\n";
	}

	if (isDoInShaderDepthMapTextureBorderToMaxDistanceFlag) {
		sourcePrefixSS << "#define isDoInShaderDepthMapTextureBorderToMaxDistanceFlag\n";
	}

	if (isLightingEnabled) {
		sourcePrefixSS << "#define lightingFlag\n";

		if (numDirectionalLights > 0) {
			sourcePrefixSS << "#define numDirectionalLights " << numDirectionalLights << "\n";
		}
		if (numPointLights > 0) {
			sourcePrefixSS << "#define numPointLights " << numPointLights << "\n";
		}

		if (numSpotLights > 0) {
			sourcePrefixSS << "#define numSpotLights " << numSpotLights << "\n";
		}
	}

	return sourcePrefixSS.str();
}


Shader3DDefault::~Shader3DDefault() {
	//void
}
