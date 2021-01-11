#include "String.h"

using namespace base;

String::String(std::string& str)
    : super(),
      str(str)
{
    //void
}

String::String(char* bytes, int bytesCount)
    : super(),
      str(bytes, bytesCount)
{
    //void
}

const char* String::cString() {
    return str.c_str();
}

String::~String() {
    reservedDisposeIfNeeded();// Note1001
}
