add_executable(texture-editor
    main.cpp
    mesh.cpp
    mesh.h
)
target_link_libraries(texture-editor PRIVATE beamlib OpenMeshCore OpenMeshTools)

if(UNIX)
    target_compile_options(texture-editor PRIVATE "-Wall" "-Wextra" "-ggdb")
endif()
