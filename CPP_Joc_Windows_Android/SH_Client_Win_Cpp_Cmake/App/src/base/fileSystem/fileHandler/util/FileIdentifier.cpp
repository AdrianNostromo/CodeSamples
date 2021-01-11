#include "FileIdentifier.h"

FileIdentifier::FileIdentifier(std::string& path, int fsTarget)
    : path(path), fsTarget(fsTarget)
{
	//void
}

std::string& FileIdentifier::getPath() {
    return path;
}

int FileIdentifier::getFSTarget() {
    return fsTarget;
}

FileIdentifier::~FileIdentifier() {
	//void
}
