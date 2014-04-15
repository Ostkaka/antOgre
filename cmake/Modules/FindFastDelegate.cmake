# Locate zlib
# This module defines
# ZLIB_LIBRARY
# ZLIB_FOUND, if false, do not try to link to zlib 
# ZLIB_INCLUDE_DIR, where to find the headers
#
# $ZLIB_DIR is an environment variable that would
# correspond to the ./configure --prefix=$ZLIB_DIR
# used in building zlib.
#
# Created by Ulrich Hertlein. 

FIND_PATH(FASTDELEGATE_INCLUDE_DIR FastDelegate.h FastDelegateBind.h
    $ENV{FASTDELEGATE_DIR}/include
    $ENV{FASTDELEGATE_DIR}
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

SET(FASTDELEGATE_FOUND "NO")
IF(FASTDELEGATE_INCLUDE_DIR)
    SET(FASTDELEGATE_FOUND "YES")
ENDIF(FASTDELEGATE_INCLUDE_DIR)


