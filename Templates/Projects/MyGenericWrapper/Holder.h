#pragma once
#include <memory>
#include <functional>

template<typename T, typename F = bool(*)(T)>
class Holder
{
	std::unique_ptr<T> mObject;
	F mManipulator;

	class Proxy
	{
		T* const mObjectPtr;
	public:
		Proxy(std::unique_ptr<T>& obj, F f) : mObjectPtr(obj.get())
		{
			if (f)
			{
				auto success = f(*mObjectPtr);
#ifdef _DEBUG
				std::cout << "\tCheck before operation";
				std::cout << std::boolalpha << "\tCondition = " << success << std::endl;
#endif
			}
		}

		~Proxy() {}

		auto* operator->() noexcept
		{
			return mObjectPtr;
		}
		const auto* operator->() const noexcept
		{
			return mObjectPtr;
		}

		auto& get() noexcept
		{
			return *mObjectPtr;
		}
		const auto& get() const noexcept
		{
			return *mObjectPtr;
		}
	};

public:
	Holder(F f) : mManipulator(f)
	{
		mObject = std::make_unique<T>(T());
	}

	Holder(T&& val, F f) :
		mObject(std::make_unique<T>(std::move(val))), mManipulator(f) {}

	template<typename... Args>
	Holder(Args&&... args, F f) :
		mObject(std::make_unique<T>(std::forward<Args>(args)...)), mManipulator(f) {}

	~Holder() {}

	auto operator->() noexcept
	{
		return Proxy(mObject, mManipulator);
	}
	const auto operator->() const noexcept
	{
		return Proxy(mObject, mManipulator);
	}

	auto operator[] (int i) noexcept
	{
		return Proxy(mObject, mManipulator).get()[i];
	}
	const auto operator[] (int i) const noexcept
	{
		return Proxy(mObject, mManipulator).get()[i];
	}

	auto& get() noexcept
	{
		return Proxy(mObject, mManipulator).get();
	}
	const auto& get() const noexcept
	{
		return Proxy(mObject, mManipulator).get();
	}
};

