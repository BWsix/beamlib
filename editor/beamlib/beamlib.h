#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace beamlib {

GLFWwindow *CreateWindow(int width, int height, const char *title);
bool WindowShouldClose(GLFWwindow *window);
void DestroyWindow(GLFWwindow *window);
void BeginUI();
void EndUI();

} // namespace beamlib
