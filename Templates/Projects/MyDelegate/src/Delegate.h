#pragma once
#include <assert.h>

class Delegate
{
public:
	Delegate() : m_parameters(nullptr) {}
	~Delegate()
	{
		if(nullptr != m_parameters)
		{
			delete m_parameters;
		}
	}

	template<typename C, typename M>
	void Connect(C* c, M m)
	{
		if(nullptr != m_parameters)
		{
			delete m_parameters;
		}

		m_parameters = new Parameters<C, M>(c, m);
	}

	template<typename A>
	void operator()(A arg)
	{
		Arguments<A> a(arg);
		m_parameters->Call(&a);
	}

private:
	struct IArguments { /*virtual ~IArguments() {}*/ };

	template<typename A>
	struct Arguments : public IArguments
	{
		Arguments(A a) : m_arguments(a) {}
		/*~Arguments()
		{
			if(nullptr != m_arguments)
			{
				delete m_arguments;
			}
		}*/

		A m_arguments;
	};

	struct IParametrs { virtual void Call(IArguments*) = 0; };

	template<typename C, typename M>
	struct Parameters : public IParametrs {};

	template<typename C, typename A>
	struct Parameters<C, void(C::*)(A)> : public IParametrs
	{
		Parameters(C* c, void(C::*m)(A)) : m_class(c), m_method(m) {}
		void Call(IArguments* a)
		{
			Arguments<A>* arguments = static_cast<Arguments<A>*>(a);
			assert(nullptr != arguments);
			if(nullptr != arguments)
			{
				(m_class->*m_method)(arguments->m_arguments);
			}
		}

		C* m_class;
		void(C::*m_method)(A);
	};

	IParametrs* m_parameters;
};
