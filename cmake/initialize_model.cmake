# 定义一个函数来初始化模块
function(initialize_model MODEL_NAME MODEL_SOURCE_DIR)
    message( ">> 模块 <${MODEL_NAME}> 创建")
    message( ">>>> 模块 <${MODEL_NAME}> 文件分组")
    # 获取 CMakeLists.txt 文件的绝对路径
    GET_FILENAME_COMPONENT(CMAKE_CURRENT_LIST_DIR_ABSOLUTE ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
    # 将相对路径转换为绝对路径
    GET_FILENAME_COMPONENT(DIR ${CMAKE_CURRENT_LIST_DIR_ABSOLUTE}/${MODEL_SOURCE_DIR} ABSOLUTE)

    file(GLOB_RECURSE HEADERS ${DIR}/*.h)
    file(GLOB_RECURSE SOURCES ${DIR}/*.cpp)
    source_group("header files" FILES ${HEADERS})
    source_group("source files" FILES ${SOURCES})

    message( ">>>>>> 头文件:")
    foreach (full_file ${HEADERS})
        STRING(REPLACE "${DIR}/" "" file "${full_file}")
        message( "\t\t[${file}]")
    endforeach ()

    message( ">>>>>> 源文件:")
    foreach (full_file ${SOURCES})
        STRING(REPLACE "${DIR}/" "" file "${full_file}")
        message( "\t\t[${file}]")
    endforeach ()

    set(MODEL_SOURCES ${SOURCES} PARENT_SCOPE)
    set(MODEL_HEADERS ${HEADERS} PARENT_SCOPE)
endfunction()
