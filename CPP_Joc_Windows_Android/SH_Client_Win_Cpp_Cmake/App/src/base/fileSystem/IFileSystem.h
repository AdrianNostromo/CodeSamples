#pragma once

#include <base/gh.h>
#include <string>
#include <memory>
#include <base/object/IManagedObject.h>

class IFileHandlerBase;
class IApp;
namespace base {
	class IFileHandlerAsync;
	class IFileHandlerBlocking;
	class ICacheGroup;
}

namespace base {
class IFileSystem : virtual public base::IManagedObject {
	pub virtual void initVars(IApp* app) = 0;

	// fsAccessMode: Util_FileSystem::FSAccessMode;
	pub virtual std::shared_ptr<IFileHandlerBase> getFileHandler(std::string const& path, int fsTarget, int fsAccessMode) = 0;

	pub virtual std::shared_ptr<IFileHandlerBlocking> getFileHandlerBlocking(std::string const& path, int fsTarget) = 0;
	pub virtual std::shared_ptr<IFileHandlerAsync> getFileHandlerAsync(std::string const& path, int fsTarget, base::ICacheGroup* cacheGroup=nullptr) = 0;

	// Return true; Processing occured.
	// Return fals; Nothing flushed.
	pub virtual bool flush() = 0;

	pub virtual ~IFileSystem() = default;
};
};
