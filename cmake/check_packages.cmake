function(check_packages)
    message( ">> 第三方库")
		set(_foundPackages "")
		get_cmake_property(_variableNames VARIABLES)
		foreach (_variableName ${_variableNames})
			# 匹配以大写字母开头，后面跟着大写字母或数字或下划线，结尾是 _FOUND 的变量名
			# 并且不是以其他 _FOUND 变量作为后缀的变量名，避免组件标志变量
			if (_variableName MATCHES "^[A-Z][a-z_\-]*_FOUND$")
				string(REGEX REPLACE "_FOUND$" "" _packageName "${_variableName}")
				list(APPEND _foundPackages "${_packageName}")
			endif ()
		endforeach ()
		# 讲结果输出
		foreach(pkg ${_foundPackages})
			message("\t\t√ ${pkg}")
		endforeach()
endfunction()
