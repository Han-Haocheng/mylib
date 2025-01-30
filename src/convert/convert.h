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

// 通用转换模板
template<typename TargetTy, typename SourceTy>
struct ConversionTraits {
	using target_t = TargetTy;
	using source_t = SourceTy;

	static target_t convert(const source_t &source) {
		if constexpr (!std::is_same_v<target_t, source_t>) {
			return boost::lexical_cast<target_t>(source);
		} else {
			return source;
		}
	}
};

template<typename SourceTy>
struct ConversionTraits<String, SourceTy> {
	using target_t = String;
	using source_t = SourceTy;

	static target_t convert(const source_t &source) {
		return ConversionTraits<target_t, SourceTy>::convert(source);
	}

	static target_t to_yaml(const source_t &source) {
		return YAML::Dump(YAML::Node{source});
	}
};

template<typename TargetTy>
struct ConversionTraits<TargetTy, String> {
	using target_t = TargetTy;
	using source_t = String;

	static target_t convert(const source_t &source) {
		return ConversionTraits<target_t, source_t>::convert(source);
	}

	static target_t from_yaml(const source_t &source) {
		return YAML::Load(source).as<target_t>();
	}
};

template<typename ValueTy>
struct ConversionTraits<String, std::vector<ValueTy>> {
	using value_t  = ValueTy;
	using target_t = String;
	using source_t = std::vector<value_t>;

	static target_t to_yaml(const source_t &source) {
		YAML::Node nd;
		for (const auto &val: source) {
			nd.push_back(ConversionTraits<target_t, ValueTy>::to_yaml(val));
		}
		return YAML::Dump(nd);
	}
};

template<typename ValueTy>
struct ConversionTraits<std::vector<ValueTy>, String> {
	using value_t  = ValueTy;
	using target_t = std::vector<value_t>;
	using source_t = String;

	static target_t from_yaml(const source_t &source) {
		target_t res;
		YAML::Node node = YAML::Load(source);
		for (const auto &iter: node) {
			res.emplace_back(ConversionTraits<value_t, source_t>::from_yaml(Dump(iter)));
		}
		return res;
	}
};

template<typename ValueTy>
struct ConversionTraits<String, std::list<ValueTy>> {
	using value_t  = ValueTy;
	using target_t = String;
	using source_t = std::list<value_t>;

	static target_t to_yaml(const source_t &source) {
		YAML::Node nd;
		for (const auto &val: source) {
			nd.push_back(ConversionTraits<target_t, ValueTy>::to_yaml(val));
		}
		return YAML::Dump(nd);
	}
};

template<typename ValueTy>
struct ConversionTraits<std::list<ValueTy>, String> {
	using value_t  = ValueTy;
	using target_t = std::list<value_t>;
	using source_t = String;

	static target_t from_yaml(const source_t &source) {
		target_t res;
		YAML::Node node = YAML::Load(source);
		for (const auto &iter: node) {
			res.emplace_back(ConversionTraits<value_t, source_t>::from_yaml(Dump(iter)));
		}
		return res;
	}
};

template<typename ValueTy>
struct ConversionTraits<String, std::forward_list<ValueTy>> {
	using value_t  = ValueTy;
	using target_t = String;
	using source_t = std::list<value_t>;

	static target_t to_yaml(const source_t &source) {
		YAML::Node nd;
		for (const auto &val: source) {
			nd.push_back(ConversionTraits<target_t, value_t>::to_yaml(val));
		}
		return YAML::Dump(nd);
	}
};

template<typename ValueTy>
struct ConversionTraits<std::forward_list<ValueTy>, String> {
	using value_t  = ValueTy;
	using target_t = std::list<value_t>;
	using source_t = String;

	static target_t from_yaml(const source_t &source) {
		target_t res;
		YAML::Node node = YAML::Load(source);
		for (const auto &iter: node) {
			res.emplace_back(ConversionTraits<value_t, source_t>::from_yaml(Dump(iter)));
		}
		return res;
	}
};

template<typename ValueTy>
struct ConversionTraits<String, std::set<ValueTy>> {
	using value_t  = ValueTy;
	using target_t = String;
	using source_t = std::set<value_t>;

	static target_t to_yaml(const source_t &source) {
		YAML::Node nd;
		for (const auto &ele: source) {
			nd.push_back(ConversionTraits<target_t, value_t>::to_yaml(ele.second));
		}
		return YAML::Dump(nd);
	}
};

template<typename ValueTy>
struct ConversionTraits<std::set<ValueTy>, String> {
	using value_t  = ValueTy;
	using target_t = std::set<value_t>;
	using source_t = String;

	static target_t from_yaml(const source_t &source) {
		target_t res;
		YAML::Node node = YAML::Load(source);
		for (const auto &iter: node) {
			res.emplace(ConversionTraits<value_t, source_t>::from_yaml(Dump(iter)));
		}
		return res;
	}
};

template<typename ValueTy>
struct ConversionTraits<String, std::unordered_set<ValueTy>> {
	using value_t  = ValueTy;
	using target_t = String;
	using source_t = std::unordered_set<value_t>;

	static target_t to_yaml(const source_t &source) {
		YAML::Node nd;
		for (const auto &ele: source) {
			nd.push_back(ConversionTraits<target_t, value_t>::to_yaml(ele.second));
		}
		return YAML::Dump(nd);
	}
};

template<typename ValueTy>
struct ConversionTraits<std::unordered_set<ValueTy>, String> {
	using value_t  = ValueTy;
	using target_t = std::unordered_set<value_t>;
	using source_t = String;

	static target_t from_yaml(const source_t &source) {
		target_t res;
		YAML::Node node = YAML::Load(source);
		for (const auto &iter: node) {
			res.emplace(ConversionTraits<value_t, source_t>::from_yaml(Dump(iter)));
		}
		return res;
	}
};

template<typename ValueTy>
struct ConversionTraits<String, std::map<String, ValueTy>> {
	using value_t  = ValueTy;
	using target_t = String;
	using source_t = std::map<String, value_t>;

	static target_t to_yaml(const source_t &source) {
		YAML::Node nd;
		for (const auto &ele: source) {
			nd[ele.first] = ConversionTraits<target_t, value_t>::to_yaml(ele.second);
		}
		return YAML::Dump(nd);
	}
};

template<typename ValueTy>
struct ConversionTraits<std::map<String, ValueTy>, String> {
	using value_t  = ValueTy;
	using target_t = std::map<String, value_t>;
	using source_t = String;

	static target_t from_yaml(const source_t &source) {
		target_t res;
		YAML::Node node = YAML::Load(source);
		for (const auto &iter: node) {
			String key	  = iter.first.IsScalar() ? iter.first.Scalar() : Dump(iter.first);
			value_t value = ConversionTraits<value_t, source_t>::from_yaml(Dump(iter.second));
			res.emplace_back(key, value);
		}
		return res;
	}
};

template<typename ValueTy>
struct ConversionTraits<String, std::unordered_map<String, ValueTy>> {
	using value_t  = ValueTy;
	using target_t = String;
	using source_t = std::unordered_map<String, value_t>;

	static target_t to_yaml(const source_t &source) {
		YAML::Node nd;
		for (const auto &ele: source) {
			nd[ele.first] = ConversionTraits<target_t, value_t>::to_yaml(ele.second);
		}
		return YAML::Dump(nd);
	}
};

template<typename ValueTy>
struct ConversionTraits<std::unordered_map<String, ValueTy>, String> {
	using value_t  = ValueTy;
	using target_t = std::unordered_map<String, value_t>;
	using source_t = String;

	static target_t from_yaml(const source_t &source) {
		target_t res;
		YAML::Node node = YAML::Load(source);
		for (const auto &iter: node) {
			String key	  = iter.first.IsScalar() ? iter.first.Scalar() : Dump(iter.first);
			value_t value = ConversionTraits<value_t, source_t>::from_yaml(Dump(iter.second));
			res.emplace_back(key, value);
		}
		return res;
	}
};


// 通用转换器
template<typename TargetTy, typename SourceTy>
TargetTy Convert(const SourceTy &source) {
	using target_t = TargetTy;
	using source_t = SourceTy;

	try {
		return ConversionTraits<target_t, source_t>::convert(source);
	} catch (const boost::bad_lexical_cast &e) {
		MYLIB_THROW(std::string("convert error: ") + e.what());
	} catch (...) {
		MYLIB_THROW("convert error: unknow.");
	}
	return {};
}


template<typename SourceTy>
String ToYamlConvert(const SourceTy &source) {
	using target_t = String;
	using source_t = SourceTy;

	try {
		return ConversionTraits<target_t, source_t>::to_yaml(source);
	} catch (const boost::bad_lexical_cast &e) {
		MYLIB_THROW(std::string("convert error: ") + e.what());
	} catch (...) {
		MYLIB_THROW("convert error: unknow.");
	}
	return {};
}

template<typename TargetTy>
TargetTy FromYamlConvert(const String &source) {
	using target_t = TargetTy;
	using source_t = String;

	try {
		return ConversionTraits<target_t, source_t>::from_yaml(source);
	} catch (const boost::bad_lexical_cast &e) {
		MYLIB_THROW(std::string("convert error: ") + e.what());
	} catch (...) {
		MYLIB_THROW("convert error: unknow.");
	}
	return {};
}

MYLIB_END

#endif
