@echo off
REM Copyright 2021 SMS
REM License(Apache-2.0)

REM 切换到主目录
pushd %~dp0\..

REM 签出第三方库
git submodule update --init

if not exist Graphics.sln set firstGenerate=true

REM 生成VS解决方案
Deps\premake\bin\premake5.exe vs2019 || pause

REM 第一次生成时, 打开解决方案
if "%firstGenerate%"=="true" start Graphics.sln

popd
