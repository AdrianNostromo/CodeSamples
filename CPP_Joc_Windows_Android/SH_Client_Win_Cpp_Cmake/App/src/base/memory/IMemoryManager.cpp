#include "IMemoryManager.h"

using namespace mem;

const char IMemoryManager::AllocationType::Void_Pointer = 1;
const char IMemoryManager::AllocationType::Object_Single = 2;
const char IMemoryManager::AllocationType::Object_Array = 3;
