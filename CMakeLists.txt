# 设置CMake最低版本要求
cmake_minimum_required(VERSION 3.22)

# 将路径转换为CMake风格的路径
file(TO_CMAKE_PATH "H:/Documents/program/c++/vcpkg" VCPKG_ROOT)
# 设置CMake工具链文件，用于vcpkg包管理器
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
# 打印工具链文件的路径
message(STATUS ">>>>>>>${CMAKE_TOOLCHAIN_FILE}")

# 设置项目版本号
set(Mylib_VEERSION 0.4.0.20241117)
# 定义项目名称和版本
project(mylib VERSION ${Mylib_VEERSION})

include(cmake/initialize_module.cmake)

# 设置C++标准为17
set(CMAKE_CXX_STANDARD 17)
# 要求C++标准必须是17
set(CMAKE_CXX_STANDARD_REQUIRED True)
# 禁用C++标准扩展
set(CMAKE_CXX_EXTENSIONS False)

# 如果是MSVC编译器，则添加编译器标志以禁用特定警告
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
endif()

# 查找Boost库，需要lexical_cast组件
find_package(Boost REQUIRED COMPONENTS lexical_cast)
# 查找yaml-cpp库，使用CONFIG模式
find_package(yaml-cpp CONFIG REQUIRED)
# 查找jsoncpp库，使用CONFIG模式
find_package(jsoncpp CONFIG REQUIRED)

set(PROJECT_MODEL_LIST CACHE INTERNAL "${PROJECT_NAME} model list")

# 添加子目录，构建mylib/core
add_subdirectory(mylib/core)
# 添加子目录，构建mylib/task
add_subdirectory(mylib/task)
# 添加子目录，构建mylib/config
add_subdirectory(mylib/config)
# 添加子目录，构建mylib/log
add_subdirectory(mylib/log)
# 添加子目录，构建测试
add_subdirectory(mylib/tests)

# 设置安装前缀为当前源码目录下的out文件夹
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/out)

install(TARGETS ${PROJECT_MODEL_LIST}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)

foreach(MODEL_NAME ${PROJECT_MODEL_LIST})
    message(STATUS >>>>>>>model:${MODEL_NAME})


    install(TARGETS ${MODEL_NAME}
            EXPORT MylibTargets
            PUBLIC_HEADER DESTINATION include/${MODEL_NAME}
    )
endforeach ()

# 安装导出目标，用于其他项目使用Mylib
install(EXPORT MylibTargets
        FILE MylibConfig.cmake
        NAMESPACE Mylib::
        DESTINATION lib/cmake/${PROJECT_NAME}
)

# 包含CMake包配置帮助模块
include(CMakePackageConfigHelpers)

# 写入基础包版本文件
write_basic_package_version_file(
        MylibConfigVersion.cmake
        VERSION ${Mylib_VEERSION}
        COMPATIBILITY AnyNewerVersion
)

# 安装版本配置文件
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/MylibConfigVersion.cmake"
        DESTINATION lib/cmake/${PROJECT_NAME}
)

# 配置包配置文件
configure_package_config_file(
        MylibConfig.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/MylibConfig.cmake"
        INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
        PATH_VARS CMAKE_INSTALL_PREFIX
)

# 安装配置文件
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/MylibConfig.cmake"
        DESTINATION lib/cmake/${PROJECT_NAME}
)
