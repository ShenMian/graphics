@echo off
REM Copyright 2021 SMS
REM License(Apache-2.0)

REM 切换到主目录
pushd %~dp0\..
if not exist Math.sln set firstGenerate=true

REM 生成VS解决方案
Thirdparty\premake\bin\premake5.exe vs2019
if errorlevel 1 pause

REM 打开解决方案
if "%firstGenerate%"=="true" start Math.sln

popd