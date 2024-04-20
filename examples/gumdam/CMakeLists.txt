add_executable(gumdam
    main.cpp
)
target_link_libraries(gumdam PRIVATE glad glfw glm imgui nlohmann_json assimp stb beamlib)

if(UNIX)
target_compile_options(gumdam PRIVATE "-Wall" "-Wextra" "-ggdb")
endif()
