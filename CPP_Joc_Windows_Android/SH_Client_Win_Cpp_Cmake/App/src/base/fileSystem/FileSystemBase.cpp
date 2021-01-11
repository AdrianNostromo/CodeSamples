#include "FileSystemBase.h"
#include "Util_FileSystem.h"

using namespace base;

FileSystemBase::FileSystemBase()
	: super()
{
    Util_FileSystem::fs = this;
}

void FileSystemBase::initVars(IApp* app) {
    this->app = app;
}

FileSystemBase::~FileSystemBase() {
    //void
}
