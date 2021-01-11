#include "NativeConsole.h"
#include <cstdio>

using namespace desktopWindows;

NativeConsole::NativeConsole() {
    //void
}

void NativeConsole::post_verbose(const char* fmt, va_list ap) {
    vprintf(fmt, ap);
    printf("\n");
}

void NativeConsole::post_debug(const char* fmt, va_list ap) {
    vprintf(fmt, ap);
    printf("\n");
}

void NativeConsole::post_info(const char* fmt, va_list ap) {
    vprintf(fmt, ap);
    printf("\n");
}

void NativeConsole::post_warn(const char* fmt, va_list ap) {
    vprintf(fmt, ap);
    printf("\n");
}

void NativeConsole::post_error(const char* fmt, va_list ap) {
    vprintf(fmt, ap);
    printf("\n");
}

NativeConsole::~NativeConsole() {
    //void
}
