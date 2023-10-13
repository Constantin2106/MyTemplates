#pragma once

#include <iostream>
#include <memory>
#include <mutex>
//#include <atomic>
//#include <type_traits>
#include <thread>
#include <chrono>

namespace Singleton
{
    template <typename T, template <typename> class SmartPtr = std::shared_ptr>
    class TemplateSingleton 
    {
        TemplateSingleton() = delete;
        ~TemplateSingleton() = delete;
        TemplateSingleton(const TemplateSingleton&) = delete;
        TemplateSingleton& operator=(const TemplateSingleton&) = delete;

    public:
        static auto Instance() 
        {
            if constexpr (std::is_same_v<SmartPtr<T>, std::shared_ptr<T>>) 
            {
                static SmartPtr<T> sharedInstance = std::make_shared<T>();
#ifdef _DEBUG
                std::cout << "Shared instance of MyClass has been created. Address " 
                    << std::hex << sharedInstance.get() 
                    << ". ThreadId " << std::this_thread::get_id() << std::endl << std::flush;
                std::cout.clear();
                this_thread::sleep_for(std::chrono::milliseconds(100));
#endif // _DEBUG
                return sharedInstance;
            }
            else if constexpr (std::is_same_v<SmartPtr<T>, std::unique_ptr<T>>) 
            {
                static SmartPtr<T> uniqueInstance = nullptr;
                static std::mutex mutex;
                if (!uniqueInstance) 
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    if (!uniqueInstance) 
                    {
                        uniqueInstance = std::make_unique<T>();
#ifdef _DEBUG
                        std::cout << "Unique instance of MyClass has been created. Address " 
                            << std::hex << uniqueInstance.get() 
                            << ". ThreadId " << std::this_thread::get_id() << std::endl << std::flush;
                        std::cout.clear();
                        this_thread::sleep_for(std::chrono::milliseconds(100));
#endif // _DEBUG
                    }
                }

                return std::move(uniqueInstance);
            }
        }
    };

    // Example usage:
    class MyClass 
    {
        std::mutex mMutex;
    public:
        MyClass()
        {
            std::lock_guard<std::mutex> lock(mMutex);
#ifdef _DEBUG
            std::cout << "MyClass has been created. Address " 
                << std::hex << this 
                << ". ThreadId " << std::this_thread::get_id() << std::endl << std::flush;
            std::cout.clear();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif // _DEBUG
        }
        ~MyClass()
        {
            std::lock_guard<std::mutex> lock(mMutex);
#ifdef _DEBUG
            std::cout << "MyClass is being deleted. Address " 
                << std::hex << this 
                << ". ThreadId " << std::this_thread::get_id() << std::endl << std::flush;
            std::cout.clear();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif // _DEBUG
        }
        void doSomething() 
        {
            std::lock_guard<std::mutex> lock(mMutex);
#ifdef _DEBUG
            std::cout << "MyClass is doing something. Address " 
                << std::hex << this 
                << ". ThreadId " << std::this_thread::get_id() << std::endl << std::flush;
            std::cout.clear();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif // _DEBUG
        }
    };
}