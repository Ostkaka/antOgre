project(ogreTestApp)

set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

# all source files
FILE( GLOB SOURCE ${SRCROOT}/*.cpp ${SRCROOT}/*.hpp ${SRCROOT}/*.h)

# define the demo target
add_executable(ogreTestApp ${SOURCE})

message("libs: " ${OGRE_LIBRARIES})

# link dependecies
target_link_libraries(ogreTestApp ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${OGRE_Overlay_LIBRARIES} ${OGRE_Terrain_LIBRARIES})

# properties
SET_PROPERTY(TARGET ogreTestApp PROPERTY FOLDER "examples")

# install target
install(TARGETS ogreTestApp RUNTIME DESTINATION examples/ogreTestApp COMPONENT examples)
