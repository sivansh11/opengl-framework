cmake_minimum_required(VERSION 3.10)

project(app)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(deps/glm)
add_subdirectory(deps/glad)
add_subdirectory(deps/glfw)
add_subdirectory(deps/imgui)
add_subdirectory(deps/stb_image)

add_executable(${PROJECT_NAME} ${SRC_FILES})

include_directories(${PROJECT_NAME}
    include
)

target_link_libraries(${PROJECT_NAME}
    glm
    glad
    glfw
    imgui
    stb_image
    pthread
    stdc++fs
)
