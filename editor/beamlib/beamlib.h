#pragma once

#include <iostream>
#include <string>
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

namespace beamlib {

GLFWwindow *CreateWindow(const char *title, int width = 800, int height = 600);
bool WindowShouldClose(GLFWwindow *window);
void DestroyWindow(GLFWwindow *window);
void BeginUI();
void EndUI();

double getTimeElapsed();
float getFrameRate();
float getDeltaTime();

inline void dbg(glm::vec3 vec) { std::cout << vec[0] << " " << vec[1] << " " << vec[2] << "\n"; }
inline void dbg(glm::mat4 mat) { for (size_t i = 0; i < 4; i++) std::cout << mat[i][0] << " " << mat[i][1] << " " << mat[i][2] << " " << mat[i][3] << "\n"; }

class Transform {
    Transform *parent = NULL;
    glm::vec3 position = {0, 0, 0};
    glm::quat rotationQuat = glm::angleAxis(0.0f, glm::vec3({0, 1, 0}));
    glm::vec3 rotationEuler = glm::eulerAngles(rotationQuat);
    glm::quat localRotationQuat = glm::angleAxis(0.0f, glm::vec3({0, 1, 0}));
    glm::vec3 localRotationEuler = glm::eulerAngles(localRotationQuat);
    glm::vec3 scale = {1, 1, 1};

public:
    void setParent(Transform *transform) { parent = transform; }

    void Translate(glm::vec3 offset) { position += offset; }
    void Rotate(glm::quat quat) { rotationEuler = glm::eulerAngles(rotationQuat *= quat); }
    void Rotate(glm::vec3 euler) { Rotate(glm::quat(euler)); }
    void RotateLocal(glm::quat quat) { localRotationEuler = glm::eulerAngles(localRotationQuat *= quat); }
    void RotateLocal(glm::vec3 euler) { Rotate(glm::quat(euler)); }
    void Scale(glm::vec3 scale) { scale *= scale; }

    glm::vec3 getPosition() const { return position; }
    glm::vec3 getRotationEuler() const { return rotationEuler; }
    glm::vec3 getLocalRotationEuler() const { return localRotationEuler; }
    glm::mat4 getModelMatrix() const;
    glm::mat4 getModelMatrixForChildren() const;

    void setRotationEuler(glm::vec3 euler) { rotationQuat = glm::quat(rotationEuler = euler); }
    void setLocalRotationEuler(glm::vec3 euler) { localRotationQuat = glm::quat(localRotationEuler = euler); }

    void RenderUI(std::string name);
};

class Mouse {
    double mouseDownX, mouseDownY;
    bool rightButtonPrssed = false;
    glm::vec2 deltaRight = {0, 0};

public:
    bool getRightButtonPressed() const { return rightButtonPrssed; }
    glm::vec2 getDeltaRight() const { return deltaRight; }

    void Update(GLFWwindow *window);
};
extern Mouse mouse;

class Camera {
    float yaw = -90.0f;
    float pitch = 0.0f;

    float fovy = 45;
    float aspect = 800.0 / 600;
    float near = 0.1;
    float far = 100;

    float sensitivity = 0.2;
    float movementSpeed = 5;

public:
    beamlib::Transform transform;

    void setAspect(float aspect) { this->aspect = aspect; }

    glm::vec3 getFront() const { return glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch))); }
    glm::mat4 getViewMatrix() const { return glm::lookAt(transform.getPosition(), transform.getPosition() + getFront(), {0, 1, 0}); }
    glm::mat4 getProjectionMatrix() const { return glm::perspective(glm::radians(fovy), aspect, near, far); }

    void Update(GLFWwindow* window);
};
extern Camera camera;

class ShaderProgram {
    GLuint id;

    void CheckShaderCompileStatus(GLuint shader);
    void CheckProgramLinkStatus(GLuint program);
    const char *SlurpFile(const char *path);

public:
    GLuint getId() const { return id; }

    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    void Use() const { glUseProgram(id); }

    void setUniformBool(const std::string &name, bool value) const { glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); }
    void setUniformInt(const std::string &name, int value) const { glUniform1i(glGetUniformLocation(id, name.c_str()), value); }
    void setUniformFloat(const std::string &name, float value) const { glUniform1f(glGetUniformLocation(id, name.c_str()), value); }
    void setUniformVec2(const std::string &name, const glm::vec2 &value) const { glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value)); }
    void setUniformVec3(const std::string &name, const glm::vec3 &value) const { glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value)); }
    void setUniformVec4(const std::string &name, const glm::vec4 &value) const { glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value)); }
    void setUniformMat4(const std::string &name, const glm::mat4 &mat) const { glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat)); }
};

template <typename T> class Instance {
protected:
    T *object;
    std::vector<Instance *> children;
    std::string name;

public:
    Transform transform;

    Instance(T *object, std::string name) : object(object), name(name) {}
    void PushChild(Instance<T> *child) {
        child->transform.setParent(&transform);
        children.push_back(child);
    }

    virtual void CustomUpdate() {}
    virtual void Update() {
        CustomUpdate();
        object->Update();
    }
    virtual void CustomRender() {}
    virtual void Render() {
        object->shaderProgram.Use();
        object->shaderProgram.setUniformMat4("model", transform.getModelMatrix());
        CustomRender();
        object->Render();
        for (auto child : children) child->Render();
    }
    virtual void RenderUI() {
        if (ImGui::TreeNode(name.c_str())) {
            transform.RenderUI(name);
            if (children.size() != 0) ImGui::Separator();
            for (auto child : children) child->RenderUI();
            ImGui::TreePop();
        }
    }
};

} // namespace beamlib
