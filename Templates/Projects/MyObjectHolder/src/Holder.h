#pragma once
#include <assert.h>
#include <memory>

//---------------------------------------------------------------------------------------//
#pragma region Holder of Singleton Object
/*
template<typename T>
class CSingletonHolder
{
	T* getObject() const
	{
		const_cast<CSingletonHolder*>(this)->init();
		assert(m_object);
		return m_object;
	}

	T* m_object = nullptr;

public:
	CSingletonHolder() 
	{ clear(); }

	T* operator->() 
	{ return getObject(); }

	const T* operator->() const 
	{ return getObject(); }

	void operator=(T* object) 
	{ m_object = object; }

	bool isEmpty() const 
	{ return m_object == nullptr; }

	void clear() 
	{ m_object = nullptr; }

	void init() 
	{ 
		if (isEmpty()) 
			reinit(); 
	}
	
	void reinit() 
	{ setObject(*this); }
};
template<typename T>
void setObject(CSingletonHolder<T>& holder)
{
	_CRT_UNUSED(holder);
	using namespace std;

	throw runtime_error(
		string("Interface implementation for ")
		+ typeid(T).name()
		+ string(" is absent.")
	);
}
*/
#pragma endregion

//---------------------------------------------------------------------------------------//
#pragma region Holder of any object (TO) with interface (TI)

// This class is an interface TI wrapper.
// The object type TO is necessary here only 
// to define whose setObject function must be invoked.

template<typename TI, typename TO>
class CHolder
{
	// Wrapped object
	std::shared_ptr<TI> m_object{};
	// Wrapped object class name
	std::string m_typeName;
	bool m_set = false;

protected:
	TI* getObject()
	{
		using namespace std;
		// This check makes usage the operator=(...) in client code senseless
		// because here the object will be reset to a new one
		// even though it has already been set before by code 
		// like CHolder<...> obj; obj = std::make_shared<...>();
		if (!m_set)
		{
			setObject(*this);
			assert(m_object);
			m_set = true;
		}
		return m_object.get();
	}

	virtual void throwException(std::string msg)
	{
		throw std::runtime_error(m_typeName.append(msg));
	}

public:
	CHolder()						{ m_typeName = std::string("[") + typeid(TO).name() + "] "; }
	void Clear()					{ m_object.reset(); m_set = false; }
	std::string GetType()			{ return m_typeName;}
	decltype(auto) operator->()		{ return getObject(); }

	// Can be invoked from the client code
	// but it has no sense because the first
	// operator->(...) usage will reset the object.
	// Needs only for usage in the setObject function.
	void operator=(std::shared_ptr<TI>&& object)
	{
		m_object = std::move(object);
	}
	void operator=(const std::shared_ptr<TI>& object) = delete;

	CHolder<TI, TO>& operator=(const CHolder<TI, TO>& object) = delete;
	CHolder<TI, TO>& operator=(CHolder<TI, TO>&& object) = delete;
	CHolder(const std::shared_ptr<TI>& object) = delete;
	CHolder(std::shared_ptr<TI>&& object) = delete;

};
template<typename TI, typename TO>
void setObject(CHolder<TI, TO>& holder)
{
	using namespace std;
	throw runtime_error( 
		holder.GetType()
		+ string("Implementation of setting function is absent.")
	);
}

#pragma endregion
