#pragma once

#include <base/gh.h>
#include <memory>
#include <string>
#include <base/buffer/Buffer.h>
#include <base/string/String.h>
#include <base/fileSystem/fileHandler/IFileHandlerBase.h>

namespace base {
class IFileHandlerBlocking : virtual public IFileHandlerBase {priv typedef IFileHandlerBase super;
public:
    virtual bool isExists() = 0;

    virtual char* readBinaryCharP(int* oBuffLen) = 0;
    virtual base::Buffer* readBinaryP() = 0;
    virtual base::Buffer readBinarySP() = 0;

    virtual base::String* readStringP() = 0;
    virtual std::shared_ptr<base::String> readStringSP() = 0;

    virtual std::string* readCppStringP() = 0;
    virtual std::shared_ptr<std::string> readCppStringSP() = 0;

	using super::write;
    // Don't put these in the base interface class because these types are not placeable in a operations list that is used by async file handler.
	virtual void write(std::string& data) = 0;
	virtual void write(std::stringstream& data) = 0;

    virtual ~IFileHandlerBlocking() = default;
};
};
