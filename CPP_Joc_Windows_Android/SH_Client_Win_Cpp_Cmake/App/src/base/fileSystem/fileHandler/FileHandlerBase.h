#pragma once

#include <base/gh.h>
#include "IFileHandlerBase.h"

class FileHandlerBase : virtual public IFileHandlerBase {
	prot std::string path;

	// File or Directory.
	prot int resourceType;

    pub dCtor(FileHandlerBase);

	pub explicit FileHandlerBase(std::string const& path);

	pub std::string& getPath() final;
	pub void setPath(std::string const& path) override;

	pub std::string extractContainingDirectory() override;

	prot static int DeduceResourceType(std::string& path);
	prot static std::string ExtractContainingDirectory(std::string const& path, int resourceType);

	pub ~FileHandlerBase() override;
};
