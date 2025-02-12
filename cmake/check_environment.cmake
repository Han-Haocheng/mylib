function(check_environment)
    message( ">> 环境")
    message("\t\t工具文件路径：${CMAKE_TOOLCHAIN_FILE}")
    message( "\t\t系统：${CMAKE_SYSTEM_NAME}")
    message( "\t\t配置：${CMAKE_BUILD_TYPE}" )
    message( "\t\tC编译器：${CMAKE_C_COMPILER_ID}" )
    message( "\t\tC标准：${CMAKE_C_STANDARD}" )
    message( "\t\tC++编译器：${CMAKE_CXX_COMPILER_ID}" )
    message( "\t\tC++标准：${CMAKE_CXX_STANDARD}" )
endfunction(check_environment)
