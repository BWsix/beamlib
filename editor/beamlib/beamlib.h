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
const char *SlurpFile(const char *path);

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
    void RotateLocal(glm::quat quat) { localRotationQuat *= quat; }
    void RotateLocal(glm::vec3 euler) { Rotate(glm::quat(euler)); }
    void Scale(glm::vec3 scale) { scale *= scale; }

    glm::vec3 getPosition() const { return rotationQuat * (parent ? parent->getPosition() + position : position); }
    glm::vec3 getLocalPosition() const { return position; }
    glm::vec3 getRotationEuler() const { return glm::eulerAngles(rotationQuat); }
    glm::vec3 getLocalRotationEuler() const { return glm::eulerAngles(localRotationQuat); }
    glm::quat getLocalRotationQuat() const { return localRotationQuat; }
    glm::mat4 getModelMatrix() const {
        glm::mat4 model(1);
        glm::mat4 parentModel = parent ? parent->getModelMatrixForChildren() : glm::mat4(1);
        return parentModel
        * glm::toMat4(rotationQuat) 
        * glm::translate(model, position)
        * glm::toMat4(localRotationQuat)
        * glm::scale(model, scale);
    }
    glm::mat4 getModelMatrixForChildren() const {
        glm::mat4 parentModel = parent ? parent->getModelMatrixForChildren() : glm::mat4(1);
        return parentModel
        * glm::toMat4(rotationQuat) 
        * glm::translate(glm::mat4(1), position);
    }

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
    json Interpolator(json from, json to, float progress) {
        return {
            {"position", toVector(lerp(vectorToVec3(from["position"]), vectorToVec3(to["position"]), progress))},
            {"rotationQuat", toVector(glm::slerp(vectorToQuat(from["rotationQuat"]), vectorToQuat(to["rotationQuat"]), progress))},
            {"localRotationQuat", toVector(glm::slerp(vectorToQuat(from["localRotationQuat"]), vectorToQuat(to["localRotationQuat"]), progress))},
            {"scale", toVector(lerp(vectorToVec3(from["scale"]), vectorToVec3(to["scale"]), progress))},
        };
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

class ShaderProgram {
    GLuint id;

    void CheckShaderCompileStatus(GLuint shader);
    void CheckProgramLinkStatus(GLuint program);

public:
    GLuint getId() const { return id; }

    void Load(const char* vertexPath, const char* fragmentPath);
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
    virtual void Update() = 0;
    virtual void Render() = 0;
};

class Instance;
extern std::map<std::string, Instance*> instanceStore;

class Instance {
protected:
    Object *object;
    std::vector<Instance *> children;

public:
    Transform transform;
    std::string name;

    Instance(Object *object, std::string name) : object(object), name(name) {}

    void PushChild(Instance *child) {
        child->transform.setParent(&transform);
        instanceStore.insert({child->name, child});
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

    virtual void CustomLoad(json j) { (void)j; }
    void Load(json j) {
        transform.LoadModel(j["transform"]);
        if (!j["data"].is_null()) {
            CustomLoad(j["data"]);
        }
        for (size_t i = 0; i < children.size(); i++) {
            children[i]->Load(j["children"][i]);
        }
    }

    virtual json CustomInterpolator(json from, json to, float progress) {
        (void)from; (void)to; (void)progress;
        return {};
    }
    json Interpolator(json from, json to, float progress) {
        json j;
        j["name"] = from["name"];
        j["transform"] = transform.Interpolator(from["transform"], to["transform"], progress);
        j["children"] = json::array();
        j["data"] = CustomInterpolator(from["data"], to["data"], progress);
        for (size_t i = 0; i < children.size(); i++) {
            j["children"].emplace_back(children[i]->Interpolator(from["children"][i], to["children"][i], progress));
        }
        return j;
    }

    virtual void CustomUpdate() {}
    virtual void Update() {
        CustomUpdate();
        if (object != NULL)  {
            object->Update();
        }
        for (auto child : children) child->Update();
    }
    virtual void CustomRender() {}
    virtual void Render() {
        CustomRender();
        if (object != NULL) {
            object->Render();
        }
        for (auto child : children) child->Render();
    }
    virtual void CustomRenderUI() {}
    virtual void RenderUI() {
        if (ImGui::TreeNode(name.c_str())) {
            if (ImGui::Button(("export##" + name).c_str())) {
                // TODO: save this to a file, or work with animation editor.
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

class DummyInstance : public Instance {
public:
    using Instance::Instance;
};
class Camera : public Instance {
    float yaw = -90.0f;
    float pitch = 0.0f;
    float yaw_prev = -90.0f;
    float pitch_prev = 0.0f;

    Instance *targetInstance;
    float radius = 5;

    float fovy = 45;
    float aspect = 800.0 / 600;
    float near = 0.1;
    float far = 100;

    float sensitivity = 0.05;
    float movementSpeed = 5;

public:
    using Instance::Instance;

    void setAspect(float aspect) { this->aspect = aspect; }
    bool isFirstPersonMode() const {
        return targetInstance == NULL;
    }
    void setTargetInstance(Instance *target) {
        if (!targetInstance) {
            yaw_prev = yaw;
            pitch_prev = pitch;
        }
        targetInstance = target;
    }
    void removeTargetInstance() {
        yaw = yaw_prev;
        pitch = pitch_prev;
        setTargetInstance(NULL);
    }
    Transform *getTargetTranform() const { return &targetInstance->transform; }

    glm::vec3 getFront() const { return glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch))); }
    glm::mat4 getViewMatrix() const {
        if (isFirstPersonMode()) {
            return glm::lookAt(transform.getLocalPosition(), transform.getLocalPosition() + getFront(), {0, 1, 0});
        } else {
            return glm::lookAt(getTargetTranform()->getPosition() - radius * getFront(), getTargetTranform()->getPosition(), {0, 1, 0});
        }
    }
    glm::mat4 getProjectionMatrix() const { return glm::perspective(glm::radians(fovy), aspect, near, far); }

    void Update() override;

    void CustomRenderUI() override {
        ImGui::DragFloat(("pitch##" + name).c_str(), &pitch, 0.1);
        ImGui::DragFloat(("yaw##" + name).c_str(), &yaw, 0.1);
        if (!isFirstPersonMode()) {
            ImGui::DragFloat(("radius##" + name).c_str(), &radius, 0.2);
            if (ImGui::Button(("Go Back To First Person Mode##" + name).c_str())) {
                removeTargetInstance();
            }
        }
    }

    json CustomSerialize() override {
        return {
            {"radius", radius},
            {"pitch", pitch},
            {"yaw", yaw},
            {"pitch_prev", pitch_prev},
            {"yaw_prev", yaw_prev},
            {"target", targetInstance ? targetInstance->name : ""},
        };
    }

    void CustomLoad(json j) override {
        radius = j["radius"];
        pitch = j["pitch"];
        yaw = j["yaw"];
        pitch_prev = j["pitch_prev"];
        yaw_prev = j["yaw_prev"];
        targetInstance = j["target"] != "" ? instanceStore.at(j["target"]) : NULL;
    }

    json CustomInterpolator(json from, json to, float progress) override {
        return {
            {"radius", beamlib::lerp(float(from["radius"]), float(to["radius"]), progress)},
            {"pitch", beamlib::lerp(float(from["pitch"]), float(to["pitch"]), progress)},
            {"yaw", beamlib::lerp(float(from["yaw"]), float(to["yaw"]), progress)},
            {"pitch_prev", beamlib::lerp(float(from["pitch_prev"]), float(to["pitch_prev"]), progress)},
            {"yaw_prev", beamlib::lerp(float(from["yaw_prev"]), float(to["yaw_prev"]), progress)},
            {"target", from["target"]},
        };
    }
};
extern Camera camera;

} // namespace beamlib
