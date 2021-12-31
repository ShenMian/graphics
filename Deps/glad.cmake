
add_library(glad STATIC "${DEPS_DIR}/glad/src/glad.c")
target_include_directories(glad PUBLIC "${DEPS_DIR}/glad/include")
