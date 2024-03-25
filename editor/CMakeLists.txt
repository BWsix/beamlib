add_subdirectory(beamlib)

add_executable(editor main.cpp shaderProgramStore.cpp model.cpp mesh.cpp)
target_link_libraries(editor PRIVATE glad glfw glm imgui nlohmann_json beamlib assimp stb)

if(UNIX)
target_compile_options(beamlib PRIVATE "-Wall" "-Wextra" "-ggdb")
endif()
