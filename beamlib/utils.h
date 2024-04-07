#pragma once

#include <vector>

#include <imgui.h>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Blib {

GLFWwindow *CreateWindow(const char *title, int width = 800, int height = 600);
bool WindowShouldClose(GLFWwindow *window);
void DestroyWindow(GLFWwindow *window);
void BeginUI();
void EndUI();

double getTimeElapsed();
float getFrameRate();
float getDeltaTime();

void dbg(glm::vec3 vec);
void dbg(glm::quat q);
void dbg(glm::mat4 mat);
std::vector<float> toVector(glm::vec3 data);
std::vector<float> toVector(glm::quat data);
std::vector<float> toVector(glm::mat4 data);
glm::vec3 vectorToVec3(std::vector<float> data);
glm::quat vectorToQuat(std::vector<float> data);
glm::mat4 vectorToMat4(std::vector<float> data);

template<typename T>
T lerp(T from, T to, float progress) {
    return (1.0f - progress) * from + progress * to;
}


class Object {
public:
    virtual void Update() = 0;
    virtual void Render() = 0;
};

} // namespace beamlib
