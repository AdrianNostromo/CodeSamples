#pragma once

#include <base/gh.h>
#include <base/fileSystem/fileHandler/FileHandlerBase.h>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>

namespace base {
class FileHandlerBlockingBase : public FileHandlerBase, virtual public IFileHandlerBlocking {priv typedef FileHandlerBase super;pub dCtor(FileHandlerBlockingBase);
	prot std::string fsPathPrefix;

	prot std::string fullPath;

	pub explicit FileHandlerBlockingBase(std::string const& fsPathPrefix, std::string const& path);

	pub void setPath(std::string const& path) override;

    pub ~FileHandlerBlockingBase() override;
};
};
