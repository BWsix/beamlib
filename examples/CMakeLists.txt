add_subdirectory(gumdam)
add_subdirectory(toon)
add_subdirectory(envmap)
add_subdirectory(water)

add_executable(hub
    hub.cpp
)
target_link_libraries(hub PRIVATE glad glfw glm imgui nlohmann_json assimp stb beamlib)

if(UNIX)
target_compile_options(hub PRIVATE "-Wall" "-Wextra" "-ggdb" "-O5")
endif()

