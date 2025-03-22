#ifndef MYLIB_CONVERT_YAMLCONVERT_H_
#define MYLIB_CONVERT_YAMLCONVERT_H_

#include "ConvertBasic.h"


// 禁用C4251警告
#pragma warning(push)
#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#ifndef YAML_CPP_API
#define YAML_CPP_API __declspec(dllimport)
#endif


#include <yaml-cpp/yaml.h>

#pragma warning(pop)


#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

MYLIB_BEGIN

MYLIB_CONSTEXPR_TYPE Dump() -> void {}

//===========================YAML转换特征模板================================

template<typename TargetTy, typename SourceTy>
struct ConversionTraits<TargetTy, SourceTy, ConvertType::CT_YAML> {
	using target_t = TargetTy;
	using source_t = SourceTy;

	static target_t convert(const source_t &source) {
		if constexpr (std::is_same_v<target_t, source_t>) {
			return source;
		}
		return boost::lexical_cast<target_t>(source);
	}
};


template<typename ValueTy>
struct ConversionTraits<String, std::vector<ValueTy>, ConvertType::CT_YAML> {
	using value_t  = ValueTy;
	using target_t = String;
	using source_t = std::vector<value_t>;

	static target_t convert(const source_t &source) {
		YAML::Node nd;
		for (const auto &val: source) {
			nd.push_back(ToYamlBase<value_t>(val));
		}
		return YAML::Dump(nd);
	}
};

template<typename ValueTy>
struct ConversionTraits<std::vector<ValueTy>, String, ConvertType::CT_YAML> {
	using value_t  = ValueTy;
	using target_t = std::vector<value_t>;
	using source_t = String;

	static target_t convert(const source_t &source) {
		target_t res;
		YAML::Node node = YAML::Load(source);
		for (const auto &iter: node) {
			res.emplace_back(FromYamlBase<value_t>(YAML::Dump(iter)));
		}
		return res;
	}
};

template<typename ValueTy>
struct ConversionTraits<String, std::list<ValueTy>, ConvertType::CT_YAML> {
	using value_t  = ValueTy;
	using target_t = String;
	using source_t = std::list<value_t>;

	static target_t convert(const source_t &source) {
		YAML::Node nd;
		for (const auto &val: source) {
			nd.push_back(ToYamlBase<value_t>(val));
		}
		return YAML::Dump(nd);
	}
};

template<typename ValueTy>
struct ConversionTraits<std::list<ValueTy>, String, ConvertType::CT_YAML> {
	using value_t  = ValueTy;
	using target_t = std::list<value_t>;
	using source_t = String;

	static target_t convert(const source_t &source) {
		target_t res;
		YAML::Node node = YAML::Load(source);
		for (const auto &iter: node) {
			res.emplace_back(FromYamlBase<value_t>(YAML::Dump(iter)));
		}
		return res;
	}
};

template<typename ValueTy>
struct ConversionTraits<String, std::forward_list<ValueTy>, ConvertType::CT_YAML> {
	using value_t  = ValueTy;
	using target_t = String;
	using source_t = std::list<value_t>;

	static target_t convert(const source_t &source) {
		YAML::Node nd;
		for (const auto &val: source) {
			nd.push_back(ToYamlBase<value_t>(val));
		}
		return YAML::Dump(nd);
	}
};

template<typename ValueTy>
struct ConversionTraits<std::forward_list<ValueTy>, String, ConvertType::CT_YAML> {
	using value_t  = ValueTy;
	using target_t = std::list<value_t>;
	using source_t = String;

	static target_t convert(const source_t &source) {
		target_t res;
		YAML::Node node = YAML::Load(source);
		for (const auto &iter: node) {
			res.emplace_back(FromYamlBase<value_t>(YAML::Dump(iter)));
		}
		return res;
	}
};

template<typename ValueTy>
struct ConversionTraits<String, std::set<ValueTy>, ConvertType::CT_YAML> {
	using value_t  = ValueTy;
	using target_t = String;
	using source_t = std::set<value_t>;

	static target_t convert(const source_t &source) {
		YAML::Node nd;
		for (const auto &ele: source) {
			nd.push_back(ToYamlBase<value_t>(ele.second));
		}
		return YAML::Dump(nd);
	}
};

template<typename ValueTy>
struct ConversionTraits<std::set<ValueTy>, String, ConvertType::CT_YAML> {
	using value_t  = ValueTy;
	using target_t = std::set<value_t>;
	using source_t = String;

	static target_t convert(const source_t &source) {
		target_t res;
		YAML::Node node = YAML::Load(source);
		for (const auto &iter: node) {
			res.emplace(FromYamlBase<value_t>(YAML::Dump(iter)));
		}
		return res;
	}
};

template<typename ValueTy>
struct ConversionTraits<String, std::unordered_set<ValueTy>, ConvertType::CT_YAML> {
	using value_t  = ValueTy;
	using target_t = String;
	using source_t = std::unordered_set<value_t>;

	static target_t convert(const source_t &source) {
		YAML::Node nd;
		for (const auto &ele: source) {
			nd.push_back(ToYamlBase<value_t>(ele.second));
		}
		return YAML::Dump(nd);
	}
};

template<typename ValueTy>
struct ConversionTraits<std::unordered_set<ValueTy>, String, ConvertType::CT_YAML> {
	using value_t  = ValueTy;
	using target_t = std::unordered_set<value_t>;
	using source_t = String;

	static target_t convert(const source_t &source) {
		target_t res;
		YAML::Node node = YAML::Load(source);
		for (const auto &iter: node) {
			res.emplace(FromYamlBase<value_t>(YAML::Dump(iter)));
		}
		return res;
	}
};

template<typename ValueTy>
struct ConversionTraits<String, std::map<String, ValueTy>, ConvertType::CT_YAML> {
	using value_t  = ValueTy;
	using target_t = String;
	using source_t = std::map<String, value_t>;

	static target_t convert(const source_t &source) {
		YAML::Node nd;
		for (const auto &ele: source) {
			nd[ele.first] = ToYamlBase<value_t>(ele.second);
		}
		return YAML::Dump(nd);
	}
};

template<typename ValueTy>
struct ConversionTraits<std::map<String, ValueTy>, String, ConvertType::CT_YAML> {
	using value_t  = ValueTy;
	using target_t = std::map<String, value_t>;
	using source_t = String;

	static target_t convert(const source_t &source) {
		target_t res;
		YAML::Node node = YAML::Load(source);
		for (const auto &iter: node) {
			String key	  = iter.first.IsScalar() ? iter.first.Scalar() : Dump(iter.first);
			value_t value = FromYamlBase<value_t>(Dump(iter.second));
			res.emplace(key, value);
		}
		return res;
	}
};

template<typename ValueTy>
struct ConversionTraits<String, std::unordered_map<String, ValueTy>, ConvertType::CT_YAML> {
	using value_t  = ValueTy;
	using target_t = String;
	using source_t = std::unordered_map<String, value_t>;

	static target_t convert(const source_t &source) {
		YAML::Node nd;
		for (const auto &ele: source) {
			nd[ele.first] = ToYamlBase<value_t>(ele.second);
		}
		return YAML::Dump(nd);
	}
};

template<typename ValueTy>
struct ConversionTraits<std::unordered_map<String, ValueTy>, String, ConvertType::CT_YAML> {
	using value_t  = ValueTy;
	using target_t = std::unordered_map<String, value_t>;
	using source_t = String;

	static target_t convert(const source_t &source) {
		target_t res;
		YAML::Node node = YAML::Load(source);
		for (const auto &iter: node) {
			String key	  = iter.first.IsScalar() ? iter.first.Scalar() : Dump(iter.first);
			value_t value = FromYamlBase<value_t>(Dump(iter.second));
			res.emplace_back(key, value);
		}
		return res;
	}
};

MYLIB_END

#endif// !MYLIB_CONVERT_YAMLCONVERT_H_