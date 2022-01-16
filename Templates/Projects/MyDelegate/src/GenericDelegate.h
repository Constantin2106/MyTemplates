#pragma once
//#define DG_NO_EXCEPTIONS

#include <utility>
#include <functional>

namespace GenericDelegate
{
    #define GD_SUBST(Arg)                           Arg
    
    #define GD_BIND_DISAMBIGUATE2(has_args, ...)    GD_SUBST(GD_BIND_ ## has_args (__VA_ARGS__))
    #define GD_BIND_DISAMBIGUATE(has_args, ...)     GD_BIND_DISAMBIGUATE2(has_args, __VA_ARGS__)
    
    #define GD_HAS_ARGS_IMPL(TWO, ONE, N, ...)      N
    #define GD_HAS_ARGS(...)                        GD_SUBST(GD_HAS_ARGS_IMPL(__VA_ARGS__, 2, 1, ERROR))
    
    #define GD_BIND_1(fp)                           GenericDelegate::bind<decltype(fp), fp>()
    #define GD_BIND_2(mf, ip)                       GenericDelegate::bind<decltype(mf), mf>(ip)
    
    #define GD_BIND(...)                            GD_BIND_DISAMBIGUATE(GD_HAS_ARGS(__VA_ARGS__), __VA_ARGS__)

    template <typename Signature>
    class GDelegate;

    template <typename R, typename ...Args>
    class GDelegate<R(Args...)>
    {
        using Func = R(*)(void const*, Args&& ...);

        Func mFunction{};
        void const* mInstance{};

    public:
        GDelegate() : mFunction(nullptr), mInstance(nullptr) {}
        GDelegate(Func func, void const* inst) : mFunction(func), mInstance(inst) {}

        R operator() (Args... args) const
        {
#ifndef DG_NO_EXCEPTIONS
            if (!mFunction)
                throw std::bad_function_call();
#endif // DG_NO_EXCEPTIONS
            return (*mFunction)(mInstance, std::forward<Args>(args)...);
        }

        explicit operator bool() const
        {
            return nullptr != mFunction;
        }

        bool operator==(GDelegate const& other) const
        {
            return mFunction == other.mFunction && mInstance == other.mInstance;
        }

        bool operator!=(GDelegate const& other) const
        {
            return !(*this == other);
        }
    };

    template <typename Signature>
    struct function_traits;

    template <typename R, typename ...Args>
    struct function_traits<R(*)(Args...)>
    {
        //typedef R (*Signature)(Args...); // MS error C3522: parameter pack cannot be expanded in this context
        using function_type = GDelegate<R(Args...)>;

        template <typename Signature, Signature ff>
        static R wrapper(void const*, Args&& ... args)
        {
            return (*ff)(std::forward<Args>(args)...);
        }
    };

    template <typename R, typename Class, typename ...Args>
    struct function_traits<R(Class::*)(Args...)>
    {
        //typedef R (Class::*Signature)(Args...); // MS error C3522: parameter pack cannot be expanded in this context
        using class_ptr = Class*;
        using function_type = GDelegate<R(Args...)>;

        template <typename Signature, Signature mf>
        static R wrapper(const void* ip, Args&& ... args)
        {
            Class* instance = const_cast<Class*>(static_cast<Class const*>(ip));
            return (instance->*mf)(std::forward<Args>(args)...);
        }
    };

    template <typename R, typename Class, typename ...Args>
    struct function_traits<R(Class::*)(Args...) const>
    {
        //typedef R (Class::*Signature)(Args...) const; // MS error C3522: parameter pack cannot be expanded in this context
        using class_ptr = const Class*;
        using function_type = GDelegate<R(Args...)>;

        template <typename Signature, Signature mf>
        static R wrapper(void const* ip, Args&& ... args)
        {
            Class const* instance = static_cast<Class const*>(ip);
            return (instance->*mf)(std::forward<Args>(args)...);
        }
    };

    // Bind free function
    template <typename Signature, Signature ff>
    typename function_traits<Signature>::function_type bind()
    {
        using traits = function_traits<Signature>;
        return typename traits::function_type(&traits::template wrapper<Signature, ff>, 0);
    }
    // Bind member function
    template <typename Signature, Signature mf>
    typename function_traits<Signature>::function_type bind(typename function_traits<Signature>::class_ptr ip)
    {
        using traits = function_traits<Signature>;
        return typename traits::function_type(&traits::template wrapper<Signature, mf>, ip);
    }
}
