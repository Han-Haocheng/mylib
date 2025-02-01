# 定义一个函数来初始化模块
function(initialize_module MODEL_NAME MODEL_TYPE)
    message( ">> 模块 <${MODEL_NAME}> 创建中")

    message( ">>>>> 模块 <${MODEL_NAME}> 文件分组")

    file(GLOB_RECURSE MODEL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
    file(GLOB_RECURSE MODEL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
    source_group("header files" FILES ${MODEL_HEADERS})
    source_group("source files" FILES ${MODEL_SOURCES})

    message( ">>>>>>> 头文件:")
    foreach (SOURCE_FILE ${MODEL_HEADERS})
        string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" SOURCE_FILE ${SOURCE_FILE})
        message( "\t\t[${SOURCE_FILE}]")
    endforeach ()

    message( ">>>>>>> 源文件:")
    foreach (SOURCE_FILE ${MODEL_SOURCES})
        string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" SOURCE_FILE ${SOURCE_FILE})
        message( "\t\t[${SOURCE_FILE}]")

    endforeach ()


    if (${MODEL_TYPE} STREQUAL exe)
        add_executable(${MODEL_NAME} ${MODEL_SOURCES})
    elseif (${MODEL_TYPE} STREQUAL lib)
        add_library(${MODEL_NAME} STATIC ${MODEL_SOURCES})
    elseif (${MODEL_TYPE} STREQUAL dll)
        add_library(${MODEL_NAME} SHARED ${MODEL_SOURCES})
    else ()
        message(WARNING "MODEL_TYPE:${MODEL_TYPE} is unknown.MODEL_TYPE must be one of in <lib>, <exe> , <dll>")
    endif ()

    message( ">>>> 模块<${MODEL_NAME}>安装")

    # 安装主目录
    install(TARGETS ${MODEL_NAME}
            EXPORT ${PROJECT_NAME}Targets
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
            PUBLIC_HEADER DESTINATION include/${MODEL_NAME}
    )

    # 查找并安装二级目录
    message( ">>>>>> 二级目录")
    file(GLOB TMP_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/* LIST_DIRECTORIES true)
    foreach(DIR ${TMP_DIRS})
        if(IS_DIRECTORY ${DIR})
            string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" DIR ${DIR})
            install(DIRECTORY ${DIR} DESTINATION include/${MODEL_NAME} FILES_MATCHING PATTERN "*.h")
            message( \t\t[${DIR}])
        endif()
    endforeach()

    # 查找并安装二级文件
    message( ">>>>>> 二级文件")
    file(GLOB TMP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
    foreach(DIR ${TMP_HEADERS})
        if (NOT IS_DIRECTORY ${DIR})
            string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" DIR ${DIR})
            install(FILES ${DIR} DESTINATION include/${MODEL_NAME})
            message( \t\t[${DIR}])
        endif ()
    endforeach ()

    message( ">> 模块 <${MODEL_NAME}> 创建完成.")
endfunction()