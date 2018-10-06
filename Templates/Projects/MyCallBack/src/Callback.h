#pragma once


//#include <type_traits>

template<class SignatureFunc> class FuncRef;
template<class R, class... Ts>
class FuncRef<R(Ts...)>
{
public:
    FuncRef() : m_func(nullptr) {}
    FuncRef(R(*f)(Ts...)) : m_func(f) {}

    ~FuncRef() {}

    R operator() (Ts... arg) const
    {
        return m_func(std::forward<Ts>(arg)...);
    }

private:
    R(*m_func)(Ts... arg);

    /*FuncRef(const FuncRef&) = delete;
    FuncRef& operator= (const FuncRef&) = delete;
    FuncRef(FuncRef&&) = delete;
    FuncRef& operator= (FuncRef&&) = delete;*/
};


template<class SignatureLambda> class LambdaRef;
template<class R, class... Ts>
class LambdaRef<R(Ts...)>
{
public:
    template<class Fn>
    LambdaRef(Fn&& func)
    {
        m_context = reinterpret_cast<void*>(&func);
        m_caller = &ObjectCaller<std::decay_t<Fn>>;
    }

    R operator () (Ts... args) const
    {
        return (*m_caller)(m_context, std::forward<Ts>(args)...);
    }

private:
    void*    m_context;
    R(*m_caller)(void*, Ts&&... args);

    template<class Fn>
    static R ObjectCaller(void* object, Ts&&... args)
    {
        return (*reinterpret_cast<Fn*>(object))(std::forward<Ts>(args)...);
    }

    LambdaRef(const LambdaRef&) = delete;
    LambdaRef& operator= (const LambdaRef&) = delete;
    LambdaRef(LambdaRef&&) = delete;
    LambdaRef& operator= (LambdaRef&&) = delete;
};
