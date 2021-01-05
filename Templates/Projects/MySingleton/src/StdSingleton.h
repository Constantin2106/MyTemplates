#pragma once

#include <memory>
#include <mutex>
#include <assert.h>

namespace Singleton
{
/*
Description
   Provides the basic features of singleton:
   1. Creates a single instance of class T
   2. Saves the instance as a shared_ptr
   3. Grants access to the instance
   4. Guarantees destroying of the instance by its virtual destructor

Arguments
   Template parameter T is a class name.

Return

History
   Created by Kostiantyn Zhelieznov   10/01/2018
*/
	template <typename T>
	class CSingleton
	{
		static std::once_flag     m_OnceFlag;
		static std::shared_ptr<T> m_Instance;

	public:
		static auto Instance()
		{
			std::call_once(m_OnceFlag, 
				[&] {
					assert(!m_Instance);
					m_Instance.reset(new T());
				}
			);

			assert(m_Instance);

			return m_Instance;
		}

		virtual ~CSingleton() {}

	protected:
		CSingleton() {}
		CSingleton(const CSingleton&) = delete;
		CSingleton(CSingleton&&) = delete;
		CSingleton& operator=(const CSingleton&) = delete;
		CSingleton& operator=(CSingleton&&) = delete;
	};

	template <typename T>
	std::shared_ptr<T> CSingleton<T>::m_Instance;

	template <typename T>
	std::once_flag CSingleton<T>::m_OnceFlag;

	class CDerivedSingleton : public CSingleton<CDerivedSingleton>
	{
		friend CSingleton<CDerivedSingleton>;
	};
	
	//auto Instance = CGatesAlertConsumer::Instance();

}