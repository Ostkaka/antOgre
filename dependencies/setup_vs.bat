@echo off

set VS_BUILDTYPE=x86_amd64

REM Get VS path 
set VC_DIR=%VS120COMNTOOLS:Common7\Tools\=%

::echo 
set VC_BAT="%VC_DIR%\VC\vcvarsall.bat"
IF EXIST %VC_BAT% (
	GOTO done
)

IF NOT EXIST %VC_BAT% (
	echo "Could not find VS2013 environment"
	goto exit
)

:done
call %VC_BAT% %VS_BUILDTYPE%

set VC_INIT=1

:exit
