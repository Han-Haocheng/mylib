//
// Created by HanHaocheng on 2025/1/29.
//
#ifndef MYLIB_CONVERT_H
#define MYLIB_CONVERT_H

#include "../core.h"
#include "boost/lexical_cast.hpp"

MYLIB_BEGIN
// 通用转换模板
template<typename TargetTy, typename SourceTy>
struct ConversionTraits {
	static TargetTy convert(const SourceTy &source) {
		return boost::lexical_cast<TargetTy>(source);
	}
};


// 通用转换器
template<typename TargetTy, typename SourceTy>
TargetTy Convert(const SourceTy &source) {
	try {
		return ConversionTraits<TargetTy, SourceTy>::convert(source);
	} catch (const boost::bad_lexical_cast &e) {
		MYLIB_THROW(std::string("convert error: ") + e.what());
		//printf();
	} catch (...) {
		printf("convert error: unknown");
	}
	return {};
}

MYLIB_END

#endif
