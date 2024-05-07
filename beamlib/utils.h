#pragma once

#include "frameData.h"
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

extern int WIDTH;
extern int HEIGHT;

GLFWwindow *CreateWindow(const char *title, int width, int height);
bool WindowShouldClose(GLFWwindow *window);
void DestroyWindow(GLFWwindow *window);
void BeginUI();
void EndUI();

double getTimeElapsed();
float getFrameRate();
float getDeltaTime();
const char *SlurpFile(const char *path);

void useFrameBuffer();
void useFrameBuffer(const std::string& id);

void dbg(glm::vec3 vec);
void dbg(glm::quat q);
void dbg(glm::mat4 mat);
void dbg(FrameData f);
std::vector<float> toVector(glm::vec3 data);
std::vector<float> toVector(glm::quat data);
std::vector<float> toVector(glm::mat4 data);
glm::vec3 vectorToVec3(std::vector<float> data);
glm::quat vectorToQuat(std::vector<float> data);
glm::mat4 vectorToMat4(std::vector<float> data);

class Object {
public:
    virtual void Update() = 0;
    virtual void Render() = 0;
};

inline const char* label(std::string& s) { return s.c_str(); }
inline const char* label(std::string&& s) { return s.c_str(); }

} // namespace beamlib
