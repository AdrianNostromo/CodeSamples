#pragma once

#include <string>

class FileIdentifier {
private:
	std::string path;
	int fsTarget;

public:
	FileIdentifier(FileIdentifier const&) = delete;
	FileIdentifier(FileIdentifier &&) = default;
	FileIdentifier& operator=(FileIdentifier const&) = delete;
	FileIdentifier& operator=(FileIdentifier &&) = default;

	explicit FileIdentifier(std::string& path, int fsTarget);

    std::string& getPath();
    int getFSTarget();

	virtual ~FileIdentifier();
};
