#include <random>
#include "ext/type_traits.hpp"
#include "ext/cmath.hpp"

template <typename T>
T RandomInRange(const T& minValue, const T& maxValue)
{
	static std::mt19937 randomNumberGenerator;
	randomNumberGenerator.seed(static_cast<decltype(randomNumberGenerator)::result_type>(__rdtsc()));
	return minValue + randomNumberGenerator() % (maxValue - minValue + 1);
}

//inline constexpr uint_least8_t gcd()
//{
//	return 0;
//}
//
//template <typename TFirst, typename... TTail> inline constexpr
//widest_type_t<make_unsigned_or_keep_t<std::decay_t<TFirst>>, make_unsigned_or_keep_t<std::decay_t<TTail>>...>
//gcd(TFirst&& first, TTail&&... tail)
//{
//	//static_assert(std::is_integral<Ta>::value, "First parameter should be of an integral type.");
//	//static_assert(std::is_integral<Tb>::value, "Second parameter should be of an integral type.");
//	return sizeof...(tail) == 0 ? abs(first) :
//		sizeof...(tail) == 1 ? (abs(tail...) == 0 ? abs(first) : gcd(abs(tail...), abs(first) % abs(tail...))) :
//		gcd(first, gcd(tail...));
//}

template <char VariableName, typename TVariableFactor = int_max_t>
struct EquationTerm
{
	static const decltype(VariableName) variableName = Name;
	typedef TVariableFactor factorType;
};

//template <typename> T
//struct 
//
//template <std::size_t NVariables = 2, char X = 'x', char Y = 'y', typename CX = int_fast32_t, typename CY = int_fast32_t, typename CC = int_fast32_t>
//class Equation
//{
//public:
//	constexpr explicit Equation() : 
//
//private:
//
//};


int main()
{
	auto a = ext::gcd();
	constexpr auto x = ext::gcd((unsigned int)8, (unsigned int)0, (unsigned int)600, (unsigned int)200, (unsigned int)72);
	return 0;
}
