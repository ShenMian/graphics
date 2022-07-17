@echo off
REM Copyright 2021 ShenMian
REM License(Apache-2.0)

cmake --version >nul 2>&1 || (
    echo === Need cmake.
    exit /b 1
)

cd %~dp0\.. || exit /b 1

echo === Generating 'compile_commands.json'...
cmake -B build/compile_db -G "Unix Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON >nul || (
    echo === Failed to generate 'compile_commands.json'.
    exit /b 1
)

copy build/compile_db/compile_commands.json .

echo === Done.
