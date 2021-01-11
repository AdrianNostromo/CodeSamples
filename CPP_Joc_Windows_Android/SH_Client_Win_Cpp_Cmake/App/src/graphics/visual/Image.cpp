#include "Image.h"

using namespace base;

Image::Image(
    base::Buffer* buffer,
    int width, int height, int bitsPerPixel_NOT_BYTES)
    : super(),
      buffer(buffer),
      width(width), height(height), bitsPerPixel_NOT_BYTES(bitsPerPixel_NOT_BYTES)
{
    //void
}

Image::~Image() {
    if(buffer != nullptr) {
        delete buffer;

        return ;
    }
}
