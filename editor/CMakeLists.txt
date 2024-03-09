add_subdirectory(beamlib)

add_executable(editor main.cpp)
target_link_libraries(editor PRIVATE glad glfw glm::glm imgui nlohmann_json beamlib)
