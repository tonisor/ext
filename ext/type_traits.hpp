#pragma once
#ifndef _TYPE_TRAITS_EXT_H_
#define _TYPE_TRAITS_EXT_H_

#include <type_traits>
#include <tuple>

namespace ext
{
	template <typename T, typename TEnable = void>
	struct make_unsigned_or_keep
	{
		typedef T type;
	};

	template <typename T>
	struct make_unsigned_or_keep<T, std::enable_if_t<std::is_signed<T>::value, void>>
	{
		typedef std::make_unsigned_t<T> type;
	};

	template <typename T>
	using make_unsigned_or_keep_t = typename make_unsigned_or_keep<T>::type;
}

//template <typename T1, typename T2, typename TEnable = void>
//struct narrower_type
//{
//	typedef T1 type;
//};
//
//template <typename T1, typename T2>
//struct narrower_type<T1, T2, std::enable_if_t<(sizeof(T1) > sizeof(T2)), void>>
//{
//	typedef T2 type;
//};
//
//template <typename T1, typename T2>
//using narrower_type_t = typename narrower_type<T1, T2>::type;
//
//template <typename T1, typename T2, typename TEnable = void>
//struct wider_type
//{
//	typedef T1 type;
//};
//
//template <typename T1, typename T2>
//struct wider_type<T1, T2, std::enable_if_t<(sizeof(T1) < sizeof(T2)), void>>
//{
//	typedef T2 type;
//};
//
//template <typename T1, typename T2>
//using wider_type_t = typename wider_type<T1, T2>::type;

//namespace Private
//{
//	template <typename Tuple>
//	struct widest_cond : std::integral_constant<bool, (sizeof(std::tuple_element_t<0, Tuple>) > sizeof(std::tuple_element_t<1, Tuple>))>
//	{};
//
//	template <std::size_t TupleCount, typename Tuple, typename Dummy = void>
//	struct widest_type
//	{
//		typedef void type;
//	};
//
//	template <typename Tuple>
//	struct widest_type<1, Tuple, void>
//	{
//		typedef std::tuple_element_t<0, Tuple> type;
//	};
//
//	template <typename Tuple>
//	struct widest_type<2, Tuple, std::enable_if_t<widest_cond<Tuple>::value, void>>
//	{
//		typedef std::tuple_element_t<0, Tuple> type;
//	};
//
//	template <typename Tuple>
//	struct widest_type<2, Tuple, std::enable_if_t<!widest_cond<Tuple>::value, void>>
//	{
//		typedef std::tuple_element_t<1, Tuple> type;
//	};
//
//	template <std::size_t TupleCount, typename Tuple>
//	struct widest_type<TupleCount, Tuple, std::enable_if_t<(TupleCount > 2), void>>
//	{
//		typedef typename widest_type<2, std::tuple<typename widest_type<TupleCount - 1, Tuple>::type, std::tuple_element_t<TupleCount - 1, Tuple>>>::type type;
//	};
//
//	template <typename Tuple>
//	using widest_type_t = typename widest_type<std::tuple_size<Tuple>::value, Tuple>::type;
//}
//
//template <typename... T>
//using widest_type_t = Private::widest_type_t<std::tuple<T...>>;

template <typename Tuple>
using first_tuple_element_t = std::tuple_element_t<0, Tuple>;
template <typename Tuple>
using second_tuple_element_t = std::tuple_element_t<1, Tuple>;

#define DEFINE_CONDITIONAL_TYPE_PRIVATE(Name, Condition) \
	namespace Private \
	{ \
		template <typename Tuple> \
		struct Name##_cond : std::integral_constant<bool, Condition(first_tuple_element_t<Tuple>, second_tuple_element_t<Tuple>)> \
		{}; \
		 \
		template <std::size_t TupleCount, typename Tuple, typename Dummy = void> \
		struct Name##_type \
		{ \
			typedef void type; \
		}; \
		 \
		template <typename Tuple> \
		struct Name##_type<1, Tuple, void> \
		{ \
			typedef std::tuple_element_t<0, Tuple> type; \
		}; \
		 \
		template <typename Tuple> \
		struct Name##_type<2, Tuple, std::enable_if_t<Name##_cond<Tuple>::value, void>> \
		{ \
			typedef std::tuple_element_t<0, Tuple> type; \
		}; \
		 \
		template <typename Tuple> \
		struct Name##_type<2, Tuple, std::enable_if_t<!Name##_cond<Tuple>::value, void>> \
		{ \
			typedef std::tuple_element_t<1, Tuple> type; \
		}; \
		 \
		template <std::size_t TupleCount, typename Tuple> \
		struct Name##_type<TupleCount, Tuple, std::enable_if_t<(TupleCount > 2), void>> \
		{ \
			typedef typename Name##_type<2, std::tuple<typename Name##_type<TupleCount - 1, Tuple>::type, std::tuple_element_t<TupleCount - 1, Tuple>>>::type type; \
		}; \
		 \
		template <typename Tuple> \
		using Name##_type_t = typename Name##_type<std::tuple_size<Tuple>::value, Tuple>::type; \
	} \
	 \
	template <typename... T> \
	using Name##_type_t = Private::Name##_type_t<std::tuple<T...>>;

#define WIDEST_TYPE_CONDITION(TLeft, TRight) (sizeof(TLeft) >= sizeof(TRight))
DEFINE_CONDITIONAL_TYPE_PRIVATE(widest, WIDEST_TYPE_CONDITION)

#define NARROWEST_TYPE_CONDITION(TLeft, TRight) (sizeof(TLeft) <= sizeof(TRight))
DEFINE_CONDITIONAL_TYPE_PRIVATE(narrowest, NARROWEST_TYPE_CONDITION)

#endif // _TYPE_TRAITS_EXT_H_
