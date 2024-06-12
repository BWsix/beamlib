add_executable(water
    main.cpp
    loader.h
    mesh.h
)

target_link_libraries(water PRIVATE beamlib glm assimp)

if(UNIX)
    target_compile_options(water PRIVATE "-Wall" "-Wextra" "-ggdb")
endif()
