# 设置CMake最低版本要求
cmake_minimum_required(VERSION 3.22)

## 将路径转换为CMake风格的路径
file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT)
## 设置CMake工具链文件，用于vcpkg包管理器
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
# 打印工具链文件的路径
message(STATUS ">>>>>>>${CMAKE_TOOLCHAIN_FILE}")

set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "")
#set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")

# 设置项目版本号
set(mylib_VERSION 0.4.0.20241117)
# 定义项目名称和版本
project(mylib VERSION ${mylib_VERSION})
include(cmake/initialize_module.cmake)

# 设置C++标准为17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

# 如果是MSVC编译器，则添加编译器标志以禁用特定警告
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
endif ()

find_package(Boost CONFIG COMPONENTS lexical_cast)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

set(PROJECT_MODEL_LIST CACHE INTERNAL "${PROJECT_NAME} model list")

## 添加子目录，构建mylib/core
#add_subdirectory(src/core)
## 添加子目录，构建mylib/task
#add_subdirectory(src/task)
## 添加子目录，构建mylib/config
#add_subdirectory(src/config)
## 添加子目录，构建mylib/log
#add_subdirectory(src/log)

add_subdirectory(src)
# 添加子目录，构建测试
add_subdirectory(tests)

# 设置安装前缀为当前源码目录下的out文件夹
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE})
#message(FATAL_ERROR ${CMAKE_BUILD_TYPE})

foreach (MODEL_NAME ${PROJECT_MODEL_LIST})
    message(STATUS ">>>>>>> install model<${MODEL_NAME}>")
    install(TARGETS ${MODEL_NAME}
            EXPORT ${PROJECT_NAME}Targets
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
            PUBLIC_HEADER DESTINATION include/${MODEL_NAME}
    )
endforeach ()

# 包含CMake包配置帮助模块
include(CMakePackageConfigHelpers)

# 写入基础包版本文件
write_basic_package_version_file(
        ${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${Mylib_VERSION}
        COMPATIBILITY AnyNewerVersion
)

# 安装导出目标，用于其他项目使用Mylib
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION lib/cmake/${PROJECT_NAME}
)

# 配置包配置文件
configure_package_config_file(
        ${PROJECT_NAME}Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
        PATH_VARS CMAKE_INSTALL_PREFIX
)

# 安装配置文件
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION lib/cmake/${PROJECT_NAME}
)

