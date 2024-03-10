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
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace beamlib {

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

class Transform {
    Transform *parent = NULL;
    glm::vec3 position = {0, 0, 0};
    glm::quat rotationQuat = glm::angleAxis(0.0f, glm::vec3({0, 1, 0}));
    glm::quat localRotationQuat = glm::angleAxis(0.0f, glm::vec3({0, 1, 0}));
    glm::vec3 scale = {1, 1, 1};

public:
    void setParent(Transform *transform) { parent = transform; }

    void Translate(glm::vec3 offset) { position += offset; }
    void Rotate(glm::quat quat) { rotationQuat *= quat; }
    void Rotate(glm::vec3 euler) { Rotate(glm::quat(euler)); }
    void RotateLocal(glm::quat quat) { localRotationQuat *= quat; }
    void RotateLocal(glm::vec3 euler) { Rotate(glm::quat(euler)); }
    void Scale(glm::vec3 scale) { scale *= scale; }

    glm::vec3 getPosition() const { return position; }
    glm::vec3 getRotationEuler() const { return glm::eulerAngles(rotationQuat); }
    glm::vec3 getLocalRotationEuler() const { return glm::eulerAngles(localRotationQuat); }
    glm::mat4 getModelMatrix() const;
    glm::mat4 getModelMatrixForChildren() const;

    void RenderUI(std::string name);
    json SerializeModel() {
        return {
            {"position", toVector(position)},
            {"rotationQuat", toVector(rotationQuat)},
            {"localRotationQuat", toVector(localRotationQuat)},
            {"scale", toVector(scale)},
        };
    }
    void LoadModel(json j) {
        position = vectorToVec3(j["position"]);
        rotationQuat = vectorToQuat(j["rotationQuat"]);
        localRotationQuat = vectorToQuat(j["localRotationQuat"]);
        scale = vectorToVec3(j["scale"]);
    }
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

class Object {
public:
    beamlib::ShaderProgram shaderProgram;
    Object(ShaderProgram program) : shaderProgram(program) {}
    virtual void Update() = 0;
    virtual void Render() = 0;
};

class Instance {
protected:
    Object *object;
    std::vector<Instance *> children;
    std::string name;

public:
    Transform transform;

    Instance(Object *object, std::string name) : object(object), name(name) {}

    void PushChild(Instance *child) {
        child->transform.setParent(&transform);
        children.push_back(child);
    }

    virtual json CustomSerialize() { return {}; };
    json Serialize() {
        json j;
        j["name"] = name;
        j["transform"] = transform.SerializeModel();
        j["children"] = json::array();
        j["data"] = CustomSerialize();
        for (auto child : children) j["children"].emplace_back(child->Serialize());
        return j;
    }

    virtual void CustomLoad(json j) {}
    void Load(json j) {
        transform.LoadModel(j["transform"]);
        if (!j["data"].is_null()) {
            CustomLoad(j["data"]);
        }
        for (size_t i = 0; i < children.size(); i++) children[i]->Load(j["children"][i]);
    }

    virtual void CustomUpdate() {}
    virtual void Update() {
        CustomUpdate();
        object->Update();
        for (auto child : children) child->Update();
    }
    virtual void CustomRender() {}
    virtual void Render() {
        object->shaderProgram.Use();
        object->shaderProgram.setUniformMat4("model", transform.getModelMatrix());
        CustomRender();
        object->Render();
        for (auto child : children) child->Render();
    }
    virtual void CustomRenderUI() {}
    virtual void RenderUI() {
        if (ImGui::TreeNode(name.c_str())) {
            if (ImGui::Button(("export##" + name).c_str())) {
                std::cout << Serialize() << "\n";
            }
            transform.RenderUI(name);
            CustomRenderUI();
            if (children.size() != 0) ImGui::Separator();
            for (auto child : children) child->RenderUI();
            ImGui::TreePop();
        }
    }
};

} // namespace beamlib
