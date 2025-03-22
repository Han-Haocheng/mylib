function(install_model MODEL_NAME)
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
endfunction()