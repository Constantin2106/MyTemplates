#pragma once

//template <typename FunSignature> struct TTFunc;
template <typename T, bool(*fun)(T, T)>
struct TComparator
{
	bool operator() (T a, T b)
	{
		return fun(a, b);
	}
};

template <typename R, typename A, R(*fun)(A)>
struct  TSquare
{
	R operator() (A a)
	{
		return fun(a);
	}

};

//template <typename R, typename... A>
//struct FunSign
//{
//
//};
//template <typename FunSign> class TTFunction;
//template <typename R, typename A>
//class TTFunction<R(A...)>
//
//{
//	R operator() (A... arg)
//	{
//		return fun(std::forward<A>(arg)...);
//	}
//};
