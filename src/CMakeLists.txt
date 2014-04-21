# add the ANT sources path
include_directories(${PROJECT_SOURCE_DIR}/src)

# set the output directory for ANT libraries 
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# ANT needs to export symbols when build so define the export flag
add_definitions(-DANT_EXPORTS)

# core library additional include files
FILE( GLOB_RECURSE 
	INCLUDES
    ${ANT_INCLUDE_DIR}/ant/*.hpp
    ${ANT_INCLUDE_DIR}/actors/*.hpp
)

FILE( GLOB_RECURSE
	SOURCES
	${ANT_SOURCE_DIR}/ant/*.cpp
	${ANT_SOURCE_DIR}/actors/*.cpp
) 

# add ANT library
add_library(${ANTCORE_TARGET_NAME} ${INCLUDES} ${SOURCES} ${ANT_INCLUDE_DIR}/ant_config.hpp)

target_link_libraries(${ANTCORE_TARGET_NAME} ${TINYXML_TARGET_NAME} ${LUAPLUSPLUS_TARGET_NAME} ${ZLIB_LIBRARY} ${OGRE_LIBS}) 

# Set properties for antCore
SET_TARGET_PROPERTIES(${ANTCORE_TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)
SET_PROPERTY(TARGET ${ANTCORE_TARGET_NAME} PROPERTY FOLDER "antCore")

# install Config header include file
install(FILES ${ANT_INCLUDE_DIR}/ant_config.hpp
        DESTINATION include/ant
        COMPONENT dev)


# add the modules subdirectories
add_subdirectory(antLua)