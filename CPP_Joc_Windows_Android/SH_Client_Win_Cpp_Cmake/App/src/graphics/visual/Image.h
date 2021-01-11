#pragma once

#include <base/buffer/Buffer.h>
#include <base/object/Object.h>

namespace base {
class Image : public Object {
private: typedef Object super;
public:
    base::Buffer* buffer;

    int width;
    int height;
    int bitsPerPixel_NOT_BYTES;

public:
    Image(Image const&) = delete;
    Image(Image &&) = default;
    Image& operator=(Image const&) = delete;
    Image& operator=(Image &&) = default;

    explicit Image(
        base::Buffer* buffer,
        int width, int height, int bitsPerPixel_NOT_BYTES);

    ~Image() override ;
};
};
