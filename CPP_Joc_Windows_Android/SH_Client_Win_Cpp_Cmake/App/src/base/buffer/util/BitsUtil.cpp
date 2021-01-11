#include "BitsUtil.h"

using namespace base;

short BitsUtil::getShort(char* bb, bool bigEndian) {
    return bigEndian ? getShortB(bb) : getShortL(bb);
}

void BitsUtil::putShort(char* bb, short value, bool bigEndian) {
    if (bigEndian) {
        putShortB(bb, value);
    } else {
        putShortL(bb, value);
    }
}

short BitsUtil::getShortL(char* buf) {
    return makeShort((unsigned char)(buf[1]),
                   (unsigned char)(buf[0]));
}

short BitsUtil::getShortB(char* buf) {
    return makeShort((unsigned char)(buf[0]),
                     (unsigned char)(buf[1]));
}

void BitsUtil::putShortL(char* bb, short value) {
    bb[0] = short0(value);
    bb[1] = short1(value);
}

void BitsUtil::putShortB(char* bb, short value) {
    bb[0] = short1(value);
    bb[1] = short0(value);
}

short BitsUtil::makeShort(char u0, char u1) {
    // This prevents buf[i] from being promoted to a signed int.
    int16_t uval = int16_t(
        u0 << 8 |
        u1
    );

    return uval;
}

char BitsUtil::short1(short value) {
    return static_cast<signed char>(value >> 8);
}

char BitsUtil::short0(short value) {
    return static_cast<signed char>(value     );
}

int32_t BitsUtil::getInt(const char* buf, bool bigEndian) {
    return bigEndian ? getIntB(buf) : getIntL(buf);
}

void BitsUtil::putInt(char* buf, int32_t value, bool bigEndian) {
    if (bigEndian) {
        putIntB(buf, value);
    } else {
        putIntL(buf, value);
    }
}

int32_t BitsUtil::getIntB(const char* buf) {
    return makeInt((unsigned char)(buf[0]),
                   (unsigned char)(buf[1]),
                   (unsigned char)(buf[2]),
                   (unsigned char)(buf[3]));
}

int32_t BitsUtil::getIntL(const char* buf) {
    return makeInt((unsigned char)(buf[3]),
                   (unsigned char)(buf[2]),
                   (unsigned char)(buf[1]),
                   (unsigned char)(buf[0]));
}

void BitsUtil::putIntL(char* buf, int32_t value) {
    uint32_t uval = static_cast<uint32_t>(value);
    buf[0] = static_cast<signed char>(uval);
    buf[1] = static_cast<signed char>(uval >> 8);
    buf[2] = static_cast<signed char>(uval >> 16);
    buf[3] = static_cast<signed char>(uval >> 24);
}

void BitsUtil::putIntB(char* buf, int32_t value) {
    uint32_t uval = static_cast<uint32_t>(value);
    buf[0] = static_cast<signed char>(uval >> 24);
    buf[1] = static_cast<signed char>(uval >> 16);
    buf[2] = static_cast<signed char>(uval >> 8);
    buf[3] = static_cast<signed char>(uval);
}

int32_t BitsUtil::makeInt(unsigned char u0, unsigned char u1, unsigned char u2, unsigned char u3) {
    // This prevents buf[i] from being promoted to a signed int.
    int32_t uval = int32_t(
        u0 << 24 |
        u1 << 16 |
        u2 << 8 |
        u3
    );

    return uval;
}

float BitsUtil::getFloat(const char* buf, bool bigEndian) {
    return bigEndian ? getFloatB(buf) : getFloatL(buf);
}

void BitsUtil::putFloat(char* buf, float value, bool bigEndian) {
    if (bigEndian) {
        putFloatB(buf, value);
    } else {
        putFloatL(buf, value);
    }
}

float BitsUtil::getFloatB(const char* buf) {
    //unsigned char b[4] {
    //    static_cast<unsigned char>(buf[0]),
    //    static_cast<unsigned char>(buf[1]),
    //    static_cast<unsigned char>(buf[2]),
    //    static_cast<unsigned char>(buf[3])
    //};

    float val = reinterpret_cast<float &&>(buf);

    return val;
}

float BitsUtil::getFloatL(const char* buf) {
    unsigned char b[4] {
        static_cast<unsigned char>(buf[3]),
        static_cast<unsigned char>(buf[2]),
        static_cast<unsigned char>(buf[1]),
        static_cast<unsigned char>(buf[0])
    };

    float val = reinterpret_cast<float &&>(b);

    return val;
}

void BitsUtil::putFloatB(char* buf, float value) {
    unsigned char const * p = reinterpret_cast<unsigned char const *>(&value);

    buf[0] = p[0];
    buf[1] = p[1];
    buf[2] = p[2];
    buf[3] = p[3];
}

void BitsUtil::putFloatL(char* buf, float value) {
    unsigned char const * p = reinterpret_cast<unsigned char const *>(&value);

    buf[0] = p[3];
    buf[1] = p[2];
    buf[2] = p[1];
    buf[3] = p[0];
}
