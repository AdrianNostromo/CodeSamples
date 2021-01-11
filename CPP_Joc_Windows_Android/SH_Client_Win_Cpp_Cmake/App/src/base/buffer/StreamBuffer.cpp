#include "StreamBuffer.h"
#include <base/exceptions/LogicException.h>
#include <base/exceptions/BoundsException.h>
#include <base/exceptions/DataException.h>
#include <base/buffer/util/BitsUtil.h>
#include <cstring>
#include <base/MM.h>

using namespace base;

StreamBuffer::StreamBuffer(base::Buffer preExistingBuffer){
    this->buffer = preExistingBuffer;

    boundsLimit = this->buffer.getBytesCount();
    postBoundsLength = boundsLimit - boundsLimit;

    bigEndian = true;
}

StreamBuffer::StreamBuffer(const int bytesCount)
    : StreamBuffer(base::Buffer{ bytesCount })
{
    //void
}

int StreamBuffer::stringInBufferSize(std::string& value) {
    return 2 + static_cast<int>(value.size());
}

int StreamBuffer::readInt() {
    if(offset + 4 > boundsLimit) {
        throw BoundsException();
    }

    int ret = buffer.readInt(offset, bigEndian);
    offset += 4;

    return ret;
}

char StreamBuffer::readByte() {
    if(offset + 1 > boundsLimit) {
        throw BoundsException();
    }

    // NOTE : This is a signed char. The server uses unsigned bytes.
    char ret = buffer.readByte(offset);
    offset += 1;

    return ret;
}

float StreamBuffer::readFloat() {
    if(offset + 4 > boundsLimit) {
        throw BoundsException();
    }

    float ret = buffer.readFloat(offset, bigEndian);
    offset += 4;

    return ret;
}

short StreamBuffer::readShort() {
    if(offset + 2 > boundsLimit) {
        throw BoundsException();
    }

    short ret = buffer.readShort(offset, bigEndian);
    offset += 2;

    return ret;
}

bool StreamBuffer::readBool() {
    if(offset + 1 > boundsLimit) {
        throw BoundsException();
    }

    bool ret = buffer.readBool(offset);
    offset += 1;

    return ret;
}

std::string StreamBuffer::readString() {
    short stringBytesLength = buffer.readShortBE(offset);

    if(stringBytesLength < 0 || offset + stringBytesLength > boundsLimit) {
        throw BoundsException();
    }

    std::string ret = buffer.readString(offset);
    offset += 2 + stringBytesLength;

    return ret;
}

base::Buffer StreamBuffer::readBuffer(int len) {
    if(offset + len > boundsLimit) {
        throw BoundsException();
    }

    Buffer ret = buffer.readBuffer(offset, len);
    offset += len;

    return ret;
}

std::shared_ptr<base::StreamBuffer> StreamBuffer::readStreamBuffer(int len) {
    if(offset + len > boundsLimit) {
        throw BoundsException();
    }

    Buffer buff = buffer.readBuffer(offset, len);

    std::shared_ptr<base::StreamBuffer> ret = std::make_shared<base::StreamBuffer>(buff);
    offset += len;

    return ret;
}

int StreamBuffer::peekInt() {
    if(offset + 4 > boundsLimit) {
        throw BoundsException();
    }

    int ret = buffer.readInt(offset, bigEndian);
    //offset += 4;

    return ret;
}

int StreamBuffer::peekIntUnsafe(const int extraOffset) {
    int finalOffset = offset;
    finalOffset += extraOffset;

    int ret = buffer.readInt(finalOffset, bigEndian);

    return ret;
}

StreamBuffer* StreamBuffer::writeInt(const int value) {
    if(offset + 4 > boundsLimit) {
        throw BoundsException();
    }

    buffer.writeInt(offset, bigEndian, value);
    offset += 4;

    return this;
}

StreamBuffer* StreamBuffer::writeByte(const char value) {
    if(offset + 1 > boundsLimit) {
        throw BoundsException();
    }

    // NOTE : This is a signed char. The server uses unsigned bytes.
    buffer.writeByte(offset, value);
    offset += 1;

    return this;
}

StreamBuffer* StreamBuffer::writeFloat(const float value) {
    if(offset + 4 > boundsLimit) {
        throw BoundsException();
    }

    buffer.writeFloat(offset, bigEndian, value);
    offset += 4;

    return this;
}

void StreamBuffer::writeShort(const short value) {
    if(offset + 2 > boundsLimit) {
        throw BoundsException();
    }

    buffer.writeShort(offset, bigEndian, value);
    offset += 2;
}

void StreamBuffer::writeBool(const bool value) {
    if(offset + 1 > boundsLimit) {
        throw BoundsException();
    }

    buffer.writeBool(offset, value);
    offset += 1;
}

StreamBuffer* StreamBuffer::writeString(std::string& value) {
    // std::string::size(); Is the number of bytes (for multicode also), not the number of characters (including multicode).
    short stringBytesLength = static_cast<short>(value.size());

    if(offset + (2 + stringBytesLength) > boundsLimit) {
        throw BoundsException();
    }

    buffer.writeString(offset, value);
    offset += 2 + stringBytesLength;

    return this;
}

StreamBuffer* StreamBuffer::writeBuffer(StreamBuffer& srcBuffer) {
    writeBuffer(srcBuffer, srcBuffer.getCount());

    return this;
}

void StreamBuffer::writeBuffer(StreamBuffer& srcBuffer, const int count) {
    if(offset + count > boundsLimit || srcBuffer.offset + count > srcBuffer.boundsLimit) {
        throw BoundsException();
    }

    buffer.writeBuffer(offset, srcBuffer.getBuffer(), srcBuffer.offset, count);

    offset += count;
    srcBuffer.setOffset(srcBuffer.getOffset() + count);
}

void StreamBuffer::readBytes(void* destBytes, const int count) {
    // Do checks on the src and this to make sure enough data and space exists.
    if (offset + count > boundsLimit) {
        throw BoundsException();
    }

    // Copy the bytes.
    buffer.readBytes(offset, destBytes, count);

    // Update the offset for this->
    offset += count;
}

StreamBuffer* StreamBuffer::writeBytes(const char* srcBytes, const int count) {
    // Do checks on the src and this to make sure enough data and space exists.
    if(offset + count > boundsLimit) {
        throw BoundsException();
    }

    // Copy the bytes.
    buffer.writeBytes(offset, srcBytes, count);

    // Update the offset for this->
    offset += count;

    return this;
}

int StreamBuffer::skipInt() {
    if(offset + 4 > boundsLimit) {
        throw BoundsException();
    }

    return offset += 4;
}

base::Buffer& StreamBuffer::getBuffer() {
    return buffer;
}

int StreamBuffer::getOffset() {
    return offset;
}

void StreamBuffer::setOffset(const int offset) {
    this->offset = offset;
}

int StreamBuffer::getLength() {
    return buffer.getBytesCount();
}

int StreamBuffer::getCount() {
    return buffer.getBytesCount();
}

int StreamBuffer::getBoundsLimit() {
    return boundsLimit;
}

void StreamBuffer::setBoundsLimit(const int boundsLimit) {
    this->boundsLimit = boundsLimit;

    postBoundsLength = getLength() - boundsLimit;
}

int StreamBuffer::getPostBoundsLength() {
    return postBoundsLength;
}

StreamBuffer::~StreamBuffer() {
    if(buffer != nullptr) {
        buffer = nullptr;
    }
}
