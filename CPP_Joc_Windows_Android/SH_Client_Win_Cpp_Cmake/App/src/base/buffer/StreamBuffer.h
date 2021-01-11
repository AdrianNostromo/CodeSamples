#pragma once

#include <base/gh.h>
#include <string>
#include <base/list/Array1D.h>
#include <memory>
#include "Buffer.h"

namespace base {
class StreamBuffer {pub dCtor(StreamBuffer);
    priv base::Buffer buffer = nullptr;
    priv int offset = 0;

    priv int boundsLimit;

    priv int postBoundsLength;
    priv bool bigEndian;

    pub explicit StreamBuffer(base::Buffer preExistingBuffer);
    pub explicit StreamBuffer(const int bytesCount);

    pub static int stringInBufferSize(std::string& value);

    pub int readInt();
    pub char readByte();
    pub float readFloat();
    pub short readShort();
    pub bool readBool();
    pub std::string readString();
    pub base::Buffer readBuffer(int len);
    pub std::shared_ptr<base::StreamBuffer> readStreamBuffer(int len);

    pub int peekInt();
    pub int peekIntUnsafe(const int extraOffset);

    pub virtual StreamBuffer* writeInt(const int value);
    pub StreamBuffer* writeByte(const char value);
    pub StreamBuffer* writeFloat(const float value);
    pub void writeShort(const short value);
    pub void writeBool(const bool value);
    pub virtual StreamBuffer* writeString(std::string& value);
    pub virtual StreamBuffer* writeBuffer(StreamBuffer& srcBytes);
    pub void writeBuffer(StreamBuffer& srcBuffer, const int count);
    pub void readBytes(void* destBytes, const int count);
    pub StreamBuffer* writeBytes(const char* srcBytes, const int count);

    pub int skipInt();

    pub base::Buffer& getBuffer();

    pub int getOffset();
    pub void setOffset(const int offset);

    pub int getLength();
    pub int getCount();

    pub int getBoundsLimit();
    pub void setBoundsLimit(const int boundsLimit);

    pub int getPostBoundsLength();

    pub virtual ~StreamBuffer();
};
};
