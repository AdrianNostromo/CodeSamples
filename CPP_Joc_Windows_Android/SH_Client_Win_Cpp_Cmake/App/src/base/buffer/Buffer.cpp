#include "Buffer.h"
#include <base/exceptions/LogicException.h>
#include <base/exceptions/BoundsException.h>
#include <base/buffer/util/BitsUtil.h>
#include <cstring>
#include <base/exceptions/DataFormatException.h>
#include <base/MM.h>
#include <cassert>

using namespace base;

int Buffer::Stats::activesCount_bufferObjects = 0;
int Buffer::Stats::functionCallsCount_newBuffer = 0;
int Buffer::Stats::functionCallsCount_deleteBuffer = 0;

Buffer::SharedData::SharedData(void* block, int32_t bytesCount, bool isBufferOwner, bool isBlockMemTracked)
    : block(block), bytesCount(bytesCount), isBufferOwner(isBufferOwner), isBlockMemTracked(isBlockMemTracked)
{};

Buffer::SharedData::~SharedData() {
    if (block != nullptr) {
        if (isBufferOwner) {
            // Don't call delete[] on large arrays that don't use destructors.
            if (isBlockMemTracked) {
                freet(block);
            } else {
                free(block);
            }
        }

        block = nullptr;
    }
};

Buffer::Buffer() noexcept
    : sharedData(nullptr)
{
    //void
}

Buffer::Buffer(std::nullptr_t) noexcept
    : sharedData(nullptr)
{
    //void
}

Buffer::Buffer(void* block, int32_t bytesCount, bool isBufferOwner, bool isBlockMemTracked) noexcept
    : sharedData(nullptr)
{
    Stats::activesCount_bufferObjects++;
    Stats::functionCallsCount_newBuffer++;

    setSharedData(newt SharedData(block, bytesCount, isBufferOwner, isBlockMemTracked));
    //void
}

Buffer::Buffer(const int32_t bytesCount) noexcept
    : Buffer(malloct(bytesCount), bytesCount, true/*isBufferOwner*/, true/*isBlockMemTracked*/)
{
    //void
}

// Copy constructor.
Buffer::Buffer(Buffer const& other) noexcept
    : sharedData(nullptr)
{
    setSharedData(other.sharedData);
}

// Move constructor.
Buffer::Buffer(Buffer&& other) noexcept
    : sharedData(nullptr)
{
    setSharedData(other.sharedData);
    other.setSharedData(nullptr);
}

// Copy operator.
Buffer& Buffer::operator=(Buffer const& other) noexcept {
    setSharedData(other.sharedData);

    return *this;
}

// Move operator.
Buffer& Buffer::operator=(Buffer&& other) noexcept {
    setSharedData(other.sharedData);
    other.setSharedData(nullptr);

    return *this;
}

bool Buffer::operator==(Buffer const& other) noexcept {
    return sharedData == other.sharedData;
}

bool Buffer::operator==(std::nullptr_t) noexcept {
    return sharedData == nullptr;
}

bool Buffer::operator!=(Buffer const& other) noexcept {
    return sharedData != other.sharedData;
}

bool Buffer::operator!=(std::nullptr_t) noexcept {
    return sharedData != nullptr;
}

void Buffer::setSharedData(SharedData* newSharedData) noexcept {
    if (this->sharedData != nullptr) {
        this->sharedData->useCount--;
        assert(this->sharedData->useCount >= 0);
        if (this->sharedData->useCount == 0) {
            Stats::activesCount_bufferObjects--;
            Stats::functionCallsCount_deleteBuffer++;

            delete this->sharedData;
        }

        this->sharedData = nullptr;
    }

    if (newSharedData != nullptr) {
        this->sharedData = newSharedData;

        this->sharedData->useCount++;
    }
}

unsigned int Buffer::use_count() {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    return sharedData->useCount;
}

int32_t Buffer::getBytesCount() {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    return sharedData->bytesCount;
}

void Buffer::memsetData(signed char value) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }
    
    memset(sharedData->block, value, sharedData->bytesCount);
}

void* Buffer::getBlock() {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    return sharedData->block;
}

char* Buffer::getBlockCharPointer() {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    return static_cast<char*>(sharedData->block);
}

unsigned char* Buffer::getBlockUnsignedCharPointer() {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    return static_cast<unsigned char*>(sharedData->block);
}

char* Buffer::extractBufferMemory() {
    if (sharedData == nullptr || sharedData->block == nullptr) {
        throw LogicException(LOC);
    }

    char* ret = static_cast<char*>(sharedData->block);
    sharedData->block = nullptr;

    return ret;
}

int8_t Buffer::readByte(int32_t byteOffset) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 1 > sharedData->bytesCount) {
        throw BoundsException();
    }

    // NOTE : This is a signed char. The server uses unsigned bytes.
    char ret = static_cast<char*>(sharedData->block)[byteOffset];

    return ret;
}

void Buffer::writeByte(int32_t byteOffset, int8_t value) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 1 > sharedData->bytesCount) {
        throw BoundsException();
    }

    static_cast<char*>(sharedData->block)[byteOffset] = value;
}

uint8_t Buffer::readUbyte(int32_t byteOffset) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 1 > sharedData->bytesCount) {
        throw BoundsException();
    }

    // NOTE : This is a signed char. The server uses unsigned bytes.
    uint8_t ret = static_cast<uint8_t*>(sharedData->block)[byteOffset];

    return ret;
}

void Buffer::writeUbyte(int32_t byteOffset, uint8_t value) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 1 > sharedData->bytesCount) {
        throw BoundsException();
    }

    static_cast<uint8_t*>(sharedData->block)[byteOffset] = value;
}

bool Buffer::readBool(int32_t byteOffset) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 1 > sharedData->bytesCount) {
        throw BoundsException();
    }

    char ret = readByte(byteOffset);

    bool boolRet;
    if(ret == 0) {
        boolRet = false;
    }else if(ret == 1) {
        boolRet = true;
    }else {
        throw DataFormatException();
    }

    return boolRet;
}

void Buffer::writeBool(int32_t byteOffset, bool value) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 1 > sharedData->bytesCount) {
        throw BoundsException();
    }

    char finalVal;
    if(value == true) {
        finalVal = 1;
    }else if(value == false) {
        finalVal = 0;
    }else {
        throw DataFormatException();
    }

    writeByte(byteOffset, finalVal);
}

int16_t Buffer::readShort(int32_t byteOffset, bool bigEndian) {
    if(bigEndian) {
        return readShortBE(byteOffset);
    }else {
        return readShortLE(byteOffset);
    }
}

int16_t Buffer::readShortBE(int32_t byteOffset) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 2 > sharedData->bytesCount) {
        throw BoundsException();
    }

    short ret = BitsUtil::getShortB(&static_cast<char*>(sharedData->block)[byteOffset]);

    return ret;
}

int16_t Buffer::readShortLE(int32_t byteOffset) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 2 > sharedData->bytesCount) {
        throw BoundsException();
    }

    short ret = BitsUtil::getShortL(&static_cast<char*>(sharedData->block)[byteOffset]);

    return ret;
}

void Buffer::writeShort(int32_t byteOffset, bool bigEndian, int16_t value) {
    if(bigEndian) {
        writeShortBE(byteOffset, value);
    }else {
        writeShortLE(byteOffset, value);
    }
}

void Buffer::writeShortBE(int32_t byteOffset, int16_t value) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 2 > sharedData->bytesCount) {
        throw BoundsException();
    }

    BitsUtil::putShortB(&static_cast<char*>(sharedData->block)[byteOffset], value);
}

void Buffer::writeShortLE(int32_t byteOffset, int16_t value) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 2 > sharedData->bytesCount) {
        throw BoundsException();
    }

    BitsUtil::putShortL(&static_cast<char*>(sharedData->block)[byteOffset], value);
}

int32_t Buffer::readInt(int32_t byteOffset, bool bigEndian) {
    if(bigEndian) {
        return readIntBE(byteOffset);
    }else {
        return readIntLE(byteOffset);
    }
}

int32_t Buffer::readIntBE(int32_t byteOffset) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 4 > sharedData->bytesCount) {
        throw BoundsException();
    }

    int32_t ret = BitsUtil::getIntB(&static_cast<char*>(sharedData->block)[byteOffset]);

    return ret;
}

int32_t Buffer::readIntLE(int32_t byteOffset) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 4 > sharedData->bytesCount) {
        throw BoundsException();
    }

    int32_t ret = BitsUtil::getIntL(&static_cast<char*>(sharedData->block)[byteOffset]);

    return ret;
}

void Buffer::writeInt(int32_t byteOffset, bool bigEndian, int32_t value) {
    if(bigEndian) {
        writeIntBE(byteOffset, value);
    }else {
        writeIntLE(byteOffset, value);
    }
}

void Buffer::writeIntBE(int32_t byteOffset, int32_t value) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 4 > sharedData->bytesCount) {
        throw BoundsException();
    }

    BitsUtil::putIntB(&static_cast<char*>(sharedData->block)[byteOffset], value);
}

void Buffer::writeIntLE(int32_t byteOffset, int32_t value) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 4 > sharedData->bytesCount) {
        throw BoundsException();
    }

    BitsUtil::putIntL(&static_cast<char*>(sharedData->block)[byteOffset], value);
}

float Buffer::readFloat(int32_t byteOffset, bool bigEndian) {
    if(bigEndian) {
        return readFloatBE(byteOffset);
    }else {
        return readFloatLE(byteOffset);
    }
}

float Buffer::readFloatBE(int32_t byteOffset) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 4 > sharedData->bytesCount) {
        throw BoundsException();
    }

    float ret = BitsUtil::getFloatB(&static_cast<char*>(sharedData->block)[byteOffset]);

    return ret;
}

float Buffer::readFloatLE(int32_t byteOffset) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 4 > sharedData->bytesCount) {
        throw BoundsException();
    }

    float ret = BitsUtil::getFloatL(&static_cast<char*>(sharedData->block)[byteOffset]);

    return ret;
}

void Buffer::writeFloat(int32_t byteOffset, bool bigEndian, const float value) {
    if(bigEndian) {
        writeFloatBE(byteOffset, value);
    }else {
        writeFloatLE(byteOffset, value);
    }
}

void Buffer::writeFloatBE(int32_t byteOffset, const float value) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 4 > sharedData->bytesCount) {
        throw BoundsException();
    }

    BitsUtil::putFloatB(&static_cast<char*>(sharedData->block)[byteOffset], value);
}

void Buffer::writeFloatLE(int32_t byteOffset, const float value) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + 4 > sharedData->bytesCount) {
        throw BoundsException();
    }

    BitsUtil::putFloatL(&static_cast<char*>(sharedData->block)[byteOffset], value);
}

Buffer Buffer::readBuffer(int32_t byteOffset, const int32_t bytesCount) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + bytesCount > this->sharedData->bytesCount) {
        throw BoundsException();
    }

    Buffer ret{ bytesCount };
    // Use a workaround to copy from the block using a offset.
    ret.writeBytes(0, &static_cast<char*>(sharedData->block)[byteOffset], bytesCount);

    return ret;
}

std::shared_ptr<Buffer> Buffer::readBufferSP(int32_t byteOffset, const int32_t bytesCount) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + bytesCount > this->sharedData->bytesCount) {
        throw BoundsException();
    }

    std::shared_ptr<Buffer> ret = std::make_shared<Buffer>(bytesCount);
    // Use a workaround to copy from the block using a offset.
    ret->writeBytes(0, &static_cast<char*>(sharedData->block)[byteOffset], bytesCount);

    return ret;
}

void Buffer::writeBuffer(int32_t byteOffset, Buffer& srcBuffer, int32_t srcBytesOffset, const int32_t bytesCount) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    if(byteOffset + bytesCount > this->sharedData->bytesCount || byteOffset + bytesCount > srcBuffer.sharedData->bytesCount) {
        throw BoundsException();
    }

    writeBytes(byteOffset, &static_cast<char*>(srcBuffer.sharedData->block)[srcBytesOffset], bytesCount);
}

void Buffer::readBytes(int32_t srcByteOffset, void* destBytes, const int32_t bytesCount) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    // Do checks on the src and this to make sure enough data and space exists.
    if (srcByteOffset + bytesCount > this->sharedData->bytesCount) {
        throw BoundsException();
    }

    // Copy the bytes.
    std::memcpy(destBytes, &static_cast<char*>(sharedData->block)[srcByteOffset], bytesCount);
}

void Buffer::writeBytes(int32_t byteOffset, const char* srcBytes, const int32_t bytesCount) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    // Do checks on the src and this to make sure enough data and space exists.
    if(byteOffset + bytesCount > this->sharedData->bytesCount) {
        throw BoundsException();
    }

    // Copy the bytes.
    std::memcpy(&static_cast<char*>(sharedData->block)[byteOffset], srcBytes, static_cast<size_t>(bytesCount));
}

std::string Buffer::readString(int32_t byteOffset) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    short stringBytesLength = readShortBE(byteOffset);

    if(stringBytesLength < 0 || byteOffset + stringBytesLength > sharedData->bytesCount) {
        throw BoundsException();
    }

    std::string ret{&static_cast<char*>(sharedData->block)[byteOffset + 2], static_cast<unsigned long>(stringBytesLength)};

    return ret;
}

void Buffer::writeString(int32_t byteOffset, std::string& value) {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    // std::string::size(); Is the number of bytes (for multicode also), not the number of characters (including multicode).
    short stringBytesLength = static_cast<short>(value.size());

    const char* stringBytes = value.data();

    writeShortBE(byteOffset, stringBytesLength);
    writeBytes(byteOffset + 2, stringBytes, stringBytesLength);
}

Buffer::~Buffer() {
    setSharedData(nullptr);
}
