add_subdirectory(beamlib)

add_executable(editor main.cpp shaderProgramStore.cpp)
target_link_libraries(editor PRIVATE glad glfw glm imgui nlohmann_json beamlib)

if(UNIX)
target_compile_options(beamlib PRIVATE "-Wall" "-Wextra" "-ggdb")
endif()
