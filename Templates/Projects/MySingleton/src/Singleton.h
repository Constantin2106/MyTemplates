#ifndef SINGLETON
#define SINGLETON

#include <string>

namespace Singleton
{
    class TSingleton
    {
    public:
        static TSingleton* Instance();
        static bool Destroy(TSingleton* singleton);
        ~TSingleton() {}

        bool Initialize();
        bool Free();
        std::string GetClassName() { return className; }
        int GetSize() { return size; }

    private:
        static TSingleton* instance;
        const std::string className = "Singleton";

        int size;
        int* m_data;

        TSingleton() : m_data(nullptr), size(0) {}
        TSingleton(const TSingleton&) = delete;
        TSingleton& operator = (const TSingleton&) = delete;

    };

    class BaseSingleton
    {
        std::string m_class_name{ "BaseSingleton" };

    public:
        static BaseSingleton Instance()
        {
            static BaseSingleton instance;
            return instance;
        }

        std::string ClassName() { return m_class_name; }
    };

    template<typename T>
    T& CreateSingleton()
    {
        static T instance;
        return instance;
    }

    class Singleton_1
    {
        std::string m_class_name{ "Singleton_1" };

    public:
        static Singleton_1 Instance()
        {
            return CreateSingleton<Singleton_1>();
        }

        std::string ClassName() { return m_class_name; }
    };

    class Singleton_2
    {
        std::string m_class_name{ "Singleton_2" };

    public:
        static Singleton_2 Instance()
        {
            return CreateSingleton<Singleton_2>();
        }

        std::string ClassName() { return m_class_name; }
    };

#define Singleton(T) T::Instance();
}
#endif
