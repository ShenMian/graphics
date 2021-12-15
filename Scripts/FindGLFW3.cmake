# 定位 GLFW3 库.
#
# 该模块将定义以下变量:
#   GLFW3_FOUND                           是否找到 GLFW3 库.
#   GLFW3_INCLUDE_DIR, GLFW3_INCLUDE_DIRS 头文件路径.
#   GLFW3_LIBRARY, GLFW3_LIBRARIES        库文件路径.
#
# 可以在 GLFW3_ROOT 中指定要额外搜索的 GLFW 库的安装目录.

set(_GLFW3_HEADER_SEARCH_DIRS
  "/usr/include"
  "/usr/local/include"
  "${CMAKE_SOURCE_DIR}/includes"
  "C:/Program Files (x86)/GLFW/include")

set(_GLFW3_LIB_SEARCH_DIRS
  "/usr/lib"
  "/usr/local/lib"
  "${CMAKE_SOURCE_DIR}/lib"
  "C:/Program Files (x86)/GLFW/lib")

# Check environment for root search directory
set(_GLFW3_ENV_ROOT $ENV{GLFW3_ROOT})
if(NOT GLFW3_ROOT AND _GLFW3_ENV_ROOT)
	set(GLFW3_ROOT ${_GLFW3_ENV_ROOT})
endif()

# 添加用户指定的查找路径
if(GLFW3_ROOT)
	list(INSERT _GLFW3_HEADER_SEARCH_DIRS 0 "${GLFW3_ROOT}/include")
	list(INSERT _GLFW3_LIB_SEARCH_DIRS    0 "${GLFW3_ROOT}/lib")
endif()

# 查找头文件路径
find_path(GLFW3_INCLUDE_DIR
  NAME "GLFW/glfw3.h"
  PATHS ${_GLFW3_HEADER_SEARCH_DIRS})
set(GLFW3_INCLUDE_DIRS ${GLFW3_INCLUDE_DIR})

# 查找库文件路径
find_library(GLFW3_LIBRARY
  NAMES glfw3 glfw
  PATHS ${_GLFW3_LIB_SEARCH_DIRS})
set(GLFW3_LIBRARIES ${GLFW3_LIBRARY})

# 是否找到指定头文件和库文件路径
if(GLFW3_INCLUDE_DIR AND GLFW3_LIBRARY)
  set(GLFW3_FOUND TRUE)
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLFW3 DEFAULT_MSG GLFW3_LIBRARY GLFW3_INCLUDE_DIR)
