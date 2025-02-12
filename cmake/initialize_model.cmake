# 定义一个函数来初始化模块
function(initialize_model MODEL_NAME MODEL_SOURCE_DIR)
    message( ">> 模块 <${MODEL_NAME}> 创建")

    message( ">>>> 模块 <${MODEL_NAME}> 文件分组")

    file(GLOB_RECURSE HEADERS ${MODEL_SOURCE_DIR}/*.h)
    file(GLOB_RECURSE SOURCES ${MODEL_SOURCE_DIR}/*.cpp)
    source_group("header files" FILES ${HEADERS})
    source_group("source files" FILES ${SOURCES})

    message( ">>>>>> 头文件:")
    foreach (full_file ${HEADERS})
        message( "\t\t[${full_file}]")
    endforeach ()

    message( ">>>>>> 源文件:")
    foreach (full_file ${SOURCES})
        message( "\t\t[${full_file}]")
    endforeach ()
    set(MODEL_SOURCES ${SOURCES} PARENT_SCOPE)
endfunction()