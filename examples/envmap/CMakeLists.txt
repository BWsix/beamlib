add_executable(envmap
    main.cpp
)

target_link_libraries(envmap PRIVATE beamlib glm assimp)

if(UNIX)
    target_compile_options(envmap PRIVATE "-Wall" "-Wextra" "-ggdb")
endif()
