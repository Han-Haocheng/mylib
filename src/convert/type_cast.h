#ifndef MYLIB_CONVERTER_H
#define MYLIB_CONVERTER_H

#include "../core.h"

// 禁用C4251警告
#pragma warning(push)
#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#include <yaml-cpp/yaml.h>

// 重新启用刚才禁用的警告
#pragma warning(pop)

#include <forward_list>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/lexical_cast.hpp>

MYLIB_BEGIN

template<typename FromType, typename ToType, typename = void>
class Cast {
public:
	using from_type = FromType;
	using to_type	= ToType;

	static to_type cast(const from_type &from) { return boost::lexical_cast<to_type>(from); }
};

template<typename FromType, typename ToType>
class Cast<FromType, ToType, std::enable_if_t<std::is_same_v<FromType, ToType>>> {
public:
	using from_type = FromType;
	using to_type	= ToType;

	static to_type cast(const from_type &from) { return from; }
};


template<typename Ty>
class StringCast {
public:
	using cast_t = Ty;

	static String toString(const cast_t &ty) { return Cast<cast_t, String>::cast(ty); }
	static cast_t fromString(const String &ty) { return Cast<String, cast_t>::cast(ty); }
};


template<typename U>
class is_StringCast {

	template<typename T>
	using FromString = decltype(T::fromString(std::declval<const String &>()));

	template<typename T>
	using ToString = decltype(T::toString(std::declval<const FromString<T> &>()));

	template<typename, typename = void>
	struct hasToString : std::false_type {};
	template<typename, typename = void>
	struct hasFromString : std::false_type {};

	template<typename T>
	struct hasToString<T, std::void_t<ToString<T>>> : std::true_type {};

	template<typename T>
	struct hasFromString<T, std::void_t<FromString<T>>> : std::true_type {};

	//using typ = decltype(std::declval<U>().fromString(std::declval<const String &>()));
public:
	using type					= std::integral_constant<bool, hasToString<U>::value && hasFromString<U>::value>;
	static constexpr bool value = hasToString<U>::value && hasFromString<U>::value;
};

#define YAML_CAST_TRY_BEG try {
#define YAML_CAST_TRY_END                    \
	}                                        \
	catch (const YAML::ParserException &e) { \
		MYLIB_THROW(e.what());               \
	}                                        \
	catch (std::exception & e) {             \
		MYLIB_THROW(e.what());               \
	}                                        \
	catch (...) {                            \
		MYLIB_THROW("cast_error");           \
	}

template<typename Ty>
class YamlCast {
public:
	using cast_t  = Ty;
	using value_t = Ty;

	static cast_t fromString(const String &yaml) { return StringCast<Ty>::fromString(yaml); }
	static String toString(const cast_t &ty) { return StringCast<Ty>::toString(ty); }
};

template<typename Ty>
class YamlCast<std::vector<Ty>> {
public:
	using cast_t  = std::vector<Ty>;
	using value_t = Ty;

	static cast_t fromString(const String &yaml);
	static String toString(const cast_t &ty);
};


template<typename Ty>
class YamlCast<std::list<Ty>> {
public:
	using cast_t  = std::list<Ty>;
	using value_t = Ty;

	static cast_t fromString(const String &yaml);
	static String toString(const cast_t &ty);
};

template<typename Ty>
class YamlCast<std::forward_list<Ty>> {
public:
	using cast_t  = std::forward_list<Ty>;
	using value_t = Ty;

	static cast_t fromString(const String &yaml);
	static String toString(const cast_t &ty);
};

template<typename Ty>
class YamlCast<std::set<Ty>> {
public:
	using cast_t  = std::set<Ty>;
	using value_t = Ty;

	static cast_t fromString(const String &yaml);
	static String toString(const cast_t &ty);
};

template<typename Ty>
class YamlCast<std::unordered_set<Ty>> {
public:
	using cast_t  = std::unordered_set<Ty>;
	using value_t = Ty;

	static cast_t fromString(const String &yaml);
	static String toString(const cast_t &ty);
};

template<typename Ty>
class YamlCast<std::map<String, Ty>> {
public:
	using cast_t  = std::map<String, Ty>;
	using value_t = Ty;

	static cast_t fromString(const String &yaml);
	static String toString(const cast_t &ty);
};

template<typename Ty>
class YamlCast<std::unordered_map<String, Ty>> {
public:
	using cast_t  = std::unordered_map<String, Ty>;
	using value_t = Ty;

	static cast_t fromString(const String &yaml);
	static String toString(const cast_t &ty);
};

template<typename Ty>
typename YamlCast<std::vector<Ty>>::cast_t YamlCast<std::vector<Ty>>::fromString(const String &yaml) {
	cast_t res;
	YAML_CAST_TRY_BEG
	YAML::Node node = YAML::Load(yaml);
	for (const auto &iter: node) { res.emplace_back(YamlCast<Ty>::fromString(Dump(iter))); }
	YAML_CAST_TRY_END
	return res;
}

template<typename Ty>
String YamlCast<std::vector<Ty>>::toString(const cast_t &ty) {
	YAML::Node node;
	YAML_CAST_TRY_BEG
	for (const auto &ele: ty) {
		node.push_back(YAML::Load(YamlCast<Ty>::toString(ele)));
	}
	YAML_CAST_TRY_END
	return Dump(node);
}
template<typename Ty>
typename YamlCast<std::list<Ty>>::cast_t YamlCast<std::list<Ty>>::fromString(const String &yaml) {
	cast_t res;
	YAML_CAST_TRY_BEG
	YAML::Node node = YAML::Load(yaml);
	for (const auto &iter: node) {
		res.emplace_back(YamlCast<Ty>::fromString(Dump(iter)));
	}
	YAML_CAST_TRY_END
	return res;
}
template<typename Ty>
String YamlCast<std::list<Ty>>::toString(const cast_t &ty) {
	YAML::Node node;
	YAML_CAST_TRY_BEG
	for (auto &ele: ty) {
		node.push_back(YamlCast<Ty>::toString(ele));
	}
	YAML_CAST_TRY_END
	return Dump(node);
}
template<typename Ty>
typename YamlCast<std::forward_list<Ty>>::cast_t YamlCast<std::forward_list<Ty>>::fromString(const String &yaml) {
	YAML::Node node = YAML::Load(yaml);
	cast_t res;
	for (const auto &iter: node) { res.emplace_front(YamlCast<Ty>::fromString(Dump(iter))); }
	return res;
}
template<typename Ty>
String YamlCast<std::forward_list<Ty>>::toString(const cast_t &ty) {
	YAML::Node node;
	for (const auto &ele: ty) { node.push_back(YamlCast<value_t>::toString(ele)); }
	return Dump(node);
}
template<typename Ty>
typename YamlCast<std::set<Ty>>::cast_t YamlCast<std::set<Ty>>::fromString(const String &yaml) {
	YAML::Node node = YAML::Load(yaml);
	cast_t res;
	for (const auto &iter: node) { res.emplace(YamlCast<value_t>::fromString(Dump(iter))); }
	return res;
}
template<typename Ty>
String YamlCast<std::set<Ty>>::toString(const cast_t &ty) {
	YAML::Node node;
	for (const auto &ele: ty) { node.push_back(YamlCast<value_t>::toString(ele)); }
	return Dump(node);
}
template<typename Ty>
typename YamlCast<std::unordered_set<Ty>>::cast_t YamlCast<std::unordered_set<Ty>>::fromString(const String &yaml) {
	YAML::Node node = YAML::Load(yaml);
	cast_t res;
	for (const auto &iter: node) { res.emplace(YamlCast<value_t>::fromString(Dump(iter))); }
	return res;
}
template<typename Ty>
String YamlCast<std::unordered_set<Ty>>::toString(const cast_t &ty) {
	YAML::Node node;
	for (const auto &ele: ty) { node.push_back(YamlCast<value_t>::toString(ele)); }
	return Dump(node);
}
template<typename Ty>
typename YamlCast<std::map<std::string, Ty>>::cast_t YamlCast<std::map<std::string, Ty>>::fromString(const String &yaml) {
	YAML::Node node = YAML::Load(yaml);
	cast_t res;
	for (const auto &iter: node) {
		String key	  = iter.first.IsScalar() ? iter.first.Scalar() : Dump(iter.first);
		value_t value = YamlCast<value_t>::fromString(Dump(iter.second));
		res.emplace(key, value);
	}

	return res;
}
template<typename Ty>
String YamlCast<std::map<std::string, Ty>>::toString(const cast_t &ty) {
	YAML::Node node;
	for (const auto &ele: ty) { node[ele.first] = YamlCast<value_t>::toString(ele.second); }
	return Dump(node);
}
template<typename Ty>
typename YamlCast<std::unordered_map<std::string, Ty>>::cast_t YamlCast<std::unordered_map<std::string, Ty>>::fromString(const String &yaml) {
	YAML::Node node = YAML::Load(yaml);
	cast_t res;
	for (const auto &iter: node) {
		String key	  = iter.first.IsScalar() ? iter.first.Scalar() : Dump(iter.first);
		value_t value = YamlCast<value_t>::fromString(Dump(iter.second));
		res.emplace(key, value);
	}
	return res;
}
template<typename Ty>
String YamlCast<std::unordered_map<std::string, Ty>>::toString(const cast_t &ty) {
	YAML::Node node;
	for (const auto &ele: ty) { node[ele.first] = YamlCast<value_t>::toString(ele.second); }
	return Dump(node);
}
MYLIB_END

#endif//MYLIB_CONVERTER_H
