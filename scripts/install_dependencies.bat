@echo off
REM Copyright 2022 ShenMian
REM License(Apache-2.0)

conan --version >nul 2>&1 || (
    echo === Need conan.
    exit /b 1
)

cd %~dp0\.. || exit /b 1

set "build_type=%~1"
if "%build_type%"=="Debug" (set "msvc_args=MTd") else (set "msvc_args=MT")

set "build_path=target/%build_type%"

echo === Installing dependencies...
set "CONAN_SYSREQUIRES_MODE=enabled"
REM -s compiler.runtime=%msvc_args%
conan install . -pr:b=default --build=missing -of "%build_path%" -s build_type=%build_type% || (
    echo === Failed to install.
    exit /b 1
)
