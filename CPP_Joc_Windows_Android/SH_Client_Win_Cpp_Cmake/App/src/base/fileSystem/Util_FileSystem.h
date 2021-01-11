#pragma once

#include "IFileSystem.h"
#include <memory>

namespace base {
class Util_FileSystem {
public:
    class FSTarget {
    public:
        static const int EmbeddedAssets;
        static const int Private;
        static const int Public;
    };
	
    class FSAccessMode {
    public:
        static const int Blocking;
        static const int Async;
    };

    class ResourceType {
    public:
        static const int File;
        static const int Directory;
    };
public:
    // This is init by FileSystemBase constructor.
    static IFileSystem* fs;

};
};
