
include_directories("${DEPS_DIR}/glad/include")
add_library(glad STATIC "${DEPS_DIR}/glad/src/glad.c")
