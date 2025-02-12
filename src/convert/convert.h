//
// Created by HanHaocheng on 2025/1/29.
//
#ifndef MYLIB_CONVERT_H
#define MYLIB_CONVERT_H


#include "JsonConvert.h"
#include "YamlConvert.h"

MYLIB_BEGIN

// 转换器模板
template<typename target_t, typename source_t, ConvertType type = ConvertType::CT_DEFAULT>
static target_t Convert(const source_t &source) {
	MYLIB_TRY_CATCH_BEGIN {
		return ConversionTraits<target_t, source_t, type>::convert(source);
	}
	MYLIB_CATCH(boost::bad_lexical_cast, MYLIB_THROW(String("convert error : ") + e.what());)
	MYLIB_TRY_CATCH_END("convert error")
	return {};
}

template<typename Ty>
static String ToString(const Ty &source) {
	MYLIB_TRY_CATCH_BEGIN {
		return ToStringBase<Ty>(source);
	}
	MYLIB_CATCH(boost::bad_lexical_cast, MYLIB_THROW(String("convert error : ") + e.what());)
	MYLIB_TRY_CATCH_END("convert error")
	return {};
}

template<typename Ty>
static String FromString(const Ty &source) {
	MYLIB_TRY_CATCH_BEGIN {
		return ToStringBase<Ty>(source);
	}
	MYLIB_CATCH(boost::bad_lexical_cast, MYLIB_THROW(String("convert error : ") + e.what());)
	MYLIB_TRY_CATCH_END("convert error")
	return {};
}
template<typename Ty>
static Ty ToYaml(const String &source) {
	MYLIB_TRY_CATCH_BEGIN {
		return ToYamlBase<Ty>(source);
	}
	MYLIB_CATCH(boost::bad_lexical_cast, MYLIB_THROW(String("convert error : ") + e.what());)
	MYLIB_TRY_CATCH_END("convert error")
	return {};
}


template<typename Ty>
static Ty FromYaml(const String &source) {
	MYLIB_TRY_CATCH_BEGIN {
		return FromYamlBase<Ty>(source);
	}
	MYLIB_CATCH(boost::bad_lexical_cast, MYLIB_THROW(String("convert error : ") + e.what());)
	MYLIB_TRY_CATCH_END("convert error")
	return {};
}

MYLIB_END

#endif
