@echo off
REM Copyright 2022 ShenMian
REM License(Apache-2.0)

set "build_path=target"

cmake --version >nul 2>&1 || (
    echo === Need cmake.
    exit /b 1
)

if "%~1"=="" (set "build_type=Debug") else (set "build_type=%~1")

cd %~dp0\.. || exit /b 1

mkdir build 2>nul

call .\scripts\install_dependencies.bat %build_type% || exit /b 1

echo === Generating CMake cache...
cmake -Wno-dev -DCMAKE_build_type=%BUILD_TYPE% -B %build_path% >nul || (
    echo === Failed to generate CMake cache.
    exit /b 1
)

echo === Generating 'compile_commands.json'...
xcopy %build_path%\compile_commands.json . 2>nul || echo No 'compile_commands.json' was generated.

echo === Building...
cmake --build %build_path% --config "%build_type%" -j18 || (
    echo === Failed to build.
    exit /b 1
)

echo === Done.
