#  CMake master file for ANT
#

# Make sure they are using a recent version of CMake
cmake_minimum_required(VERSION 2.8)

# Project name
project(antDependencies)

# Include the macros file for this project
#include(${PROJECT_SOURCE_DIR}/cmake/antMacros.cmake)
#include(${PROJECT_SOURCE_DIR}/cmake/HandleDependencies.cmake)

# Enable ExternalProject CMake module
INCLUDE(ExternalProject)

# Set default ExternalProject root directory
set(EP_PREFIX ${CMAKE_BINARY_DIR})
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${EP_PREFIX})


set( INSTALLED_DIR ${PROJECT_SOURCE_DIR}/../installed)

# Visual studio specific
if (MSVC)
	#add_definitions(-DUNICODE -D_UNICODE)
	set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif(MSVC)

# TODO get dependencies and compile them and install for ogre to find!
ExternalProject_Add(
    ogreDeps
    HG_REPOSITORY https://bitbucket.org/cabalistic/ogredeps
    TIMEOUT 10  
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
   	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${INSTALLED_DIR}
    # Wrap download, configure and build steps in a script to log output	    
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

SET_PROPERTY(TARGET ogreDeps PROPERTY FOLDER "external")

set(OGRE_DEPENDENCIES_DIR ${CMAKE_BINARY_DIR}/installed)

# Add Ogre
ExternalProject_Add(
    ogre 
    HG_REPOSITORY http://bitbucket.org/sinbad/ogre/ 
    HG_TAG v1-9
    TIMEOUT 10  
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
   	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${INSTALLED_DIR} -DOGRE_DEPENDENCIES_DIR:PATH=${INSTALLED_DIR}
    # Wrap download, configure and build steps in a script to log output	    
    UPDATE_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

SET_PROPERTY(TARGET ogre PROPERTY FOLDER "external")

# Add zlib
ExternalProject_Add(
    zlib
    GIT_REPOSITORY https://github.com/madler/zlib.git
    TIMEOUT 10  
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${INSTALLED_DIR}
    # Wrap download, configure and build steps in a script to log output
    # Disable install step
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

SET_PROPERTY(TARGET zlib PROPERTY FOLDER "external")

# Add glm
ExternalProject_Add(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    TIMEOUT 10  
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${INSTALLED_DIR} -DGLM_TEST_ENABLE=ON
    # Wrap download, configure and build steps in a script to log output
    # Disable install step
    BUILD_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

SET_PROPERTY(TARGET zlib PROPERTY FOLDER "external")

# Add bullet
ExternalProject_Add(
    bullet
    GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
    TIMEOUT 10  
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${INSTALLED_DIR} -DINSTALL_LIBS=ON -BUILD_BULLET3=OFF
    # Wrap download, configure and build steps in a script to log output
    # Disable install step    
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

SET_PROPERTY(TARGET bullet PROPERTY FOLDER "external")

ADD_DEPENDENCIES(ogre glm)

ADD_DEPENDENCIES(ogre zlib)

ADD_DEPENDENCIES(ogre ogreDeps)
