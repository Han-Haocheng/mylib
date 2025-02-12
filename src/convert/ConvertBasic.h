#ifndef MYLIB_CONVERT_CONVERTBASIC_H_
#define MYLIB_CONVERT_CONVERTBASIC_H_

#include "../core/core.h"

#include <boost/lexical_cast.hpp>
#include <boost/lexical_cast/bad_lexical_cast.hpp>


MYLIB_BEGIN
enum class ConvertType {
	CT_DEFAULT,
	CT_YAML,
	CT_JSON,
};

template<typename TargetTy, typename SourceTy, ConvertType>
struct ConversionTraits;

template<typename Ty>
MYLIB_CONSTEXPR_TYPE ToStringBase = &ConversionTraits<String, Ty, ConvertType::CT_DEFAULT>::convert;

template<typename Ty>
MYLIB_CONSTEXPR_TYPE FromStringBase = &ConversionTraits<Ty, String, ConvertType::CT_DEFAULT>::convert;

template<typename Ty>
MYLIB_CONSTEXPR_TYPE ToYamlBase = &ConversionTraits<String, Ty, ConvertType::CT_YAML>::convert;

template<typename Ty>
MYLIB_CONSTEXPR_TYPE FromYamlBase = &ConversionTraits<Ty, String, ConvertType::CT_YAML>::convert;


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


MYLIB_END

#endif// !MYLIB_CONVERT_CONVERTBASIC_H_