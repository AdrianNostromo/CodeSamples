#pragma once

#include <base/object/Object.h>
#include <string>

namespace base {
class String final/*Note1001. calls_the_reserved_dispose*/ : public Object {
private: typedef Object super;
public:
    std::string str;

public:
    String(String const&) = delete;
    String(String &&) = default;
    String& operator=(String const&) = delete;
    String& operator=(String &&) = default;

    explicit String(std::string& str);
    explicit String(char* bytes, int bytesCount);

    const char* cString();

    ~String() override;
};
};
