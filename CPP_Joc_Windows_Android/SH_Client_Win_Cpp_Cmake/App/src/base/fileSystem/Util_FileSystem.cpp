#include "Util_FileSystem.h"

using namespace base;

IFileSystem* Util_FileSystem::fs = nullptr;

const int Util_FileSystem::FSTarget::EmbeddedAssets = 1;
const int Util_FileSystem::FSTarget::Private = 2;
const int Util_FileSystem::FSTarget::Public = 3;

const int Util_FileSystem::FSAccessMode::Blocking = 1;
const int Util_FileSystem::FSAccessMode::Async = 2;

const int Util_FileSystem::ResourceType::File = 1;
const int Util_FileSystem::ResourceType::Directory = 2;
