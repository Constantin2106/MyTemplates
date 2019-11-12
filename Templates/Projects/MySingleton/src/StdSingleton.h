#pragma once

#include <memory>
#include <mutex>
#include <assert.h>

namespace Singleton
{
	template <typename T, typename... Arg>
	class CSingleton
	{
		static std::once_flag     m_OnceFlag;
		static std::unique_ptr<T> m_Instance;

	public:
		static auto Instance(Arg&&... arg)
		{
			std::call_once(m_OnceFlag, [&] {
				assert(!m_Instance);
				m_Instance.reset(new T(std::forward<Arg>(arg)...));
				});

			assert(m_Instance);

			return m_Instance.get();
		}

		virtual ~CSingleton() {}

	protected:
		CSingleton() {}// = delete;
	};

	template <typename T, typename... Arg>
	std::unique_ptr<T> CSingleton<T, Arg...>::m_Instance;

	template <typename T, typename... Arg>
	std::once_flag CSingleton<T, Arg...>::m_OnceFlag;

	class CDerivedSingleton : public CSingleton<CDerivedSingleton>
	{
		friend CSingleton<CDerivedSingleton>;
	};
	
	//auto Instance = CGatesAlertConsumer::Instance();
}