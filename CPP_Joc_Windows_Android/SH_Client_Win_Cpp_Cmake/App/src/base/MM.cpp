#include "MM.h"
#include <base/memory/MemoryManager.h>
#include <base/exceptions/LogicException.h>

using namespace mem;

IMemoryManager* MM::mm = nullptr;

void* operator new(std::size_t bytesCount) {
    if (bytesCount > 100000000) {
        // Probably an error occured.
        throw LogicException(LOC);
    }
    if (MM::mm == nullptr) {
        MemoryManager* memoryManager = new (std::malloc(sizeof(MemoryManager))) MemoryManager();
        MM::mm = memoryManager;
    }

    void* ptr = mem::MM::mm->malloc(bytesCount, nullptr, 0, IMemoryManager::AllocationType::Object_Single);

    return ptr;
}

void* operator new(std::size_t bytesCount, const char* fileName, size_t lineIndex) {
    if (bytesCount > 100000000) {
        // Probably an error occured.
        throw LogicException(LOC);
    }
    if (mem::MM::mm == nullptr) {
        mem::MemoryManager* memoryManager = new (std::malloc(sizeof(mem::MemoryManager))) mem::MemoryManager();
        mem::MM::mm = memoryManager;
    }

    void* ptr = mem::MM::mm->malloc(bytesCount, fileName, lineIndex, IMemoryManager::AllocationType::Object_Single);

    return ptr;
}

void* operator new[](std::size_t bytesCount) {
    if (MM::mm == nullptr) {
        MemoryManager* memoryManager = new (std::malloc(sizeof(MemoryManager))) MemoryManager();
        MM::mm = memoryManager;
    }

    void* ptr = mem::MM::mm->malloc(bytesCount, nullptr, 0, IMemoryManager::AllocationType::Object_Array);

    return ptr;
}

void* operator new[](std::size_t bytesCount, const char* fileName, size_t lineIndex) {
    if (mem::MM::mm == nullptr) {
        mem::MemoryManager* memoryManager = new (std::malloc(sizeof(mem::MemoryManager))) mem::MemoryManager();
        mem::MM::mm = memoryManager;
    }

    void* ptr = mem::MM::mm->malloc(bytesCount, fileName, lineIndex, IMemoryManager::AllocationType::Object_Array);

    return ptr;
}

void operator delete(void* ptr) noexcept {
    mem::MM::mm->free(ptr, IMemoryManager::AllocationType::Object_Single);
}

void operator delete[](void* ptr) noexcept {
    mem::MM::mm->free(ptr, IMemoryManager::AllocationType::Object_Array);
}
