#include "define.h"

#include <cstring>

MYLIB_BEGIN

String FormatClassName(CString fullFuncName) {
	if (fullFuncName == nullptr || strlen(fullFuncName) == 0) return "";

	CString lastSpace = strrchr(fullFuncName, ' ');
	CString lastColon = strrchr(fullFuncName, ':');

	if (lastSpace == nullptr || lastColon == nullptr || lastSpace >= lastColon) return "";

	size_t length = lastColon - lastSpace - 1;
	return {lastSpace + 1, length};
}

MYLIB_END
