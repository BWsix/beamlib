#pragma once

#include "utils.h"

namespace Blib {

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
        * glm::translate(glm::mat4(1), position)
        * glm::toMat4(localRotationQuat);
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

}
