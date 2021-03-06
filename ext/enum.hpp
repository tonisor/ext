#pragma once
#ifndef _ENUM_EXT_H_
#define _ENUM_EXT_H_

#include <array>
#include <type_traits>

namespace ext
{
	constexpr int strlen_c(const char* str)
	{
		return *str == 0 ? 0 : 1 + strlen_c(str + 1);
	}

	constexpr bool strequal_c(const char* s1, const char* s2)
	{
		return *s1 == 0 && *s2 == 0
			? true
			: *s1 != *s2
				? false
				: strequal_c(s1 + 1, s2 + 1);
	}

	template <typename T, std::size_t N>
	class Enum
	{
	public:
		using storage_type = typename std::array<T, N>;
		using size_type = typename storage_type::size_type;
		using const_reference = typename storage_type::const_reference;
		using const_iterator = typename storage_type::const_iterator;
		using underlying_type = T;

		static constexpr std::size_t Unknown = -1;

		constexpr Enum(const storage_type& v) : _storage(v) {}

		constexpr const_reference operator[](size_type pos) const
		{
			return _storage[pos];
		}

		constexpr const_iterator cbegin() const
		{
			return _storage.cbegin();
		}

		constexpr const_iterator cend() const
		{
			return _storage.cend();
		}

		constexpr std::size_t operator()(T v) const
		{
			std::size_t i = N - 1;
			while (i != Unknown && !equal(v, _storage[i]))
				i--;
			return i;
		}

	private:
		template <typename U = T> static constexpr
			std::enable_if_t<!std::is_same<U, const char*>::value, std::size_t>
			equal(U lhs, U rhs)
		{
			return lhs == rhs;
		}

		template <typename U = T> static constexpr
			std::enable_if_t<std::is_same<U, const char*>::value, std::size_t>
			equal(U lhs, U rhs)
		{
			return strequal_c(lhs, rhs);
		}

		storage_type _storage;
	};

	namespace details
	{
		template <typename D, typename...> struct return_type_helper { using type = D; };
		template <typename... Types>
		struct return_type_helper<void, Types...> : std::common_type<Types...> {};

		template <typename D, typename... Types>
		using return_type = std::array<typename return_type_helper<D, Types...>::type, sizeof...(Types)>;

		template <typename D, typename... Types>
		using enum_return_type = Enum<typename return_type_helper<D, Types...>::type, sizeof...(Types)>;
	}

	template <typename D = void, typename... Types>
	constexpr details::return_type<D, Types...> make_array(Types&&... t) {
		return {{std::forward<Types>(t)...}};
	}

	template <typename D = void, typename... Types>
	constexpr details::enum_return_type<D, Types...> make_enum(Types&&... t) {
		return make_array<D>(std::forward<Types>(t)...);
	}
}

#endif // _ENUM_EXT_H_
