add_executable(toon
    main.cpp
)
target_link_libraries(toon PRIVATE beamlib glm assimp)

if(UNIX)
    target_compile_options(toon PRIVATE "-Wall" "-Wextra" "-ggdb")
endif()
