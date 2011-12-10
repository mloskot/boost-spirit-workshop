@echo off
REM ### CONFIGURATION BEGIN ###
set PRJ=boost-spirit-workshop
set DIR=D:\dev\boost\spirit
set SRC=%DIR%\%PRJ%
set BLD=%SRC%\build
set BOOST=D:\dev\boost\_svn\trunk
set G="Visual Studio 11"
REM ### CONFIGURATION END   ###
IF NOT EXIST %BLD% mkdir %BLD%
cd %SRC% 
cd %BLD%
cmake -G %G% -DBOOST_ROOT=%BOOST% %SRC%
CALL fix_vs11_sln.bat %PRJ%.sln
cd %SRC%