project(imgui)

file(GLOB SRC_FILES imgui/*.cpp)

set(BACKEND_SRC_FILES imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)

add_library(imgui ${SRC_FILES} ${BACKEND_SRC_FILES})

include_directories(imgui
    PUBLIC ../glfw/include
)

target_include_directories(imgui
    PUBLIC imgui/
    PUBLIC imgui/backends
)





