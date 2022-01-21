@ECHO OFF
SET workspaceFolder=%~dp0
REM ECHO Workspace Folder: %workspaceFolder%

SET driver=%workspaceFolder:~0,1%
REM ECHO Workspace Driver: %driver%

SET wpath=%workspaceFolder:~3,-1%
REM ECHO Workspace windows style path: %wpath%

SET xpath=%wpath:\=/%
REM ECHO Workspace linux style path: %xpath%

SET cygpath=/cygdrive/%driver%/%xpath%
REM ECHO cygwin style full path: %cygpath%

SET buildBashScript=%cygpath%/build.sh
ECHO Cygwin Bash start shellscript: %buildBashScript%

SET buildArch=%1
SET buildMode=%2
SET buildType=%3

c:/cygwin64/bin/bash.exe --login -i -c "%buildBashScript% %buildArch% %buildMode% %buildType%"
