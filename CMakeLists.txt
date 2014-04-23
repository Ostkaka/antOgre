#  CMake master file for ANT
#

# Make sure they are using a recent version of CMake
cmake_minimum_required(VERSION 2.8)

# Project name
project(antOgre)

# Include the macros file for this project
include(${PROJECT_SOURCE_DIR}/cmake/antMacros.cmake)

# Enable ExternalProject CMake module
INCLUDE(ExternalProject)

# Set default ExternalProject root directory
set(EP_PREFIX ${CMAKE_BINARY_DIR}/dependencies)
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${EP_PREFIX})

# define the path of our additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/Modules/)

# Build options
option(BUILD_EXAMPLES "If example applications should be build" ON)
option(BUILD_UNITTESTS "If unit tests should be buildt" ON)

# setup version numbers - This is not really needed
set(ANT_VERSION_MAJOR 0)
set(ANT_VERSION_MINOR 22)
set(ANT_VERSION_PATCH 4)
set(ANT_VERSION "${ANT_VERSION_MAJOR}.${ANT_VERSION_MINOR}.${ANT_VERSION_PATCH}")

# Set directory paths for data
set( ANT_DATA_PATH        "${PROJECT_SOURCE_DIR}/data" )
set( OGRE_MEDIA_PATH      "${PROJECT_SOURCE_DIR}/installed/media" )
set( ANT_DEPENDENCIES_DIR "${PROJECT_SOURCE_DIR}/dependencies" )

# Properties
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

set( ANT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set( ANT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set( INSTALLED_DIR ${PROJECT_SOURCE_DIR}/installed)

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin PATH "Installation directory for executables")
set(INSTALL_DATA_DIR bin PATH "Installation directory for data")
set(INSTALL_INCLUDE_DIR include PATH  "Installation directory for header files")

# Set project targets
SET( ANTCORE_TARGET_NAME "antCore")
SET( ANTLUA_TARGET_NAME "antLua")
SET( ANTOGRE_TARGET_NAME "antOgre")

# Visual studio specific
if (MSVC)
	add_definitions(-DUNICODE -D_UNICODE)
	set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif(MSVC)

CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/configuration/setup_env.bat.in ${CMAKE_CURRENT_SOURCE_DIR}/setup_env.bat @ONLY )
CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/configuration/setup_vs.bat.in  ${CMAKE_CURRENT_SOURCE_DIR}/setup_vs.bat @ONLY )

# Do configure file
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/include/ant_config.hpp.in"
  "${PROJECT_BINARY_DIR}/include/ant_config.hpp"
  )

configure_file (
  "${PROJECT_SOURCE_DIR}/data/resources.cfg.in"
  "${PROJECT_BINARY_DIR}/data/resources.cfg"
  )

configure_file (
  "${PROJECT_SOURCE_DIR}/data/plugins.cfg.in"
  "${PROJECT_BINARY_DIR}/data/plugins.cfg"
  )

# add include paths of external libraries
#include_directories(${PROJECT_SOURCE_DIR}/installed/include)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/installed/CMake ${PROJECT_SOURCE_DIR}/installed/CMake/Utils)

include_directories(${PROJECT_SOURCE_DIR}/installed/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

# shall we do FindOgre here?  Set Ogre home
SET( OGRE_HOME ${PROJECT_SOURCE_DIR}/installed)
find_package(OGRE) 
if(OGRE_FOUND)
  set(OGRE_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/installed/include/OGRE)
  include_directories( 
    ${OGRE_INCLUDE_DIRS}  
    ${OGRE_Overlay_INCLUDE_DIRS}
  )
else() 
  message("Ogre not found!")      
endif(OGRE_FOUND)

find_package(OIS REQUIRED)
if(NOT OIS_FOUND)
    message(SEND_ERROR "Failed to find OIS.")
else()
    include_directories( 
      ${OIS_INCLUDE_DIRS}  
    )
    set( OGRE_LIBS ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${OGRE_Overlay_LIBRARIES} ${OGRE_Terrain_LIBRARIES} ) 
endif()


# TinyXML
set(TINYXML_TARGET_NAME "TinyXml")
include_directories(${ANT_DEPENDENCIES_DIR}/tinyxml)
add_subdirectory(${ANT_DEPENDENCIES_DIR}/tinyxml)

# luaplusplus
set( LUAPLUSPLUS_TARGET_NAME "luaplusplus" )
include_directories(${ANT_DEPENDENCIES_DIR}/luaplusplus)
add_subdirectory(${ANT_DEPENDENCIES_DIR}/luaplusplus)

# find fastdelegate
set(ENV{FASTDELEGATE_DIR} ${ANT_DEPENDENCIES_DIR}/FastDelegate)
find_package(FASTDELEGATE)
if(FASTDELEGATE_FOUND)
  message("FASTDELEGATET: FOUND")  
else()
  message(FATAL_ERROR "FASTDELEGATE: NOT FOUND")
endif(FASTDELEGATE_FOUND)
include_directories(${FASTDELEGATE_INCLUDE_DIR})

# find fastdelegate
set(ENV{FASTDELEGATE_DIR} ${ANT_DEPENDENCIES_DIR}/FastDelegate)
find_package(FASTDELEGATE)
if(FASTDELEGATE_FOUND)
  message("FASTDELEGATET: FOUND")  
else()
  message(FATAL_ERROR "FASTDELEGATE: NOT FOUND")
endif(FASTDELEGATE_FOUND)
include_directories(${FASTDELEGATE_INCLUDE_DIR})

# find glm
set(GLM_ROOT_DIR ${INSTALLED_DIR})
find_package(GLM)
if(GLM_FOUND)
  message("GLM: FOUND")  
  include_directories(${GLM_INCLUDE_DIR})
else()
  message(FATAL_ERROR "GLM: NOT FOUND")
endif(GLM_FOUND)

# add the ANT subdirectory
message("Configuring ANT...")

ADD_SUBDIRECTORY(src)

# add the examples subdirectory
if(BUILD_EXAMPLES)
  message("Configuring examples...")
    add_subdirectory(examples)
endif()

# add the unittest subdirectory
if(BUILD_UNITTESTS)
  message("Configuring unittests...")
  add_subdirectory(unittests)
endif()