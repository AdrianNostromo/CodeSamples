#include <base/fileSystem/Util_FileSystem.h>
#include <base/util/StringUtil.h>
#include "FileHandlerBase.h"

FileHandlerBase::FileHandlerBase(std::string const& path)
	: path(path)
{
	resourceType = DeduceResourceType(this->path);
}

std::string& FileHandlerBase::getPath() {
	return path;
}

void FileHandlerBase::setPath(std::string const& path) {
	this->path = path;

	resourceType = DeduceResourceType(this->path);
}

int FileHandlerBase::DeduceResourceType(std::string& path) {
	int resourceType;

	if(StringUtil::lastIndexOf(path, "/") == (int)path.size() - 1) {
		resourceType = base::Util_FileSystem::ResourceType::Directory;
	}else {
		resourceType = base::Util_FileSystem::ResourceType::File;
	}

	return resourceType;
}

std::string FileHandlerBase::extractContainingDirectory() {
	return ExtractContainingDirectory(path, resourceType);
}

std::string FileHandlerBase::ExtractContainingDirectory(std::string const& path, int resourceType) {
	int dirPathEnd;
	if(resourceType == base::Util_FileSystem::ResourceType::File) {
		dirPathEnd = StringUtil::lastIndexOf(path, "/", (int)(path.size() - 1));
	}else if(resourceType == base::Util_FileSystem::ResourceType::Directory) {
		dirPathEnd = StringUtil::lastIndexOf(path, "/", (int)path.size() - 2);
	}else {
		throw LogicException(LOC);
	}

	std::string containingDirectory;
	if(dirPathEnd == -1) {
		containingDirectory = "";
	}else {
		containingDirectory = StringUtil::substringRange(path, 0, dirPathEnd);
	}

	return containingDirectory;
}

FileHandlerBase::~FileHandlerBase() {
    //void
}
