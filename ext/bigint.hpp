#pragma once
#ifndef _BIGINT_EXT_H_
#define _BIGINT_EXT_H_

#include <cstdint>
#include <array>
#include <iostream>
#include <iomanip>
//#include <type_traits>

template <std::size_t N, typename T, typename F, std::size_t ...I>
constexpr decltype(auto) fillArray(std::index_sequence<I...>)
{
	return std::array<T, N>{ { F()(I)... } };
}

template <std::size_t N, typename T, typename F, typename Indices = std::make_index_sequence<N>>
constexpr decltype(auto) fillArray(F& f)
{
	return fillArray<N, T, F>(Indices());
}

template <std::size_t Bits>
class BigUint
{
public:
	using underlying_type = uint32_t;
	using overflow_type = uint_fast64_t;
	static constexpr auto underlying_type_bits = sizeof(underlying_type) * 8;
	static constexpr auto underlying_count = (Bits + underlying_type_bits - 1) / underlying_type_bits;
	static constexpr auto underlying_max = std::numeric_limits<underlying_type>::max();
	static constexpr auto overflow_min = overflow_type(underlying_max) + 1;
	std::array<underlying_type, underlying_count> store;

	constexpr BigUint()
		: store{}
	{}

	template <typename T>
	BigUint(T v)
	{
		std::size_t k = 0;
		overflow_type vc = v;
		while (vc != 0)
		{
			store[k++] = vc % overflow_min;
			vc /= overflow_min;
		}
		std::fill(begin(store) + k, end(store), 0);
	}

	BigUint& operator+=(const BigUint& other)
	{
		overflow_type carry = 0;
		for (std::size_t i = 0; i < underlying_count; ++i)
		{
			carry += store[i];
			carry += other.store[i];
			store[i] = carry % overflow_min;
			carry /= overflow_min;
		}
		return *this;
	}

	BigUint& operator*=(const underlying_type other)
	{
		overflow_type carry = 0;
		overflow_type p = 0;
		const overflow_type o = other;
		for (std::size_t i = 0; i < underlying_count; ++i)
		{
			carry += p;
			p = o * store[i];
			carry += p % overflow_min;
			p /= overflow_min;
			store[i] = carry % overflow_min;
			carry /= overflow_min;
		}
		return *this;
	}

	BigUint& operator/=(const underlying_type other)
	{
		overflow_type remainder = 0;
		overflow_type d;
		for (std::size_t i = underlying_count; i--; )
		{
			remainder *= overflow_min;
			remainder += store[i];
#if 0
			store[i] = remainder / other;
			remainder %= other;
#else
			d = remainder / other;
			store[i] = static_cast<underlying_type>(d);
			remainder -= d * other;
#endif
		}
		return *this;
	}

	BigUint& operator%=(const underlying_type other)
	{
		overflow_type remainder = 0;
		for (std::size_t i = underlying_count - 1; i != 0; --i)
		{
			remainder *= overflow_min;
			remainder += store[i];
			remainder %= other;
			store[i] = 0;
		}
		remainder *= overflow_min;
		remainder += store[0];
		store[0] = remainder % other;
		return *this;
	}

	BigUint& underlying_left_shift(const std::size_t value)
	{
		if (value < underlying_count)
		{
			std::move(store.begin() + value, store.end(), store.begin());
			std::fill(store.end() - value, store.end(), 0);
		}
		else
		{
			std::fill(store.begin(), store.end(), 0);
		}
	}

	BigUint& underlying_right_shift(const std::size_t value)
	{
		if (value < underlying_count)
		{
			std::move_backwards(store.begin(), store.end() - value, store.begin() + value);
			std::fill(store.begin(), store.begin() + value, 0);
		}
		else
		{
			std::fill(store.begin(), store.end(), 0);
		}
	}
};

template <std::size_t Bits>
std::ostream& operator<<(std::ostream& out, const BigUint<Bits>& u)
{
	std::size_t i = BigUint<Bits>::underlying_count;
	while (--i && u.store[i] == 0);
	out << u.store[i];
	for (; --i;)
	{
		out << std::setfill('0') << std::setw(10) << u.store[i];
	}
	out << std::setfill('0') << std::setw(10) << u.store[0];
	return out;
}

#endif // _BIGINT_EXT_H_
