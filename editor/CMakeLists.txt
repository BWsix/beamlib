add_executable(editor main.cpp)
target_link_libraries(editor PRIVATE glad glfw glm imgui nlohmann_json beamlib assimp stb beamlib)

if(UNIX)
target_compile_options(editor PRIVATE "-Wall" "-Wextra" "-ggdb")
endif()
