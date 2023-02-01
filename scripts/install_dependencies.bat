@echo off
REM Copyright 2022 ShenMian
REM License(Apache-2.0)

set "build_path=target"

conan --version >nul 2>&1 || (
    echo === Need conan.
    exit /b 1
)

cd %~dp0\.. || exit /b 1

set "build_type=%~1"
if "%build_type%"=="Debug" (set "msvc_args=MTd") else (set "msvc_args=MT")

echo === Installing dependencies...
set "CONAN_SYSREQUIRES_MODE=enabled"
conan install . -pr:b=default --build=missing -if %build_path% -of %build_path% -s build_type=%build_type% -s compiler.runtime=%msvc_args% || (
    echo === Failed to install.
    exit /b 1
)