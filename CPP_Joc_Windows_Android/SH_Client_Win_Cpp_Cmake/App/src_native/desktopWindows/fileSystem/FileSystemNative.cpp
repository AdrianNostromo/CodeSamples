#include <base/fileSystem/Util_FileSystem.h>
#include "FileSystemNative.h"
#include <base/exceptions/LogicException.h>
#include <desktopWindows/fileSystem/fileHandler/blocking/FileHandlerBlockingHdd.h>

FileSystemNative::FileSystemNative(std::string& fsPath_embeddedAssets, std::string& fsPath_private)
    : super(),
    fsPath_embeddedAssets(fsPath_embeddedAssets), fsPath_private(fsPath_private)
{
    //void
}

std::shared_ptr<base::IFileHandlerBlocking> FileSystemNative::getFileHandlerBlocking(std::string const& path, int fsTarget) {
	std::shared_ptr<base::IFileHandlerBlocking> ret;
	if (fsTarget == base::Util_FileSystem::FSTarget::EmbeddedAssets) {
		ret = std::make_shared<desktopWindows::FileHandlerBlockingHdd>(fsPath_embeddedAssets, path);
	} else if (fsTarget == base::Util_FileSystem::FSTarget::Private) {
		ret = std::make_shared<desktopWindows::FileHandlerBlockingHdd>(fsPath_private, path);
	} else if (fsTarget == base::Util_FileSystem::FSTarget::Public) {
		// Implement when needed (my documents root or something). May be used to save screenshots.
		throw LogicException(LOC);
	} else {
		throw LogicException(LOC);
	}

	return ret;
}

FileSystemNative::~FileSystemNative() {
    //void
}
