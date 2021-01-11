#pragma once

#include <stdint.h>

namespace base {
class BitsUtil {
public:
    static short getShort(char* bb, bool bigEndian);
    static void putShort(char* bb, short value, bool bigEndian);

    static int32_t getInt(const char* buf, bool bigEndian);
    static void putInt(char* buf, int32_t value, bool bigEndian);

    static float getFloat(const char* buf, bool bigEndian);
    static void putFloat(char* buf, float value, bool bigEndian);

    static short getShortL(char* buf);
    static short getShortB(char* buf);

    static void putShortL(char* bb, short value);
    static void putShortB(char* bb, short value);

    static short makeShort(char u0, char u1);

    static char short1(short value);
    static char short0(short value);

    static int32_t getIntL(const char* buf);
    static int32_t getIntB(const char* buf);

    static void putIntL(char* buf, int32_t value);
    static void putIntB(char* buf, int32_t value);

    static int32_t makeInt(unsigned char u0, unsigned char u1, unsigned char u2, unsigned char u3);

    static float getFloatB(const char* buf);
    static float getFloatL(const char* buf);

    static void putFloatB(char* buf, float value);
    static void putFloatL(char* buf, float value);

};
};
