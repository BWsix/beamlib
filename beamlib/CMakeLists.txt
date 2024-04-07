add_library(beamlib
  animator.cpp
  animator.h
  camera.cpp
  camera.h
  instance.cpp
  instance.h
  mesh.cpp
  mesh.h
  model.cpp
  model.h
  mouse.cpp
  mouse.h
  resourceManager.cpp
  resourceManager.h
  shaderProgram.cpp
  shaderProgram.h
  texture.cpp
  texture.h
  transform.cpp
  transform.h
  utils.cpp
  utils.h
)
target_link_libraries(beamlib PRIVATE glad glfw glm imgui nlohmann_json assimp stb)
target_include_directories(beamlib PUBLIC "include/")

if(UNIX)
target_compile_options(beamlib PRIVATE "-Wall" "-Wextra" "-ggdb")
endif()
