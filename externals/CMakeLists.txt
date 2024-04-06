# glad
add_subdirectory(glad)

# glfw
add_subdirectory(glfw)

# glm
add_subdirectory(glm)

# json
add_subdirectory(json)

# assimp
add_subdirectory(assimp)

# imgui
add_library(imgui
    imgui/imconfig.h
    imgui/imgui.cpp
    imgui/imgui.h
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_internal.h
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h

    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_glfw.h
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_opengl3.h
)
target_include_directories(imgui PUBLIC "imgui/")
target_include_directories(imgui PUBLIC "imgui/backends/")
target_link_libraries(imgui PRIVATE glfw)

# stb_image
add_library(stb INTERFACE stb/stb_image.h stb/stb_include.h)
target_include_directories(stb INTERFACE "stb/")
