#pragma once

#include <memory>
#include <string>
#include <sstream>
#include <base/buffer/Buffer.h>
#include <base/sp.h>
#include <base/string/String.h>

class IFileHandlerBase {
public:
	virtual std::string& getPath() = 0;
	virtual void setPath(std::string const& path) = 0;

	virtual std::string extractContainingDirectory() = 0;
	
	virtual void createOrClear() = 0;
	virtual void deleteIfExists() = 0;

	virtual void rename(std::string const& newResourceName, bool replaceIfExists = false) = 0;
	// Directories can only be renamed, not moved.
	virtual void renameOrMove(std::string const& newPath, bool replaceIfExists = false) = 0;

	virtual void write(base::Buffer& data) = 0;
	virtual void write(std::shared_ptr<std::string> data) = 0;
	virtual void write(sp<std::string> data) = 0;
	virtual void write(std::shared_ptr<std::stringstream> data) = 0;

	virtual void createBoundedDirectories() = 0;

    virtual ~IFileHandlerBase() = default;
};
