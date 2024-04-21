#pragma once

#include "model.h"
#include "shaderProgram.h"
#include "frameData.h"
#include "utils.h"
#include "transform.h"

#include <iostream>

namespace Blib {

class Instance {
public:
    Model model;
    std::vector<Instance *> children;
    std::map<std::string, Instance *> children_mapper;

    Transform transform;
    std::string name;

    Instance(std::string name) : name(name) {}
    Instance(std::string name, Model model) : model(model), name(name) {}

    void PushChild(Instance *child);

    virtual json CustomSerialize() { return {}; };
    json Serialize() {
        json j;
        j["transform"] = transform.SerializeModel();
        j["data"] = CustomSerialize();
        for (auto child : children) j["children"][child->name] = child->Serialize();
        return j;
    }

    virtual void CustomLoad(json j) { (void)j; }
    void Load(json j) {
        transform.LoadModel(j["transform"]);
        if (!j["data"].is_null()) {
            CustomLoad(j["data"]);
        }
        for (size_t i = 0; i < children.size(); i++) {
            children[i]->Load(j["children"][children[i]->name]);
        }
    }

    virtual json CustomInterpolator(json from, json to, float progress) {
        (void)from; (void)to; (void)progress;
        return {};
    }
    json Interpolator(json& from, json& to, float progress) {
        json j;
        j["transform"] = transform.Interpolator(from["transform"], to["transform"], progress);
        j["data"] = CustomInterpolator(from["data"], to["data"], progress);
        for (auto& elem : from["children"].items()) {
            j["children"][elem.key()] = children_mapper[elem.key()]->Interpolator(from["children"][elem.key()], to["children"][elem.key()], progress);
        }
        return j;
    }

    virtual void CustomUpdate() {}
    virtual void Update() {
        CustomUpdate();
        for (auto child : children) child->Update();
    }

    virtual void CustomRender(ShaderProgram prog) {
        prog.Use();
        prog.SetMat4("model", transform.getModelMatrix());
        model.draw(prog);
    }
    virtual void Render(ShaderProgram prog) {
        CustomRender(prog);
        for (auto child : children) child->Render(prog);
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
