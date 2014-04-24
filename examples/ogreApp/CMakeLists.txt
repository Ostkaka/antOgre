project(ogreTestApp)

set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

# all source files
FILE( GLOB SOURCE ${SRCROOT}/*.cpp ${SRCROOT}/*.hpp ${SRCROOT}/*.h)

# define the demo target
add_executable(ogreApp ${SOURCE})

# link dependecies
target_link_libraries(ogreApp ${ANTCORE_TAGET_NAME} ${ANTOGRE_TARGET_NAME} ${ANTLUA_TARGET_NAME})

# properties
SET_PROPERTY(TARGET ogreApp PROPERTY FOLDER "examples")

# install target
install(TARGETS ogreApp RUNTIME DESTINATION examples/ogreApp COMPONENT examples)
