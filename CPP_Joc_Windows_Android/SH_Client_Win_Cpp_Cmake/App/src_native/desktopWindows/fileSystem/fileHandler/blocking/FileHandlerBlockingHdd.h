#pragma once

#include <base/gh.h>
#include <base/string/String.h>
#include <base\fileSystem\fileHandler\blocking\FileHandlerBlockingBase.h>

namespace desktopWindows {
class FileHandlerBlockingHdd : public base::FileHandlerBlockingBase {priv typedef base::FileHandlerBlockingBase super;pub dCtor(FileHandlerBlockingHdd);
	priv static const bool IS_DEBUG;

	pub explicit FileHandlerBlockingHdd(std::string const& fsPathPrefix, std::string const& path);

	pub bool isExists() override;
	pub static bool IsExists(std::string& path);
	pub static bool IsExists(std::string& path, int resourceType);

	pub void createOrClear() final;
	pub void deleteIfExists() final;

	pub void rename(std::string const& newResourceName, bool replaceIfExists=false) override;
	pub void renameOrMove(std::string const& newPath, bool replaceIfExists = false) override;

	pub char* readBinaryCharP(int* oBuffLen) override;
	pub base::Buffer* readBinaryP() override;
	pub base::Buffer readBinarySP() override;

	pub base::String* readStringP() override ;
	pub std::shared_ptr<base::String> readStringSP() override;

	pub std::string* readCppStringP() override;
	pub std::shared_ptr<std::string> readCppStringSP() override;

	pub void write(base::Buffer& data) override;
	pub void write(std::string& data) override;
	pub void write(std::stringstream& data) override;

	pub void write(std::shared_ptr<std::string> data) override;
	pub void write(sp<std::string> data) override;
	pub void write(std::shared_ptr<std::stringstream> data) override;

	pub void createBoundedDirectories() override;
	pub static int CreateBoundedDirectories(std::string& fullPath, int resourceType);

	pub ~FileHandlerBlockingHdd() override;
};
};
