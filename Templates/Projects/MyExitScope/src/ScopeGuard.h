#pragma once
//#include "Prepocessor.h"
#include <type_traits>
#include <utility>

namespace Cis
{
	//
	// ScopeGuard is a general implementation of the "Initialization is
	// Resource Acquisition" idiom. Basically, it guarantees that a function
	// is executed upon leaving the currrent scope unless otherwise told.
	//
	template <class Func>
	class ScopeGuard
	{

	public:
		explicit ScopeGuard(const Func& function);
		explicit ScopeGuard(Func&& function);
		ScopeGuard(ScopeGuard&& other);
		~ScopeGuard();

		ScopeGuard(const ScopeGuard& other) = delete             // Non-copyable
		ScopeGuard& operator=(const ScopeGuard& other) = delete // Non-copyable

		void Dismiss();
		void Enforce();

	private:
		Func m_function;
		bool m_active;
	};

	template <class Func>
	ScopeGuard<Func>::ScopeGuard(const Func& function) :
		m_function(function),
		m_active(true)
	{
	}

	template <class Func>
	ScopeGuard<Func>::ScopeGuard(Func&& function) :
		m_function(std::move(function)),
		m_active(true)
	{
	}

	template <class Func>
	ScopeGuard<Func>::ScopeGuard(ScopeGuard&& other) :
		m_function(std::move(other.m_function)),
		m_active(other.m_active)
	{
		other.Dismiss();
	}

	template <class Func>
	ScopeGuard<Func>::~ScopeGuard()
	{
		Enforce();
	}

	template <class Func>
	void ScopeGuard<Func>::Dismiss()
	{
		m_active = false;
	}

	template <class Func>
	void ScopeGuard<Func>::Enforce()
	{
		if (m_active)
		{
			m_active = false;
			m_function();
		}
	}

	namespace Details
	{
		//
		// Internal use for the macro SCOPE_EXIT below
		//
		enum ScopeGuardOnExit {};

		template <typename Func>
		ScopeGuard<typename std::decay<Func>::type>
			operator+(ScopeGuardOnExit, Func&& function)
		{
			return ScopeGuard<typename std::decay<Func>::type>(
				std::forward<Func>(function));
		}
	}
}

#define SCOPE_EXIT \
auto CIS_ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) \
= ::Cis::Details::ScopeGuardOnExit() + [&]()

