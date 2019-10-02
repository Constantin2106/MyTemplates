#pragma once

#include <map>
#include <memory>

template<typename Base>
class GenericFactory
{
	class BaseType
	{
	public:
		virtual ~BaseType() {}
		virtual std::shared_ptr<Base> Create() const = 0;
	};

	template<typename Derived>
	class DerivedType : public BaseType
	{
	public:
		std::shared_ptr<Base> Create() const override final
		{
			return std::static_pointer_cast<Base>(std::make_shared<Derived>());
			//return std::shared_ptr<Base>(new Derived());
		}
	};

	std::map<std::wstring, std::shared_ptr<BaseType>> m_Factories;

public:

	template<typename Derived>
	bool Register(std::wstring const& name)
	{
		if (m_Factories.find(name) != m_Factories.end())
			return false;
			
		m_Factories[name] = std::static_pointer_cast<BaseType>(std::make_shared<DerivedType<Derived>>());
		return true;
		//m_Factories[name] = std::shared_ptr<BaseType>(new DerivedType<Derived>());
	}
	
	std::shared_ptr<Base> Create(std::wstring const& name)
	{
		return m_Factories[name]->Create();
	}
};

