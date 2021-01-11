#pragma once

#include <base/gh.h>
#include <base/exceptions/LogicException.h>
#include <cassert>
#include <cstdint>
#include <base/MM.h>
#include <base/object/IDisposable.h>
#include <base/memory/ISelfSharedPointerContainer.h>

namespace mem {
    
template<class T>
class SharedPointer;

template<typename T, typename std::enable_if<std::is_base_of<mem::ISelfSharedPointerContainer, T>::value>::type* = nullptr>
void initObjSelfWSPIfNeeded(T* thisObjP, SharedPointer<T>* thisSP) noexcept {
    if (!thisObjP->getSelfWSPRef().isWeakPointer) {
        // The selfSP must be a weak shared pointer.
        assert(false);
    }
    thisObjP->getSelfWSPRef() = *thisSP;
}

template<typename T, typename std::enable_if<!std::is_base_of<mem::ISelfSharedPointerContainer, T>::value>::type* = nullptr>
void initObjSelfWSPIfNeeded(T* thisObjP, SharedPointer<T>* thisSP) noexcept {
    //void
}

template<typename T, typename std::enable_if<std::is_base_of<mem::ISelfSharedPointerContainer, T>::value>::type* = nullptr>
void clearObjSelfWSPIfNeeded(T* thisObjP) noexcept {
    if (!thisObjP->getSelfWSPRef().isWeakPointer) {
        // The selfSP must be a weak shared pointer.
        assert(false);
    }
    thisObjP->getSelfWSPRef() = nullptr;
}

template<typename T, typename std::enable_if<!std::is_base_of<mem::ISelfSharedPointerContainer, T>::value>::type* = nullptr>
void clearObjSelfWSPIfNeeded(T* thisObjP) noexcept {
    //void
}

template<typename T, typename std::enable_if<std::is_base_of<base::IDisposable, T>::value>::type* = nullptr>
void callReservedDisposeOnDisposables(T* thisObjP) noexcept {
    thisObjP->reservedDispose();
}

template<typename T, typename std::enable_if<!std::is_base_of<base::IDisposable, T>::value>::type* = nullptr>
void callReservedDisposeOnDisposables(T* thisObjP) noexcept {
    //void
}

namespace detail {
struct SharedDataBase {
    pub void* objVoidP;
    pub unsigned int count = 0;

    pub explicit SharedDataBase(void* objVoidP)
        : objVoidP(objVoidP)
    {};

    pub virtual ~SharedDataBase()
    {
        //void
    };
};

template <class TData>
struct SharedDataTyped : public SharedDataBase {priv typedef SharedDataBase super;
    pub explicit SharedDataTyped(TData* obj)
        : super(obj) 
    {}

    pub ~SharedDataTyped() override {
        // This class is still required for the destructor because the IDisposable::reservedDispose() call must be called from the top class.
        if (objVoidP != nullptr) {
            TData* objP = static_cast<TData*>(objVoidP);
            callReservedDisposeOnDisposables(objP);
            
            clearObjSelfWSPIfNeeded(objP);

            delete objP;
            objVoidP = nullptr;
        }
    };
};
} //namespace detail

class SharedPointer_Stats {
    pub static unsigned int activesCount_spObjects;
    pub static unsigned int functionCallsCount_newSP;
    pub static unsigned int functionCallsCount_deleteSP;
};

template<class T>
class SharedPointer {
    // This means that bar is a friend of foo regardless of bar's template arguments. bar<char>, bar<int>, bar<float>, and any other bar would be friends of foo<char>.
    template<typename F>
    friend class SharedPointer;

    priv detail::SharedDataBase* sharedData;
    // This is implemented as a offset from objVoidP and not as a type pointer because a memory manager with defragmentation will be inplemented and the objVoidP address will change.
    priv int32_t localTypeOffset;
    pub const bool isWeakPointer = false;
    
    pub constexpr SharedPointer(bool isWeakPointer = false) noexcept;
    pub SharedPointer(std::nullptr_t) noexcept;

    // Don't make explicit so it can be called as a optimisation for the example:
    /// sp<RegisteredEntry> registeredEntry = new RegisteredEntry(this, turnBasedEntity);
    pub SharedPointer(T* ptr) noexcept;

    pub SharedPointer(detail::SharedDataBase* sharedData, int32_t localTypeOffset) noexcept;

    // Copy constructor.
    SharedPointer(SharedPointer<T> const& other) noexcept;
    pub template <typename T2, typename std::enable_if<std::is_base_of<T, T2>::value>::type* = nullptr>
    SharedPointer(SharedPointer<T2> const& other) noexcept;
    
    // Move constructor.
    SharedPointer(SharedPointer<T>&& other) noexcept;
    pub template <typename T2, typename std::enable_if<std::is_base_of<T, T2>::value>::type* = nullptr>
    SharedPointer(SharedPointer<T2>&& other) noexcept;

    // Copy operator.
    SharedPointer<T>& operator=(SharedPointer<T> const& other) noexcept;
    pub template <typename T2, typename std::enable_if<std::is_base_of<T, T2>::value>::type* = nullptr>
    SharedPointer<T>& operator=(SharedPointer<T2> const& other) noexcept;

    // Move operator.
    SharedPointer<T>& operator=(SharedPointer<T>&& other) noexcept;
    pub template <typename T2, typename std::enable_if<std::is_base_of<T, T2>::value>::type* = nullptr>
    SharedPointer<T>& operator=(SharedPointer<T2>&& other) noexcept;

    // Move operator with pointer grabing for management.
    pub SharedPointer<T>& operator=(T* ptr) noexcept;

    pub bool operator==(SharedPointer<T> const& other) const noexcept;
    pub bool operator==(std::nullptr_t) const noexcept;

    pub bool operator!=(SharedPointer<T> const& other) const noexcept;
    pub bool operator!=(std::nullptr_t) const noexcept;

    priv void setSharedData(detail::SharedDataBase* newSharedData, int32_t localTypeOffset) noexcept;

    pub unsigned int use_count() noexcept;

    pub T* get() const;

    pub T& operator*();
    pub T* operator->();

    // Note. The enable_if check may seem inverted bu the classes types call is also inverted so it is correct.
    // Note. The is_base_of only checks che class types and not the actual instances (this check is not sufficient to guarantee the types requested is a subclass of this base_class).
    pub template <typename T2, typename std::enable_if<std::is_base_of<T, T2>::value>::type* = nullptr>
    SharedPointer<T2> dCast(bool mustExtend);

    pub ~SharedPointer() noexcept;
};

template <typename T>
constexpr SharedPointer<T>::SharedPointer(bool isWeakPointer) noexcept
    : sharedData(nullptr), localTypeOffset(0), isWeakPointer(isWeakPointer)
{
    //void
}

template <typename T>
SharedPointer<T>::SharedPointer(std::nullptr_t) noexcept
    : sharedData(nullptr), localTypeOffset(0)
{
    //void
}

template <typename T>
SharedPointer<T>::SharedPointer(T* ptr) noexcept
    : sharedData(nullptr), localTypeOffset(0)
{
    if (ptr != nullptr) {
        SharedPointer_Stats::activesCount_spObjects++;
        SharedPointer_Stats::functionCallsCount_newSP++;

        setSharedData(newt detail::SharedDataTyped<T>(ptr), 0/*localTypeOffset*/);

        initObjSelfWSPIfNeeded(ptr, this);
    }
}

template <typename T>
SharedPointer<T>::SharedPointer(detail::SharedDataBase* sharedData, int32_t localTypeOffset) noexcept
    : sharedData(nullptr), localTypeOffset(0)
{
    if (sharedData != nullptr) {
        setSharedData(sharedData, localTypeOffset);
    }
}

// Copy constructor.
template <typename T>
SharedPointer<T>::SharedPointer(SharedPointer<T> const& other) noexcept
    : sharedData(nullptr), localTypeOffset(0)
{
    setSharedData(other.sharedData, other.localTypeOffset);
}


template <typename T>
template <typename T2, typename std::enable_if<std::is_base_of<T, T2>::value>::type*>
SharedPointer<T>::SharedPointer(SharedPointer<T2> const& other) noexcept
    : sharedData(nullptr), localTypeOffset(0)
{
    int32_t localTypeOffset;
    if (other.sharedData == nullptr) {
        localTypeOffset = 0;
    } else {
        T2* otherObjP = static_cast<T2*>(static_cast<void*>(static_cast<char*>(other.sharedData->objVoidP) + other.localTypeOffset));
        T* thisObjP = static_cast<T*>(otherObjP);

        // Grab the offset other has to the top class and add the offset this has to other.
        localTypeOffset = other.localTypeOffset + reinterpret_cast<char*>(thisObjP) - reinterpret_cast<char*>(otherObjP);
    }

    setSharedData(other.sharedData, localTypeOffset);
}

// Move constructor.
template <typename T>
SharedPointer<T>::SharedPointer(SharedPointer<T>&& other) noexcept
    : sharedData(nullptr), localTypeOffset(0)
{
    setSharedData(other.sharedData, other.localTypeOffset);
    other.setSharedData(nullptr, 0/*localTypeOffset*/);
}

template <typename T>
template <typename T2, typename std::enable_if<std::is_base_of<T, T2>::value>::type*>
SharedPointer<T>::SharedPointer(SharedPointer<T2>&& other) noexcept
    : sharedData(nullptr), localTypeOffset(0)
{
    int32_t localTypeOffset;
    if (other.sharedData == nullptr) {
        localTypeOffset = 0;
    } else {
        T2* otherObjP = static_cast<T2*>(static_cast<void*>(static_cast<char*>(other.sharedData->objVoidP) + other.localTypeOffset));
        T* thisObjP = static_cast<T*>(otherObjP);

        // Grab the offset other has to the top class and add the offset this has to other.
        localTypeOffset = other.localTypeOffset + reinterpret_cast<char*>(thisObjP) - reinterpret_cast<char*>(otherObjP);
    }

    setSharedData(other.sharedData, localTypeOffset);
    other.setSharedData(nullptr, 0/*localTypeOffset*/);
}

// Copy operator.
template <typename T>
SharedPointer<T>& SharedPointer<T>::operator=(SharedPointer<T> const& other) noexcept {
    setSharedData(other.sharedData, other.localTypeOffset);

    return *this;
}

template <typename T>
template <typename T2, typename std::enable_if<std::is_base_of<T, T2>::value>::type*>
SharedPointer<T>& SharedPointer<T>::operator=(SharedPointer<T2> const& other) noexcept {
    int32_t localTypeOffset;
    if (other.sharedData == nullptr) {
        localTypeOffset = 0;
    } else {
        T2* otherObjP = static_cast<T2*>(static_cast<void*>(static_cast<char*>(other.sharedData->objVoidP) + other.localTypeOffset));
        T* thisObjP = static_cast<T*>(otherObjP);

        // Grab the offset other has to the top class and add the offset this has to other.
        localTypeOffset = other.localTypeOffset + reinterpret_cast<char*>(thisObjP) - reinterpret_cast<char*>(otherObjP);
    }

    setSharedData(other.sharedData, localTypeOffset);

    return *this;
}

// Move operator.
template <typename T>
SharedPointer<T>& SharedPointer<T>::operator=(SharedPointer<T>&& other) noexcept {
    setSharedData(other.sharedData, other.localTypeOffset);
    other.setSharedData(nullptr, 0/*localTypeOffset*/);

    return *this;
}


template <typename T>
template <typename T2, typename std::enable_if<std::is_base_of<T, T2>::value>::type*>
SharedPointer<T>& SharedPointer<T>::operator=(SharedPointer<T2>&& other) noexcept {
    int32_t localTypeOffset;
    if (other.sharedData == nullptr) {
        localTypeOffset = 0;
    } else {
        T2* otherObjP = static_cast<T2*>(static_cast<void*>(static_cast<char*>(other.sharedData->objVoidP) + other.localTypeOffset));
        T* thisObjP = static_cast<T*>(otherObjP);

        // Grab the offset other has to the top class and add the offset this has to other.
        localTypeOffset = other.localTypeOffset + reinterpret_cast<char*>(thisObjP) - reinterpret_cast<char*>(otherObjP);
    }

    setSharedData(other.sharedData, localTypeOffset);
    other.setSharedData(nullptr, 0/*localTypeOffset*/);

    return *this;
}

template <typename T>
SharedPointer<T>& SharedPointer<T>::operator=(T* ptr) noexcept {
    if (ptr != nullptr) {
        setSharedData(newt detail::SharedDataTyped<T>(ptr), 0/*localTypeOffset*/);

        initObjSelfWSPIfNeeded(ptr, this);
    } else {
        setSharedData(nullptr, 0/*localTypeOffset*/);
    }

    return *this;
}

template <typename T>
bool SharedPointer<T>::operator==(SharedPointer<T> const& other) const noexcept {
    return sharedData == other.sharedData;
}

template <typename T>
bool SharedPointer<T>::operator==(std::nullptr_t) const noexcept {
    return sharedData == nullptr;
}

template <typename T>
bool SharedPointer<T>::operator!=(SharedPointer<T> const& other) const noexcept {
    return sharedData != other.sharedData;
}

template <typename T>
bool SharedPointer<T>::operator!=(std::nullptr_t) const noexcept {
    return sharedData != nullptr;
}

template <typename T>
void SharedPointer<T>::setSharedData(detail::SharedDataBase* newSharedData, int32_t localTypeOffset) noexcept {
    if (this->sharedData != nullptr) {
        if (!isWeakPointer) {
            this->sharedData->count--;

            assert(this->sharedData->count >= 0);
            if (this->sharedData->count == 0) {
                SharedPointer_Stats::activesCount_spObjects--;
                SharedPointer_Stats::functionCallsCount_deleteSP++;

                delete this->sharedData;
            }
        }

        this->localTypeOffset = localTypeOffset;
        this->sharedData = nullptr;
    }

    if (newSharedData != nullptr) {
        this->sharedData = newSharedData;
        this->localTypeOffset = localTypeOffset;

        if (!isWeakPointer) {
            this->sharedData->count++;
        } else {
            if (this->sharedData->count == 0) {
                // Not allowed to initialise a weak pointer when there aren't any shared pointers to manage the data.
                assert(false);
            }
        }
    }
}

template <typename T>
unsigned int SharedPointer<T>::use_count() noexcept {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    return sharedData->count;
}

template <typename T>
T* SharedPointer<T>::get() const {
    if (sharedData == nullptr) {
        return nullptr;
    }

    return static_cast<T*>(static_cast<void*>(static_cast<char*>(sharedData->objVoidP) + localTypeOffset));
}

template <typename T>
T& SharedPointer<T>::operator*() {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    return *static_cast<T*>(static_cast<void*>(static_cast<char*>(sharedData->objVoidP) + localTypeOffset));
}

template <typename T>
T* SharedPointer<T>::operator->() {
    if (sharedData == nullptr) {
        throw LogicException(LOC);
    }

    return static_cast<T*>(static_cast<void*>(static_cast<char*>(sharedData->objVoidP) + localTypeOffset));
}

template <typename T>
// Note. The enable_if check may seem inverted bu the classes types call is also inverted so it is correct.
// Note. The is_base_of only checks che class types and not the actual instances (this check is not sufficient to guarantee the types requested is a subclass of this base_class).
template <typename T2, typename std::enable_if<std::is_base_of<T, T2>::value>::type*>
SharedPointer<T2> SharedPointer<T>::dCast(bool mustExtend) {
    if (sharedData == nullptr) {
        return nullptr;
    }
    
    T* thisObjP = reinterpret_cast<T*>(static_cast<char*>(sharedData->objVoidP) + localTypeOffset);

    T2* targetObjP = dynamic_cast<T2*>(thisObjP);
    if (targetObjP == nullptr) {
        if (mustExtend) {
            throw LogicException(LOC);
        }

        return nullptr;
    }

    // Grab the offset other has to the top class and add the offset this has to other.
    // Note. ewLocalTypeOffset should be less that localTypeOffset.
    int32_t ewLocalTypeOffset = localTypeOffset + reinterpret_cast<char*>(targetObjP) - reinterpret_cast<char*>(thisObjP);
    if (ewLocalTypeOffset < 0) {
        // Bug occured, this should never be negatice (in front of the actual memory).
        throw LogicException(LOC);
    }

    return SharedPointer<T2>{sharedData, ewLocalTypeOffset};
}

template <typename T>
SharedPointer<T>::~SharedPointer() noexcept {
    setSharedData(nullptr, 0/*localTypeOffset*/);
}

template <typename T, typename... Args>
SharedPointer<T> MakeSharedPointer(Args&&... args) {
    T* val = new T(std::forward<Args>(args)...);
    SharedPointer<T> sp(val);

    return sp;
}

};

template <typename T, typename... Args>
mem::SharedPointer<T> msp(Args&&... args) {
    T* val = new T(std::forward<Args>(args)...);
    mem::SharedPointer<T> sp(val);

    return sp;
}

template<typename T>
using sp = mem::SharedPointer<T>;
