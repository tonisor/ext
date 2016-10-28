#include <random>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <utility>
#include <tuple>
#include <array>
#include <regex>
#include <string>
#include <memory>
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

//template <char VariableName, typename TVariableFactor = int_max_t>
//struct EquationTerm
//{
//	static const decltype(VariableName) variableName = Name;
//	typedef TVariableFactor factorType;
//};

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

//template <typename T = void>
//struct Op1Square
//{
//	template <typename T1>
//	auto operator()(T1&& v1) const
//	{
//		return static_cast<T1&&>(v1) * static_cast<T1&&>(v1);
//	}
//};

template <typename T> struct type_c;
template <typename ...T> struct tuple_c;

template <typename T>
struct type_c
{
	constexpr type_c() {}
	using type = T;

	template <typename TOther>
	friend constexpr auto operator+(const type_c<T>& left, const type_c<TOther>& right)
	{
		return tuple_c<T, TOther>{};
	}

	template <typename ...TOther>
	friend constexpr auto operator+(const type_c<T>& left, const tuple_c<TOther...>& other)
	{
		return tuple_c<T, TOther...>{};
	}
};

template <typename ...T>
struct tuple_c
{
	constexpr tuple_c() {}
	using type = std::tuple<T...>;

	template <typename TOther>
	friend constexpr auto operator+(const tuple_c<T...>& left, const type_c<TOther>& right)
	{
		return tuple_c<T..., TOther>{};
	}

	template <typename ...TOther>
	friend constexpr auto operator+(const tuple_c<T...>& left, const tuple_c<TOther...>& other)
	{
		return tuple_c<T..., TOther...>{};
	}
};

template <std::size_t N>
struct base_struct
{
	using membersType = tuple_c<>;
	typename membersType::type members_;
	template <std::size_t I>
	const auto& get() const { return std::get<I>(members_); }
	template <std::size_t I>
	auto& get() { return std::get<I>(members_); }
};

struct s0
{
	using membersType = tuple_c<>;
	static constexpr auto memberCount = std::tuple_size_v<typename membersType::type>;
	static constexpr std::array<const char*, memberCount> memberNames_ = { {} };
	typename membersType::type members_;
	template <std::size_t I>
	const auto& get() const { return std::get<I>(members_); }
	template <std::size_t I>
	auto& get() { return std::get<I>(members_); }
};

constexpr std::size_t strlen_c(const char* str)
{
	return *str != char{0} ? 1 + strlen_c(str + 1) : 0;
}

constexpr bool strequal_c(const char* left, const char* right)
{
	return *left == char{0}
			? *right == char{0}
				? true
				: false
			: *right == char{0}
				? false
				: (*left != *right)
					? false
					: strequal_c(left + 1, right + 1)
	;
}

template <std::size_t N, std::size_t I = N - 1>
constexpr std::pair<std::size_t, bool> find(const std::array<const char*, N>& a, const char* str)
{
	return N < 1
			? std::make_pair(std::size_t{}, false)
			: I >= N
				? find<N, N - 1>(a, str)
				: I == 0
					? std::make_pair(I, strequal_c(a[0], str))
					: strequal_c(a[I], str)
						? std::make_pair(I, true)
						: find<N, I - 1>(a, str)
	;
}

struct s2
{
	using membersType = tuple_c<int, bool>;
	static constexpr auto memberCount = std::tuple_size_v<typename membersType::type>;
	static constexpr std::array<const char*, memberCount> memberNames_ = { { "range", "valid" } };
	typename membersType::type members_;

	template <std::size_t I>
	const auto& get() const
	{
		static_assert(I < memberCount, "Template parameter I is too large");
		return std::get<I>(members_);
	}
	template <std::size_t I>
	auto& get()
	{
		static_assert(I < memberCount, "Template parameter I is too large");
		return std::get<I>(members_);
	}

	//template <std::size_t I = memberCount>
	//static constexpr std::pair<std::size_t, bool> find(const char* str)
	//{
	//	return memberCount < 1
	//			? std::make_pair(std::size_t{}, false)
	//			: I == 0
	//				? std::make_pair(I, strequal_c(memberNames_[0], str))
	//				: strequal_c(memberNames_[I], str)
	//					? std::make_pair(I, true)
	//					: find<I - 1>(str)
	//	;
	//}

	template <std::size_t I>
	static constexpr bool isMember(const char* str)
	{
		return strequal_c(memberNames_[I], str);
	}

	template <std::size_t I = memberCount - 1>
	static constexpr std::size_t memberPosition(const char* str)
	{
		return memberCount < 1
				? 255
				: isMember<I>(str)
					? I
					: I == 0
						? 255
						: memberPosition<I - 1>(str)
		;
	}

	//const auto& get(const char* member) const
	//{
	//	constexpr auto findResult = find<memberCount>(memberNames_, member);
	//	static_assert(findResult.second, "Member not found in struct");
	//	return get<findResult.first>();
	//}
	//auto& get(const char* member)
	//{
	//	static_assert(find<memberCount>(memberNames_, member).second, "Member not found in struct");
	//	return get<find<memberCount>(memberNames_, member).first>();
	//}

	const auto& range() const { return get<0>(); }
	auto& range() { return get<0>(); }
	const auto& valid() const { return get<1>(); }
	auto& valid() { return get<1>(); }
};

#define DECLARE_STRUCT(name, type1, member1, type2, member2)												\
	struct name {																							\
		using membersType = tuple_c<type1, type2>;															\
		static constexpr auto memberCount = std::tuple_size_v<typename membersType::type>;					\
		static constexpr std::array<const char*, memberCount> memberNames_ = { { #member1, #member2 } };	\
		typename membersType::type members_;																\
		template <std::size_t I>																			\
		const auto& get() const { return std::get<I>(members_); }											\
		template <std::size_t I>																			\
		auto& get() { return std::get<I>(members_); }														\
		const auto& ##member1() const { return get<0>(); }													\
		auto& ##member1() { return get<0>(); }																\
		const auto& ##member2() const { return get<1>(); }													\
		auto& ##member2() { return get<1>(); }																\
	};

DECLARE_STRUCT(macroStruct,
	int, range,
	bool, valid)

class object_t
{
public:
	template <typename T>
	object_t(T v) : self_(std::make_shared<model<T>>(std::move(v))) {}

	std::string to_string() const
	{
		return self_->to_string();
	}

private:
	struct concept_t
	{
		virtual ~concept_t() = default;
		virtual std::string to_string() const = 0;
	};
	template <typename T>
	class model : concept_t
	{
		model(T v) : data_(std::move(v)) {}
		std::string to_string() const { return std::string(); }
		T data_;
	};
	std::shared_ptr<const concept_t> self_;
};

template <typename T>
void my_println(const T& v)
{
	std::cout << v << std::endl;
}

template <typename T>
void my_println(const std::vector<T>& v)
{
	std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, " "));
	std::cout << std::endl;
}

void test1()
{
	std::vector<int> a(10);
	std::vector<int> b(10, 10);
	int x = 3;
	std::generate(a.begin(), a.end(), [&x]() { return x++; });
	my_println(a);
	my_println(b);

	std::remove_copy(b.begin(), b.begin() + 5, a.begin() + 5, 10);

	my_println(a);
	my_println(b);

	int i; std::cin >> i;
}

struct ZZ
{
	int x;
	ZZ(int z) : x(z) {}
};

struct Bau
{
	Bau() {}
	Bau(const ZZ& z) {}
	Bau(const ZZ&&) = delete;
};

//template <typename T>
//struct szt
//{
//	static constexpr int value = int(sizeof(T));
//	char* value(char* currentValue, const T& value)
//	{
//		return currentValue + sizeof(value);
//	}
//};

template <typename T>
void GenericSave(char*& currentValue, const T& value, size_t sizeOfValue)
{
	currentValue += sizeof(value);
}

//#define GENERIC_SAVE(currentValue, value) { currentValue += sizeof(value); }
//#define xstr(TUse, currentValue, value) GENERIC_SAVE(TUse, currentValue, value)
//#define GENERIC_SAVE(TUse, currentValue, value) { GenericSave##TUse(currentValue, value); }

constexpr int bau(const int x) { return sizeof(decltype(x)); }

enum SaveUse : uint8_t
{
	kSave,
	kAddSaveSize,
};

template <SaveUse TUse> struct generic_save;
template<> struct generic_save<kSave> { typedef char* type; };
template<> struct generic_save<kAddSaveSize> { typedef uintptr_t type; };

template <SaveUse TUse>
using generic_save_t = typename generic_save<TUse>::type;

template <SaveUse TUse, typename T>
auto gensave(T&& value) -> std::enable_if_t<TUse == kSave, generic_save_t<TUse>>
{
	std::cout << value;
	return nullptr;
}

template <SaveUse TUse, typename T>
constexpr auto gensave(T&& value) -> std::enable_if_t<TUse == kAddSaveSize, generic_save_t<TUse>>
{
	return sizeof(value);
}

class Obj
{
	int mInt;
	bool mBool;

	enum HelperType : uint8_t
	{
		eSave,
		eGetSaveSize,
	};

	template <HelperType Type>
	std::conditional_t<Type == eSave, char*, size_t> Generic(char* buffer = nullptr) const;

public:
	int getInt() const { return mInt; }
	bool getBool() const { return mBool; }

	char* Save(char* buffer) const { return Generic<eSave>(buffer); }
	size_t GetSaveSize() const { return Generic<eGetSaveSize>(); }
};

template <Obj::HelperType Type>
std::conditional_t<Type == Obj::eSave, char*, size_t> Obj::Generic(char* buffer) const
{
	typedef std::conditional_t<Type == eSave, char*, size_t> TRetVal;
	TRetVal retval = reinterpret_cast<TRetVal>(Type == eSave ? buffer : 0);
	auto process = [&retval](auto&& v) {
		if (Type == eSave)
			retval += sizeof(v);
		else
			retval += 2 * sizeof(v);
	};
	process(mInt);
	process(getBool());

	//xstr(Type, retval, mInt);

	return retval;
}

template <typename T>
constexpr int f(T& n) { return sizeof(T); }
int n = 0;
constexpr int i = f(n);

int main()
{
	{
		static_assert(std::tuple_size_v<decltype(macroStruct::memberNames_)> == 2, "");
		macroStruct m;
		m.range() = 3;
		static_assert(s2::memberNames_[0][0] == 'r', "");
		static_assert(strlen_c(s2::memberNames_[0]) == 5, "");
		static_assert(strequal_c(s2::memberNames_[0], "range"), "");
		static_assert(strequal_c(s2::memberNames_[0], "rang") == false, "");
		static_assert(strequal_c(s2::memberNames_[0], "ranger") == false, "");
		//constexpr auto im = s2::isMember<1>("fasfa");
		//constexpr auto vvv = s2::memberPosition("rangee");
		constexpr auto findResult = find(s2::memberNames_, "range");
		constexpr std::array<const char*, 2> aa{{"range", "valid"}};
		//constexpr auto vvv = bau(aa, "rangee");
		//static_assert(vvv.second == false, "");
		{

			tuple_c<int, float> t1;
			type_c<bool> b1;
			constexpr auto res = t1 + b1;
			static_assert(std::is_same_v<typename decltype(res)::type, std::tuple<int, float, bool>>, "");
		}
		{

			tuple_c<int, float> t1;
			tuple_c<bool, double> t2;
			constexpr auto res = t1 + t2;
			static_assert(std::is_same_v<typename decltype(res)::type, std::tuple<int, float, bool, double>>, "");
		}
		{

			tuple_c<int, float> t1;
			type_c<bool> b1;
			constexpr auto res = b1 + t1;
			static_assert(std::is_same_v<typename decltype(res)::type, std::tuple<bool, int, float>>, "");
		}
		{

			tuple_c<int, float> t1;
			tuple_c<bool, double> t2;
			constexpr auto res = t2 + t1;
			static_assert(std::is_same_v<typename decltype(res)::type, std::tuple<bool, double, int, float>>, "");
		}
	}
	auto af = []() { return std::make_pair(0, 'a'); };
	//int ai;
	char ac;
	std::tie(std::ignore, ac) = af();

	int ai = sin(6);
	generic_save_t<kAddSaveSize> cvalue = 0;
	constexpr auto resai = gensave<kAddSaveSize>(ai);
	static_assert(resai == 4, "");

	//ZZ qwerq(3);
	//Bau asdf(ZZ(3));

	std::vector<std::string> words;
	std::string str("at b c d e");
	const auto r = std::regex("[b-d]+");
	auto it = std::sregex_token_iterator(str.begin(), str.end(), r);
	std::copy(it, std::sregex_token_iterator(), std::back_inserter(words));
	for (const auto& s : words)
		std::cout << s << ",";
	std::cout << std::endl;

	//const auto& eit = std::sregex_token_iterator();
	////std::vector<std::string> words;
	//std::copy(std::sregex_token_iterator(str.begin(), str.end(), std::regex("(\\+|-)?[[:digit:]]+")),
	//	eit,
	//	std::back_inserter(words));

	test1();
	auto a = ext::gcd();

	//ext::narrowest_type_t<float, char> aa;

	constexpr auto x = ext::gcd((unsigned int)8, (unsigned int)0, (unsigned int)600, (unsigned int)200, (unsigned int)72);
	constexpr auto y = ext::min((unsigned int)800, (unsigned int)900, (unsigned int)600, (unsigned int)200, (unsigned int)72);
	return 0;
}
