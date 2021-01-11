#pragma once

#include <base/gh.h>
#include <base/fileSystem/FileSystem.h>

class FileSystemNative : public base::FileSystem {priv typedef base::FileSystem super; pub dCtor(FileSystemNative);
    priv std::string fsPath_embeddedAssets;
    priv std::string fsPath_private;

	pub explicit FileSystemNative(std::string& fsPath_embeddedAssets, std::string& fsPath_private);
	
	pub std::shared_ptr<base::IFileHandlerBlocking> getFileHandlerBlocking(std::string const& path, int fsTarget) override;

	pub ~FileSystemNative() override;
};
