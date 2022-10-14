#!/bin/bash
# Copyright 2021 SMS
# License(Apache-2.0)

# 切换到主目录
cd "$( cd "$( dirname "$0"  )" && pwd  )" || exit
cd .. || exit

# 签出第三方库
git submodule update --init

./deps/premake/bin/premake5 gmake2
