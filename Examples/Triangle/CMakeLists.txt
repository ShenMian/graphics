# Copyright 2021 SMS
# License(Apache-2.0)

include_directories(
    "${PROJECT_SOURCE_DIR}/Source"
    "${THIRDPARTY_DIR}/Math/include")

add_executable(main main.cpp)

target_link_libraries(main PRIVATE Graphics assimp glad)

file(COPY Shaders DESTINATION ".")
