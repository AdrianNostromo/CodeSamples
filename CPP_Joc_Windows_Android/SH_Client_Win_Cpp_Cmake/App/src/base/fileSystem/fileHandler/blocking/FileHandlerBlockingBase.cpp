#include "FileHandlerBlockingBase.h"

using namespace base;

FileHandlerBlockingBase::FileHandlerBlockingBase(std::string const& fsPathPrefix, std::string const& path)
	: super(path),
	fsPathPrefix(fsPathPrefix)
{
	fullPath = fsPathPrefix + path;
	resourceType = DeduceResourceType(fullPath);
}

void FileHandlerBlockingBase::setPath(std::string const& path) {
	super::setPath(path);
	
	fullPath = fsPathPrefix + path;

	resourceType = DeduceResourceType(fullPath);
}

FileHandlerBlockingBase::~FileHandlerBlockingBase() {
	//void
}
