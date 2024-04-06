add_library(beamlib beamlib.cpp beamlib.h)
target_include_directories(beamlib PUBLIC ".")
target_link_libraries(beamlib PRIVATE glad glfw imgui glm nlohmann_json stb)

if(UNIX)
target_compile_options(beamlib PRIVATE "-Wall" "-Wextra" "-ggdb")
endif()
