#pragma once
#include <memory>
#include <mutex>
#include <assert.h>

namespace Singleton
{
	template <typename T>
	class CSingleton
	{
		static std::once_flag     m_onceFlag;
		static std::unique_ptr<T> m_instance;

	public:
		static T& Instance();

		virtual ~CSingleton(void){}

	protected:
		CSingleton(){}
	};

	template <typename T>
	std::unique_ptr<T> CSingleton<T>::m_instance;

	template <typename T>
	std::once_flag CSingleton<T>::m_onceFlag;

	template <typename T>
	T& CSingleton<T>::Instance()
	{
		std::call_once(m_onceFlag, [] 
						{
							assert(!m_instance);
							m_instance.reset(new T);
						});

		assert(m_instance);

		return *m_instance;
	}

	class CDerivedSingleton : public CSingleton<CDerivedSingleton>
	{
		friend CSingleton<CDerivedSingleton>;
	}
	
	//auto Instance = CGatesAlertConsumer::Instance();
}