add_subdirectory(beamlib)

add_executable(editor main.cpp)

#   App.cpp
#   App.h
#   Camera.hpp
#   main.cpp
#   Objects.hpp
#   Scene.hpp
#   ShaderProgram.hpp
#   UI.hpp
#   Utils.hpp
#   Transform.hpp
#   Input.cpp
#   Input.h
# )

target_link_libraries(editor PRIVATE glad glfw glm::glm imgui beamlib)
