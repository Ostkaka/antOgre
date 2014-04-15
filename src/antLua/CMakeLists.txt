#Add the core library include header
set(CORE_HEADER ${ANT_INCLUDE_DIR}/core_include.hpp)

MESSAGE ( ${CORE_HEADER} )

# core library additional include files
FILE( GLOB_RECURSE 
	INCLUDES
    ${ANT_INCLUDE_DIR}/ant/*.hpp
)

FILE( GLOB_RECURSE
	SOURCES
	${ANT_SOURCE_DIR}/ant/*.cpp
) 

# add ANT library
add_library(${ANTLUA_TARGET_NAME} ${CORE_HEADER} ${INCLUDES} ${SOURCES} ${ANT_INCLUDE_DIR}/ant_config.hpp)

target_link_libraries(${ANTLUA_TARGET_NAME} ${TINYXML_TARGET_NAME} ${LUAPLUSPLUS_TARGET_NAME} ${OGRE_LIBS}) 

# Set properties for antCore
SET_TARGET_PROPERTIES(${ANTLUA_TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)
SET_PROPERTY(TARGET ${ANTLUA_TARGET_NAME} PROPERTY FOLDER "antLua")

