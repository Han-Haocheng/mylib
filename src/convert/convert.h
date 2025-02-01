//
// Created by HanHaocheng on 2025/1/29.
//
#ifndef MYLIB_CONVERT_H
#define MYLIB_CONVERT_H

#include "../core.h"

// 禁用C4251警告
#pragma warning(push)
#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#include <yaml-cpp/yaml.h>
#pragma warning(pop)

#include <boost/lexical_cast.hpp>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


MYLIB_BEGIN

enum class ConvertType {
	CT_DEFAULT,
	CT_YAML,
	CT_JSON,
};

template<typename TargetTy, typename SourceTy, ConvertType>
struct ConversionTraits;

// 转换器模板
template<typename target_t, typename source_t, ConvertType type = ConvertType::CT_DEFAULT>
target_t Convert(const source_t &source) {
	try {
		return ConversionTraits<target_t, source_t, type>::convert(source);
	} catch (const boost::bad_lexical_cast &e) {
		MYLIB_THROW(String("convert error: ") + e.what());
	} catch (...) {
		MYLIB_THROW("convert error: unknow.");
	}
}

//===========================通用转换特征模板================================

template<typename TargetTy, typename SourceTy>
struct ConversionTraits<TargetTy, SourceTy, ConvertType::CT_DEFAULT> {
	using target_t = TargetTy;
	using source_t = SourceTy;

	static target_t convert(const source_t &source) {
		if constexpr (std::is_same_v<target_t, source_t>) {
			return source;
		}
		return boost::lexical_cast<target_t>(source);
	}
};

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
			nd.push_back(ConversionTraits<target_t, ValueTy, ConvertType::CT_YAML>::convert(val));
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
			res.emplace_back(ConversionTraits<value_t, source_t, ConvertType::CT_YAML>::convert(Dump(iter)));
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
			nd.push_back(ConversionTraits<target_t, ValueTy, ConvertType::CT_YAML>::convert(val));
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
			res.emplace_back(ConversionTraits<value_t, source_t, ConvertType::CT_YAML>::convert(Dump(iter)));
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
			nd.push_back(ConversionTraits<target_t, value_t, ConvertType::CT_YAML>::convert(val));
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
			res.emplace_back(ConversionTraits<value_t, source_t, ConvertType::CT_YAML>::convert(Dump(iter)));
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
			nd.push_back(ConversionTraits<target_t, value_t, ConvertType::CT_YAML>::convert(ele.second));
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
			res.emplace(ConversionTraits<value_t, source_t, ConvertType::CT_YAML>::convert(Dump(iter)));
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
			nd.push_back(ConversionTraits<target_t, value_t, ConvertType::CT_YAML>::convert(ele.second));
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
			res.emplace(ConversionTraits<value_t, source_t, ConvertType::CT_YAML>::convert(Dump(iter)));
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
			nd[ele.first] = ConversionTraits<target_t, value_t, ConvertType::CT_YAML>::convert(ele.second);
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
			value_t value = ConversionTraits<value_t, source_t, ConvertType::CT_YAML>::convert(Dump(iter.second));
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
			nd[ele.first] = ConversionTraits<target_t, value_t, ConvertType::CT_YAML>::convert(ele.second);
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
			value_t value = ConversionTraits<value_t, source_t, ConvertType::CT_YAML>::convert(Dump(iter.second));
			res.emplace_back(key, value);
		}
		return res;
	}
};

//===========================JSON转换特征模板================================

template<typename TargetTy, typename SourceTy>
struct ConversionTraits<TargetTy, SourceTy, ConvertType::CT_JSON> {
	using target_t = TargetTy;
	using source_t = SourceTy;

	static target_t convert(const source_t &source) {
		if constexpr (std::is_same_v<target_t, source_t>) {
			return source;
		}
		return boost::lexical_cast<target_t>(source);
	}
};

MYLIB_END

#endif
