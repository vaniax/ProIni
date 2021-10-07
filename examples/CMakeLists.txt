set(TARGET_NAME example.ini)

include_directories(
        ../src
)

set(SOURCE_FILES
        main.cpp
        MappedImage.h
        MappedImage.cpp
        INIFileProperty.h
        INIFileProperty.cpp
        Types.hpp
        )

if (UNIX)
    set(LINK_LIBS ${LINK_LIBS} pthread dl)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wl,--no-as-needed)
endif(UNIX)

set(LINK_LIBS
        ProIni
        )

add_executable (${TARGET_NAME} ${TARGET_TYPE} ${SOURCE_FILES})
target_link_libraries(${TARGET_NAME} PRIVATE ${LINK_LIBS})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



#install(TARGETS ${PROJECT_NAME}
#        LIBRARY DESTINATION ${DEST_LIBRARY_DIR_CONFIG}
#        RUNTIME DESTINATION ${DEST_BIN_DIR_CONFIG}
#        ARCHIVE DESTINATION ${DEST_ARCHIVE_DIR_CONFIG}
#        BUNDLE  DESTINATION ${DEST_BIN_DIR_CONFIG}
#        )
