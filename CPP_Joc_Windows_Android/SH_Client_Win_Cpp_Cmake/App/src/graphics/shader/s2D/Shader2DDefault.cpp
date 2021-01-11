#include "Shader2DDefault.h"
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>
#include <memory>
#include <base/fileSystem/Util_FileSystem.h>
#include <graphics/shader/program/ShaderProgram.h>

using namespace graphics;

bool Shader2DDefault::isDefaultSourceLoaded = false;
std::string Shader2DDefault::defaultFileVSSource = "";
std::string Shader2DDefault::defaultFileFSSource = "";

Shader2DDefault::Shader2DDefault()
	: super()
{
	//void
}

void Shader2DDefault::init(IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList) {
	if (!isDefaultSourceLoaded) {
		std::string* sourceData;
		std::string path = "shaders/default2D.vert";
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

		path = "shaders/default2D.frag";
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

Shader2DDefault::~Shader2DDefault() {
	//void
}
