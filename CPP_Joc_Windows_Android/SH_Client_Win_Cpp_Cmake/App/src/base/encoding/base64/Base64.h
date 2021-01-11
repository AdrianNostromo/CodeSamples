#pragma once

#include <string>
#include <memory>
#include <base/buffer/Buffer.h>

namespace base {
namespace encoding {
class Base64 {
private:
    static const std::string base64_chars;

public:
    static std::string encode(const char* bytes_to_encode, unsigned int in_len);
    static std::string encode(unsigned char const* bytes_to_encode, unsigned int in_len);
    static base::Buffer decode(std::string const& encoded_string);

private:
    static inline bool is_base64(unsigned char c);
    static int decode_countBytes(std::string const& encoded_string);

};
};
};
