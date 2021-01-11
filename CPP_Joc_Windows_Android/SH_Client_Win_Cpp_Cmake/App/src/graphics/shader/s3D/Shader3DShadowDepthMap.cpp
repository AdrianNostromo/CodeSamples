#include "Shader3DShadowDepthMap.h"
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>
#include <memory>
#include <base/fileSystem/Util_FileSystem.h>
#include <graphics/shader/program/ShaderProgram.h>

using namespace graphics;

bool Shader3DShadowDepthMap::isDefaultSourceLoaded = false;
std::string Shader3DShadowDepthMap::defaultFileVSSource = "";
std::string Shader3DShadowDepthMap::defaultFileFSSource = "";

Shader3DShadowDepthMap::Shader3DShadowDepthMap()
	: super()
{
	//void
}

void Shader3DShadowDepthMap::init(IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList) {
	if (!isDefaultSourceLoaded) {
		std::string* sourceData;
		std::string path = "shaders/s3DShadowDepthMap.vert";
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

		path = "shaders/s3DShadowDepthMap.frag";
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

	super::init(renderable, environment, rendererAttributesList, defaultFileVSSource, defaultFileFSSource);
}

bool Shader3DShadowDepthMap::canRender(IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList) {
	bool b = super::canRender(renderable, environment, rendererAttributesList);
	if (!b) {
		return b;
	}
	
	return true;
}

std::string Shader3DShadowDepthMap::generatePrefix() {
	std::string superPrefix = super::generatePrefix();

	std::stringstream sourcePrefixSS;

	sourcePrefixSS << superPrefix;

	return sourcePrefixSS.str();
}

Shader3DShadowDepthMap::~Shader3DShadowDepthMap() {
	//void
}
