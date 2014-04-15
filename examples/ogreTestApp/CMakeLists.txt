project(ogreTestApp)

set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

# all source files
FILE( GLOB SOURCE ${SRCROOT}/*.cpp ${SRCROOT}/*.hpp ${SRCROOT}/*.h)

# define the demo target
add_executable(ogreTestApp ${SOURCE})

# link dependecies
target_link_libraries(ogreTestApp ${OGRE_LIBS})

# properties
SET_PROPERTY(TARGET ogreTestApp PROPERTY FOLDER "examples")

# install target
install(TARGETS ogreTestApp RUNTIME DESTINATION examples/ogreTestApp COMPONENT examples)
