@echo off
REM Copyright 2021 SMS
REM License(Apache-2.0)

REM 切换到主目录
pushd %~dp0\..

ThirdParty\premake\bin\premake5.exe gmake2

popd
