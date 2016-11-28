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

	namespace Private
	{
		template <typename Tuple, typename Condition>
		struct winner_cond : Condition::template condition<std::tuple_element_t<0, Tuple>, std::tuple_element_t<1, Tuple>>
		{};

		template <std::size_t TupleCount, typename Tuple, typename Condition, typename Dummy = void>
		struct winner_type
		{
			typedef void type;
		};

		template <typename Tuple, typename Condition>
		struct winner_type<1, Tuple, Condition, void>
		{
			typedef std::tuple_element_t<0, Tuple> type;
		};

		template <typename Tuple, typename Condition>
		struct winner_type<2, Tuple, Condition, std::enable_if_t<winner_cond<Tuple, Condition>::value, void>>
		{
			typedef std::tuple_element_t<0, Tuple> type;
		};

		template <typename Tuple, typename Condition>
		struct winner_type<2, Tuple, Condition, std::enable_if_t<!winner_cond<Tuple, Condition>::value, void>>
		{
			typedef std::tuple_element_t<1, Tuple> type;
		};

		template <std::size_t TupleCount, typename Tuple, typename Condition>
		struct winner_type<TupleCount, Tuple, Condition, std::enable_if_t<(TupleCount > 2), void>>
		{
			typedef typename winner_type<2, std::tuple<typename winner_type<TupleCount - 1, Tuple, Condition>::type, std::tuple_element_t<TupleCount - 1, Tuple>>, Condition>::type type;
		};

		template <typename Tuple, typename Condition>
		using winner_type_t = typename winner_type<std::tuple_size<Tuple>::value, Tuple, Condition>::type;
	}

	template <typename T = void>
	struct widest_type
	{
		template <typename TLeft, typename TRight>
		struct condition : std::integral_constant<bool, (sizeof(TLeft) >= sizeof(TRight))>
		{};
	};

	template <typename... T>
	using widest_type_t = Private::winner_type_t<std::tuple<T...>, widest_type<>>;

	template <typename T = void>
	struct narrowest_type
	{
		template <typename TLeft, typename TRight>
		struct condition : std::integral_constant<bool, (sizeof(TLeft) <= sizeof(TRight))>
		{};
	};

	template <typename... T>
	using narrowest_type_t = Private::winner_type_t<std::tuple<T...>, narrowest_type<>>;

	template <typename T = void>
	struct widest_integral_type
	{
		template <typename TLeft, typename TRight>
		struct condition : std::integral_constant<bool, std::is_integral<TLeft>::value ? (std::is_integral<TRight>::value ? (sizeof(TLeft) >= sizeof(TRight)) : true) : (std::is_integral<TRight>::value ? false : true)>
		{};
	};

	template <typename... T>
	using widest_integral_type_t = Private::winner_type_t<std::tuple<T...>, widest_integral_type<>>;
}

#endif // _TYPE_TRAITS_EXT_H_
