#ifndef MYLIB_CONVERT_JSONCONVERT_H_
#define MYLIB_CONVERT_JSONCONVERT_H_

#include "ConvertBasic.h"

MYLIB_BEGIN


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
#endif //!MYLIB_CONVERT_JSONCONVERT_H_