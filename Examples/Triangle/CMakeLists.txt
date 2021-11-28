# Copyright 2021 SMS
# License(Apache-2.0)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE Graphics assimp glad)

file(COPY Shaders DESTINATION ".")
