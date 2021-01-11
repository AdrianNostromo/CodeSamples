#pragma once

#include <cstddef>
#include <base/memory/IMemoryManager.h>
#include <base/DevUtils.h>

// The default new, delete operators don't need definitions here but are added for completeness because the extra_params version do require definitions here.
//void* operator new(std::size_t bytesCount);
//void* operator new[](std::size_t bytesCount);
void* operator new(std::size_t bytesCount, const char* fileName, size_t lineIndex);
void* operator new[](std::size_t bytesCount, const char* fileName, size_t lineIndex);

//void operator delete(void* ptr) noexcept;
//void operator delete[](void* ptr) noexcept;

// The reason the macro name (newt) is different because this header file must be included for te macro to work. Use find,replace to find untracked calls.
#define newt new(__FILE_NAME__,__LINE__)

// Don't use a mallocnt (not tracked) because that would call the std function and that is not desired.
// The reason the macro name (malloct) is different because this header file must be included for te macro to work. Use find,replace to find untracked calls.
#define malloct(byteCount) mem::MM::mm->malloc(byteCount, __FILE_NAME__, __LINE__)
// These are here so the correct versions are called.
#define realloct(block, byteCount) mem::MM::mm->realloc(block, byteCount, __FILE_NAME__, __LINE__)
#define freet(block) mem::MM::mm->free(block)

namespace mem {
class MM {
public:
    static IMemoryManager* mm;

};
};
