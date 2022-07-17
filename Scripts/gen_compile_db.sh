#!/bin/bash
# Copyright 2021 ShenMian
# License(Apache-2.0)

if ! hash "cmake" &>/dev/null; then
    echo "=== Need cmake."
    exit 1
fi

cd "$( cd "$( dirname "$0"  )" && pwd  )" || exit
cd .. || exit

echo "=== Generating 'compile_commands.json'..."
if ! cmake -B build/compile_db -G "Unix Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON >/dev/null
then
    echo "=== Failed to generate 'compile_commands.json'."
    exit 1
fi

cp build/compile_db/compile_commands.json .

echo "=== Done."
