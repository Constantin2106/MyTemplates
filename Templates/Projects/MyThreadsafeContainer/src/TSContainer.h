#pragma once

#include <memory>
#include <mutex>


template<typename T, typename Mutex = std::recursive_mutex>
class TSContainer
{
    Mutex mMutex;
    std::unique_ptr<T> mContainer;

    class SubClass
    {
        Mutex& mMtx;
        T* const mContainerPtr;

    public:
        SubClass(std::unique_ptr<T>& c, Mutex& m) : mContainerPtr(c.get()), mMtx(m)
        {
            mMtx.lock();
#ifdef _DEBUG
            std::cout << "\tLocked in thread = " << std::this_thread::get_id();
            std::cout << "\tSize = " << mContainerPtr->size() << std::endl;
#endif
        }
        ~SubClass()
        {
#ifdef _DEBUG
            std::cout << "\tUnlocked in thread = " << std::this_thread::get_id();
            std::cout << "\tSize = " << mContainerPtr->size() << std::endl << std::endl;
#endif
            mMtx.unlock();
        }

        T* operator->()
        {
            return mContainerPtr;
        }
        const T* operator->() const
        {
            return mContainerPtr;
        }
    };

public:
    TSContainer() : mContainer(std::make_unique<T>()) {}

    template<typename... Args>
    TSContainer(Args... args) : mContainer(std::make_unique<T>(args...)) {}
    ~TSContainer() {}

    auto operator->()
    {
        return SubClass(mContainer, mMutex);
    }
    const auto operator->() const
    {
        return SubClass(mContainer, mMutex);
    }

    auto& values()
    {
        return *mContainer.get();
    }
    const auto& values() const
    {
        return *mContainer.get();
    }
};
