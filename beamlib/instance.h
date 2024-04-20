#pragma once

#include "model.h"
#include "shaderProgram.h"
#include "frameData.h"
#include "utils.h"
#include "transform.h"

#include <iostream>

namespace Blib {

class Instance {
protected:
    Model model;
    std::vector<Instance *> children;

public:
    Transform transform;
    std::string name;

    Instance(std::string name) : name(name) {}
    Instance(std::string name, Model model) : model(model), name(name) {}

    void PushChild(Instance *child);

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
    json Interpolator(json& from, json& to, float progress) {
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
        for (auto child : children) child->Update();
    }

    virtual void CustomRender() {}
    virtual void Render(ShaderProgram& shader) {
        CustomRender();
        shader.SetMat4("model", transform.getModelMatrix());
        model.draw(shader);
        for (auto child : children) child->Render(shader);
    }
    virtual void CustomRenderUI() {}
    virtual void RenderUI() {
        if (ImGui::TreeNode(name.c_str())) {
            transform.RenderUI(name);
            CustomRenderUI();
            if (children.size() != 0) ImGui::Separator();
            for (auto child : children) child->RenderUI();
            ImGui::TreePop();
        }
    }
};

extern std::map<std::string, Instance*> instanceStore;
}
