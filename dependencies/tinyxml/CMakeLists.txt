project(${TINYXML_TARGET_NAME})

include_directories(${PROJECT_SOURCE_DIR})

add_library(${TINYXML_TARGET_NAME} tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp tinyxml.h tinystr.h)

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

# Set properties for antCore
SET_TARGET_PROPERTIES(${TINYXML_TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)

SET_PROPERTY(TARGET ${TINYXML_TARGET_NAME} PROPERTY FOLDER "external")

# set to depency stuff

SET_PROPERTY(TARGET ${TINYXML_TARGET_NAME} PROPERTY FOLDER "external")

#SET_TARGET_PROPERTIES (tinyxml PROPERTIES DEFINE_SYMBOL  "EXPORT" )