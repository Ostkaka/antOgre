# For make-based builds, defines make target named test.
# For Visual Studio builds, defines Visual Studio project named RUN_TESTS.
project(unittests)

set(SRCROOT ${PROJECT_SOURCE_DIR})

# all source files
FILE(GLOB GTEST_SOURCE ${SRCROOT}/*.cpp ${SRCROOT}/*.hpp)

# Add gtest
ExternalProject_Add(
    googletest
    SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/
    TIMEOUT 10
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO:PATH=ReleaseLibs 
               -DCMAKE_CXX_FLAGS=${MSVC_COMPILER_DEFS}
               -Dgtest_force_shared_crt=ON
    # Disable install step
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

# add include paths of external libraries
include_directories(${PROJECT_SOURCE_DIR})

SET_PROPERTY(TARGET googletest PROPERTY FOLDER "external")

# Specify include dir
ExternalProject_Get_Property(googletest source_dir)
INCLUDE_DIRECTORIES(${source_dir}/include)

# Get em all
FILE(GLOB UNIT_TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/unittest_*.cpp)

# Loop every cpp file and create unittests
FOREACH(TESTFILE ${UNIT_TEST_FILES})

  # Get the name without the extension
  GET_FILENAME_COMPONENT(NAME ${TESTFILE} NAME_WE)

  # define the demo target
  add_executable(${NAME} ${TESTFILE} ${PROJECT_SOURCE_DIR}/unittests.hpp)

  # Create dependency of MainTest on googletest
  ADD_DEPENDENCIES(${NAME} googletest)

  # Specify MainTest's link libraries
  ExternalProject_Get_Property(googletest binary_dir)
  TARGET_LINK_LIBRARIES(${NAME}
                        debug ${binary_dir}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
                        optimized ${binary_dir}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES})

  # Link dependencies
  target_link_libraries(${NAME} ${ANT_LIBS})

  # test!
  add_test( runUnitTests ${NAME})

  SET_PROPERTY(TARGET ${NAME} PROPERTY FOLDER "unittests")

ENDFOREACH(TESTFILE ${UNIT_TEST_FILES})