// This is used because a compilation error occurs that std::experimental will be removed in c++17.
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "FileHandlerBlockingHdd.h"
#include <base/exceptions/FileNotFoundException.h>
#include <base/exceptions/LogicException.h>
#include <cstdio>
#include <iostream>
#include <sys/stat.h>
#include <sstream>
#include <base/exceptions/NotDirectoryException.h>
#include <base/exceptions/NotFileException.h>
#include <base/util/StringUtil.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/exceptions/ResourceNotFoundException.h>
#include <base/exceptions/ResourceAlreadyExistsException.h>
#include <experimental/filesystem>
#include <base/fileSystem/fileHandler/util/FileIOProfilerStatics.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/app/config/AppConfig.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#endif

typedef struct stat Stat;

#if defined(_WIN32) || defined(_WIN64)
#define	S_ISDIR(m)	(m & S_IFDIR)	/* directory */
#define	S_ISREG(m)	(m & S_IFREG)	/* regular file */
#endif

using namespace desktopWindows;

const bool FileHandlerBlockingHdd::IS_DEBUG = false;

FileHandlerBlockingHdd::FileHandlerBlockingHdd(std::string const& fsPathPrefix, std::string const& path)
    : super(fsPathPrefix, path)
{
    //void
}

bool FileHandlerBlockingHdd::isExists() {
    return IsExists(fullPath, resourceType);
}

bool FileHandlerBlockingHdd::IsExists(std::string& path) {
    int resourceType = DeduceResourceType(path);

    return IsExists(path, resourceType);
}

bool FileHandlerBlockingHdd::IsExists(std::string& path, int resourceType) {
    Stat st;
    if (stat(path.c_str(), &st) != 0) {
        if(errno == ENOENT) {
            return false;
        }else {
            throw LogicException(LOC);
        }
    }

    if(resourceType == base::Util_FileSystem::ResourceType::File) {
		bool b = S_ISREG(st.st_mode);
        if(!b) {
            // Resource exists but it has wrong type.
            throw LogicException(LOC);
        }
    }else if(resourceType == base::Util_FileSystem::ResourceType::Directory) {
		bool b = S_ISDIR(st.st_mode);
        if(!b) {
            // Resource exists but it has wrong type.
            throw LogicException(LOC);
        }
    }else {
        throw LogicException(LOC);
    }

    return true;
}

void FileHandlerBlockingHdd::createOrClear() {
    if (AppConfig::IS_DEBUG && IS_DEBUG) {
        GlobalAccessLogHandler::devLogHandler->post_fileIO("File IO (sync). createOrClear.pre: %s", fullPath.c_str());
    }

    if(!isExists()) {
        // This will create the file if it doesn't exist. If the file exists it's content is discarded.
        CreateBoundedDirectories(fullPath, resourceType);

        FILE* fp = fopen(fullPath.c_str(), "wb");
        if (!fp) {
            throw LogicException(LOC);
        }
        // Don't write anything.
        fclose(fp);
    }

    base::FileIOProfilerStatics::COUNTER_FileIOSyncTotal++;
}

void FileHandlerBlockingHdd::deleteIfExists() {
    if (AppConfig::IS_DEBUG && IS_DEBUG) {
        GlobalAccessLogHandler::devLogHandler->post_fileIO("File IO (sync). deleteIfExists.pre: %s", fullPath.c_str());
    }

    if(isExists()) {
		int filesDeleted = std::experimental::filesystem::remove_all(fullPath.c_str());
        // Note. After testing, on win_10, remove_all does not count any directories or sub-directories, only files.
        /// Don't use filesDeleted for error detection.
    }

    base::FileIOProfilerStatics::COUNTER_FileIOSyncTotal++;
}

void FileHandlerBlockingHdd::rename(std::string const& newResourceName, bool replaceIfExists) {
    if (AppConfig::IS_DEBUG && IS_DEBUG) {
        GlobalAccessLogHandler::devLogHandler->post_fileIO("File IO (sync). rename.pre: %s to %s", fullPath.c_str(), newResourceName.c_str());
    }

    if(resourceType != base::Util_FileSystem::ResourceType::File && resourceType != base::Util_FileSystem::ResourceType::Directory) {
        // Unhandled resource type.
        throw LogicException(LOC);
    }

    // The new resource name must not contain extra directories information.
    int i = StringUtil::indexOf(newResourceName, "/");
    if(i >= 0 && i != (int)(newResourceName.size() - 1)) {
        throw LogicException(LOC);
    }

    if(!isExists()) {
        throw ResourceNotFoundException();
    }

    std::string newPath = extractContainingDirectory() + newResourceName;
    std::string newFullPath = fsPathPrefix + newPath;
    int newResourceType = DeduceResourceType(newFullPath);
    if(resourceType != newResourceType) {
        // Resource type will change. Not allowed.
        throw LogicException(LOC);
    }

    if(IsExists(newFullPath, newResourceType)) {
		if (replaceIfExists) {
			if (std::experimental::filesystem::remove_all(newFullPath.c_str()) == false) {
				throw LogicException(LOC);
			}
		} else {
			throw ResourceAlreadyExistsException();
		}
    }

    CreateBoundedDirectories(newFullPath, resourceType);

    int r = std::rename(fullPath.c_str(), newFullPath.c_str());
    if(r != 0) {
        // Some error occured.
        throw LogicException(LOC);
    }

    setPath(newPath);

    base::FileIOProfilerStatics::COUNTER_FileIOSyncTotal++;
}

void FileHandlerBlockingHdd::renameOrMove(std::string const& newPath, bool replaceIfExists) {
    if (AppConfig::IS_DEBUG && IS_DEBUG) {
        GlobalAccessLogHandler::devLogHandler->post_fileIO("File IO (sync). renameOrMove.pre: %s to %s", fullPath.c_str(), newPath.c_str());
    }

    if(resourceType == base::Util_FileSystem::ResourceType::Directory) {
        // Extract the containing directory from the current and new paths.
        std::string currentPathContainingDirectoryPath = extractContainingDirectory();
        std::string newPathContainingDirectoryPath = ExtractContainingDirectory(newPath, resourceType);

        if(currentPathContainingDirectoryPath != newPathContainingDirectoryPath) {
            // Directories can only be renamed, not moved.
            throw LogicException(LOC);
        }
    }

    if(!isExists()) {
        throw ResourceNotFoundException();
    }

    std::string newFullPath = fsPathPrefix + newPath;
    int newResourceType = DeduceResourceType(newFullPath);
    if(resourceType != newResourceType) {
        // Resource type will change. Not allowed.
        throw LogicException(LOC);
    }

    if(IsExists(newFullPath, newResourceType)) {
		if (replaceIfExists) {
			if (std::experimental::filesystem::remove_all(newFullPath.c_str()) == false) {
				// The file was checked to exist but no file was deleted.
				throw LogicException(LOC);
			}
		} else {
			throw ResourceAlreadyExistsException();
		}
    }

    CreateBoundedDirectories(newFullPath, resourceType);

    if(std::rename(fullPath.c_str(), newFullPath.c_str()) != 0) {
        // Some error occured.
        throw LogicException(LOC);
    }

    setPath(newPath);

    base::FileIOProfilerStatics::COUNTER_FileIOSyncTotal++;
}

char* FileHandlerBlockingHdd::readBinaryCharP(int* oBuffLen) {
    if (AppConfig::IS_DEBUG && IS_DEBUG) {
        GlobalAccessLogHandler::devLogHandler->post_fileIO("File IO (sync). readBinaryCharP.pre: %s", fullPath.c_str());
    }

    FILE* f = fopen(fullPath.c_str(), "rb");// rb(read binary)
    if (f == nullptr) {
        throw FileNotFoundException();
    }

    fseek(f, 0, SEEK_END);
    int buffLen = (int)ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buff = (char*)malloct(buffLen);
    fread(buff, 1, buffLen, f);
    fclose(f);

    *oBuffLen = buffLen;

    base::FileIOProfilerStatics::COUNTER_FileIOSyncTotal++;

    return buff;
}

base::Buffer* FileHandlerBlockingHdd::readBinaryP() {
    int buffLen;
    char* buff = readBinaryCharP(&buffLen);

    base::Buffer* ret = newt base::Buffer(buff, buffLen, true/*isBufferOwner*/, true/*isBlockMemTracked*/);

    return ret;
}

base::Buffer FileHandlerBlockingHdd::readBinarySP() {
    int buffLen;
    char* buff = readBinaryCharP(&buffLen);

    base::Buffer ret{ buff, buffLen, true/*isBufferOwner*/, true/*isBlockMemTracked*/ };

    return ret;
}

base::String* FileHandlerBlockingHdd::readStringP() {
    base::Buffer* data = readBinaryP();

    base::String* s = newt base::String(
        data->getBlockCharPointer(), data->getBytesCount()
    );

    delete data;

    return s;
}

std::shared_ptr<base::String> FileHandlerBlockingHdd::readStringSP() {
    base::Buffer* data = readBinaryP();

    std::shared_ptr<base::String> s = std::make_shared<base::String>(
            data->getBlockCharPointer(), data->getBytesCount()
    );

    delete data;

    return s;
}

std::string* FileHandlerBlockingHdd::readCppStringP() {
    base::Buffer* data = readBinaryP();

    std::string* s = newt std::string(
        data->getBlockCharPointer(), (unsigned long)data->getBytesCount()
    );

    delete data;

    return s;
}

std::shared_ptr<std::string> FileHandlerBlockingHdd::readCppStringSP() {
    base::Buffer* data = readBinaryP();

    std::shared_ptr<std::string> s = std::make_shared<std::string>(
        data->getBlockCharPointer(), (unsigned long)data->getBytesCount()
    );

    delete data;

    return s;
}

void FileHandlerBlockingHdd::write(base::Buffer& data) {
    if (AppConfig::IS_DEBUG && IS_DEBUG) {
        GlobalAccessLogHandler::devLogHandler->post_fileIO("File IO (sync). write.pre: %s", fullPath.c_str());
    }

	createBoundedDirectories();

	FILE* fp = fopen(fullPath.c_str(), "wb");
	if (!fp) {
		throw LogicException(LOC);
	}
	fwrite(data.getBlock(), 1, (size_t)data.getBytesCount(), fp);
	fclose(fp);

    base::FileIOProfilerStatics::COUNTER_FileIOSyncTotal++;
}

void FileHandlerBlockingHdd::write(std::string& data) {
    if (AppConfig::IS_DEBUG && IS_DEBUG) {
        GlobalAccessLogHandler::devLogHandler->post_fileIO("File IO (sync). write.pre: %s", fullPath.c_str());
    }

	createBoundedDirectories();

	FILE* fp = fopen(fullPath.c_str(), "wb");
	if (!fp) {
		throw LogicException(LOC);
	}
    
    fwrite(data.c_str(), 1, data.length(), fp);

	fclose(fp);

    base::FileIOProfilerStatics::COUNTER_FileIOSyncTotal++;
}

void FileHandlerBlockingHdd::write(std::stringstream& data) {
    if (AppConfig::IS_DEBUG && IS_DEBUG) {
        GlobalAccessLogHandler::devLogHandler->post_fileIO("File IO (sync). write.pre: %s", fullPath.c_str());
    }

	createBoundedDirectories();

	FILE* fp = fopen(fullPath.c_str(), "wb");
	if (!fp) {
		throw LogicException(LOC);
	}

    std::string sData = data.str();
    fwrite(sData.c_str(), 1, sData.length(), fp);

	fclose(fp);

    base::FileIOProfilerStatics::COUNTER_FileIOSyncTotal++;
}

void FileHandlerBlockingHdd::write(std::shared_ptr<std::string> data) {
	write(*data);
}

void FileHandlerBlockingHdd::write(sp<std::string> data) {
	write(*data);
}

void FileHandlerBlockingHdd::write(std::shared_ptr<std::stringstream> data) {
	write(*data);
}

void FileHandlerBlockingHdd::createBoundedDirectories() {
    CreateBoundedDirectories(fullPath, resourceType);
}

int FileHandlerBlockingHdd::CreateBoundedDirectories(std::string& fullPath, int resourceType) {
    if (AppConfig::IS_DEBUG && IS_DEBUG) {
        GlobalAccessLogHandler::devLogHandler->post_fileIO("File IO (sync). CreateBoundedDirectories.pre: %s", fullPath.c_str());
    }

    // Empty string is invalid.
    if(fullPath.size() == 0) {
        throw LogicException(LOC);
    }

    // Check if the path if for a file or directory (ends with a "/");
    bool isSuffixFileName = false;
    if(resourceType == base::Util_FileSystem::ResourceType::Directory) {
        // Directory.
        if (fullPath.size() == 1) {
            // No directories need creation.
            return false;
        }
    }else if(resourceType == base::Util_FileSystem::ResourceType::File) {
        // File.

        isSuffixFileName = true;
    }else {
        throw LogicException(LOC);
    }

    // Split into parts.
    std::shared_ptr<ArrayList<std::string>> pathParts = StringUtil::split(fullPath, "/", true);
    std::stringstream cPathSS{};
    bool isAnyDirectoryCreated = false;
    int directoriesCount = pathParts->size() - ((isSuffixFileName == true) ? 1 : 0);

    for(int i=0;i<directoriesCount;i++) {
        std::string& pathPart = pathParts->getReference(i);
        if(pathPart.size() > 0 && pathPart != ".") {
            // This allows for "/..." and "////" to be handled correctly.
            cPathSS << pathPart << "/";
            std::string cPath = cPathSS.str();

            bool b;
            try {
                b = IsExists(cPath, base::Util_FileSystem::ResourceType::Directory);
            } catch (NotDirectoryException &x) {
                throw LogicException(LOC);
            }
            if (!b) {
                // Create directory.
				#if defined(_WIN32) || defined(_WIN64)
				if (_mkdir(cPath.c_str()) != 0) {
					throw LogicException(LOC);
				}
				#else
				if (mkdir(cPath.c_str(), 0755) != 0) {
					throw LogicException(LOC);
				}
				#endif
                
                isAnyDirectoryCreated = true;
            }
        }else if(pathPart.size() <= 0) {
            // This ensures that "///" is converted to a single "/";
            if(i == 0) {
                cPathSS << "/";
            }
        }else {
            // Part == ".";
            if(i == 0) {
                cPathSS << pathPart << "/";
            }
        }
    }

    base::FileIOProfilerStatics::COUNTER_FileIOSyncTotal++;

    return isAnyDirectoryCreated;
}

FileHandlerBlockingHdd::~FileHandlerBlockingHdd() {
    //void
}
