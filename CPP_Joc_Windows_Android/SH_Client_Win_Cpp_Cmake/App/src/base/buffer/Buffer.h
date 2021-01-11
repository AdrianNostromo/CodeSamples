#pragma once

#include <base/gh.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <memory>

namespace base {
class Buffer final {
    priv class Stats {
        pub static int activesCount_bufferObjects;
        pub static int functionCallsCount_newBuffer;
        pub static int functionCallsCount_deleteBuffer;
    };

    priv class SharedData {pub dCtor(SharedData);
        pub void* block = nullptr;
        pub int32_t bytesCount;

        // true: delete buffer in destructor.
        pub bool isBufferOwner;

        pub bool isBlockMemTracked;

        pub int useCount = 0;

        pub explicit SharedData(void* block, int32_t bytesCount, bool isBufferOwner, bool isBlockMemTracked);

        pub ~SharedData();
    };
    
    priv SharedData* sharedData;

    pub Buffer() noexcept;
    pub Buffer(std::nullptr_t) noexcept;
    // Don't make explicit so it can be called as a optimisation for the example:
    /// sp<RegisteredEntry> registeredEntry = new RegisteredEntry(this, turnBasedEntity);
    pub Buffer(void* block, int32_t bytesCount, bool isBufferOwner, bool isBlockMemTracked) noexcept;
    pub Buffer(int32_t bytesCount) noexcept;

    // Copy constructor.
    pub Buffer(Buffer const&) noexcept;
    // Move constructor.
    pub Buffer(Buffer&&) noexcept;

    // Copy operator.
    pub Buffer& operator=(Buffer const&) noexcept;
    // Move operator.
    pub Buffer& operator=(Buffer&&) noexcept;

    pub bool operator==(Buffer const&) noexcept;
    pub bool operator==(std::nullptr_t) noexcept;

    pub bool operator!=(Buffer const&) noexcept;
    pub bool operator!=(std::nullptr_t) noexcept;

    priv void setSharedData(SharedData* newSharedData) noexcept;

    pub unsigned int use_count();

    pub int32_t getBytesCount();

    pub void memsetData(signed char value);

    pub void* getBlock();
    pub char* getBlockCharPointer();
    pub unsigned char* getBlockUnsignedCharPointer();
    pub char* extractBufferMemory();

    pub int8_t readByte(int32_t byteOffset);
    pub void writeByte(int32_t byteOffset, int8_t value);

    pub uint8_t readUbyte(int32_t byteOffset);
    pub void writeUbyte(int32_t byteOffset, uint8_t value);

    pub bool readBool(int32_t byteOffset);
    pub void writeBool(int32_t byteOffset, bool value);

    pub int16_t readShort(int32_t byteOffset, bool bigEndian);
    pub int16_t readShortBE(int32_t byteOffset);
    pub int16_t readShortLE(int32_t byteOffset);

    pub void writeShort(int32_t byteOffset, bool bigEndian, int16_t value);
    pub void writeShortBE(int32_t byteOffset, int16_t value);
    pub void writeShortLE(int32_t byteOffset, int16_t value);

    pub int32_t readInt(int32_t byteOffset, bool bigEndian);
    pub int32_t readIntBE(int32_t byteOffset);
    pub int32_t readIntLE(int32_t byteOffset);

    pub void writeInt(int32_t byteOffset, bool bigEndian, int32_t value);
    pub void writeIntBE(int32_t byteOffset, int32_t value);
    pub void writeIntLE(int32_t byteOffset, int32_t value);

    pub float readFloat(int32_t byteOffset, bool bigEndian);
    pub float readFloatBE(int32_t byteOffset);
    pub float readFloatLE(int32_t byteOffset);

    pub void writeFloat(int32_t byteOffset, bool bigEndian, const float value);
    pub void writeFloatBE(int32_t byteOffset, const float value);
    pub void writeFloatLE(int32_t byteOffset, const float value);

    pub Buffer readBuffer(int32_t byteOffset, const int32_t bytesCount);
    pub std::shared_ptr<Buffer> readBufferSP(int32_t byteOffset, const int32_t bytesCount);
    pub void writeBuffer(int32_t byteOffset, Buffer& srcBuffer, int32_t srcBytesOffset, const int32_t bytesCount);

    pub void readBytes(int32_t srcByteOffset, void* destBytes, const int32_t bytesCount);
    pub void writeBytes(int32_t byteOffset, const char* srcBytes, const int32_t bytesCount);

    pub std::string readString(int32_t byteOffset);
    pub void writeString(int32_t byteOffset, std::string& value);

    pub virtual ~Buffer();
};
};
