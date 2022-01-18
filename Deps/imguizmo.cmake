file(GLOB SOURCES "${DEPS_DIR}/ImGuizmo/*.cpp")
add_library(imguizmo STATIC ${SOURCES})
target_include_directories(imguizmo PRIVATE
    "${DEPS_DIR}/ImGuizmo"
    "${DEPS_DIR}/imgui")
