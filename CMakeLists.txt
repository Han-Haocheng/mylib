# 设置CMake最低版本要求
cmake_minimum_required(VERSION 3.22)

## 将路径转换为CMake风格的路径
file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT)
set(VCPKG_ROOT "F:/lib_vcpkg")
## 设置CMake工具链文件，用于vcpkg包管理器
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE STRING "")
#set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(mylib VERSION 0.4.0.20241117 LANGUAGES C CXX)

# 设置C++标准为17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

set(CMAKE_SCRIPT_DIR "${CMAKE_SOURCE_DIR}/cmake")
file(GLOB CMAKE_SCRIPT_FILES "${CMAKE_SCRIPT_DIR}/*.cmake")
foreach(SCRIPT_FILE ${CMAKE_SCRIPT_FILES})
    include(${SCRIPT_FILE})
endforeach()

# 包含CMake包配置帮助模块
include(CMakePackageConfigHelpers)

add_definitions(-DYAML_CPP_API)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG=1)
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_definitions(-D_WIN32_WINNT=0x0600)
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
endif ()
set(PROJECT_MODEL_LIST CACHE INTERNAL "${PROJECT_NAME} model list")
# 设置安装前缀为当前源码目录下的out文件夹
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE})


find_package(Boost CONFIG REQUIRED COMPONENTS lexical_cast)
find_package(Yaml-cpp CONFIG REQUIRED)
find_package(Jsoncpp CONFIG REQUIRED)
find_package(Gtest CONFIG REQUIRED)

check_packages()


add_subdirectory(src)
add_subdirectory(tests)

install(DIRECTORY cmake/
         DESTINATION lib/cmake
         FILES_MATCHING PATTERN "*.cmake"
)

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

