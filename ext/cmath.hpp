#pragma once
#ifndef _CMATH_EXT_H_
#define _CMATH_EXT_H_

#include "type_traits.hpp"

namespace ext
{
	template <typename T> inline constexpr
		std::enable_if_t<std::is_integral<T>::value && std::is_signed<T>::value, std::make_unsigned_t<std::decay_t<T>>>
		abs(const T& a)
	{
		return a < 0 ? -a : a;
	}

	template <typename T> inline constexpr
		std::enable_if_t<std::is_integral<T>::value && !std::is_signed<T>::value, std::decay_t<T>>
		abs(const T& a)
	{
		return a;
	}

	template <typename T> inline constexpr
		std::enable_if_t<std::is_floating_point<T>::value, std::decay_t<T>>
		abs(const T& a)
	{
		return std::fabs(a);
	}

	template <typename T> inline constexpr
		std::enable_if_t<!std::is_integral<T>::value && !std::is_floating_point<T>::value, std::decay_t<T>>
		abs(const T& a)
	{
		return a < 0 ? -a : a;
	}

	namespace Private
	{
		template <typename...> struct count_unsigned;
		template <typename T> struct count_unsigned<T>
		{
			static constexpr std::size_t value = std::is_integral<T>::value && !std::is_signed<T>::value;
		};
		template <typename TFirst, typename... TTail> struct count_unsigned<TFirst, TTail...>
		{
			static constexpr std::size_t value = count_unsigned<TFirst>::value + count_unsigned<TTail...>::value;
		};
	}

	inline constexpr uint_least8_t gcd()
	{
		return 0;
	}

	template <typename TFirst, typename... TTail> inline constexpr
		std::enable_if_t<sizeof...(TTail) == 0 && Private::count_unsigned<TFirst, TTail...>::value == sizeof...(TTail)+1, std::decay_t<TFirst>>
		gcd(TFirst&& first, TTail&&... tail)
	{
		return first;
	}

	template <typename TFirst, typename... TTail> inline constexpr
		std::enable_if_t<sizeof...(TTail) == 1 && Private::count_unsigned<TFirst, TTail...>::value == sizeof...(TTail)+1, widest_type_t<std::decay_t<TFirst>, std::decay_t<TTail>...>>
		gcd(TFirst&& first, TTail&&... tail)
	{
		return gcd(std::forward<TTail>(tail)...) == 0 ? first : gcd(gcd(std::forward<TTail>(tail)...), first % gcd(std::forward<TTail>(tail)...));
	}

	template <typename TFirst, typename... TTail> inline constexpr
		std::enable_if_t<(sizeof...(TTail) > 1) && Private::count_unsigned<TFirst, TTail...>::value == sizeof...(TTail)+1, widest_type_t<std::decay_t<TFirst>, std::decay_t<TTail>...>>
		gcd(TFirst&& first, TTail&&... tail)
	{
		return gcd(std::forward<TFirst>(first), gcd(std::forward<TTail>(tail)...));
	}

	template <typename TFirst, typename... TTail> inline constexpr
		std::enable_if_t<(Private::count_unsigned<TFirst, TTail...>::value < sizeof...(TTail)+1), widest_type_t<std::make_unsigned_t<std::decay_t<TFirst>>, std::make_unsigned_t<std::decay_t<TTail>>...>>
		gcd(TFirst&& first, TTail&&... tail)
	{
		static_assert(std::is_integral<TFirst>::value, "ext::gcd<T...> requires that each type in the pack is integral.");
		return gcd(ext::abs(std::forward<TFirst>(first)), gcd(std::forward<TTail>(tail)...));
	}
}

#endif // _CMATH_EXT_H_
