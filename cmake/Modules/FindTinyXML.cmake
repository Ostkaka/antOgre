# - Find TinyXML
# Find the native TinyXML includes and library
#
#   TINYXML_FOUND       - True if TinyXML found.
#   TINYXML_INCLUDE_DIR - where to find tinyxml.h, etc.
#   TINYXML_LIBRARY   - TinyXML library.
#
# Set ENV TINYXML_DIR to add a search path

FIND_PATH(TINYXML_INCLUDE_DIR tinyxml.h
    $ENV{TINYXML_DIR}/include
    $ENV{TINYXML_DIR}/include/tinyxml
    $ENV{TINYXML_DIR}
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/include
    /usr/include
    /sw/include # Fink
    /opt/local/include # DarwinPorts
    /opt/csw/include # Blastwave
    /opt/include
    /usr/freeware/include
)

FIND_LIBRARY(TINYXML_LIBRARY 
    NAMES tinyxml
    PATHS
    $ENV{TINYXML_DIR}/lib
    $ENV{TINYXML_DIR}
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/lib
    /usr/lib
    /sw/lib
    /opt/local/lib
    /opt/csw/lib
    /opt/lib
    /usr/freeware/lib64
)

SET(TINYXML_FOUND "NO")
IF(TINYXML_LIBRARY AND TINYXML_INCLUDE_DIR)
    SET(TINYXML_FOUND "YES")
ENDIF(TINYXML_LIBRARY AND TINYXML_INCLUDE_DIR)