#pragma once

#include <map>
#include <memory>


template<typename Base>
class GenericFactory
{
	struct BaseType
	{
		virtual ~BaseType() {}
		virtual std::shared_ptr<Base> Create() const = 0;
	};

	template<typename Derived>
	struct DerivedType : public BaseType
	{
		std::shared_ptr<Base> Create() const override final
		{
			return std::static_pointer_cast<Base>(std::make_shared<Derived>());
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
	}
	
	std::shared_ptr<Base> Create(std::wstring const& name)
	{
		return m_Factories[name]->Create();
	}
};
