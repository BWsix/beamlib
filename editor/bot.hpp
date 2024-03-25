#pragma once

#include <beamlib.h>

#include "model.h"
#include "shaderProgramStore.h"

class BotBodyUpper : public beamlib::Object {
    Model::Model model;

public:
    BotBodyUpper() {
        model.load("editor/trashbot/upper-body.obj");
    }

    void Update() override {}

    void Render() override {
        store::defaultProgram.Use();
        store::defaultProgram.setUniformMat4("view", beamlib::camera.getViewMatrix());
        store::defaultProgram.setUniformMat4("projection", beamlib::camera.getProjectionMatrix());
        model.Draw(store::defaultProgram);
    }
};

class BotBodyUpperInstance : public beamlib::Instance {
public:
    using Instance::Instance;

    void CustomRender() override {
        store::defaultProgram.Use();
        store::defaultProgram.setUniformMat4("model", transform.getModelMatrix());
    }
};

class BotBodyLower : public beamlib::Object {
    Model::Model model;

public:
    BotBodyLower() {
        model.load("editor/trashbot/lower-body.obj");
    }

    void Update() override {}

    void Render() override {
        store::defaultProgram.Use();
        store::defaultProgram.setUniformMat4("view", beamlib::camera.getViewMatrix());
        store::defaultProgram.setUniformMat4("projection", beamlib::camera.getProjectionMatrix());
        model.Draw(store::defaultProgram);
    }
};

class BotBodyLowerInstance : public beamlib::Instance {
public:
    using Instance::Instance;

    void CustomRender() override {
        store::defaultProgram.Use();
        store::defaultProgram.setUniformMat4("model", transform.getModelMatrix());
    }
};

class BotLid : public beamlib::Object {
    Model::Model model;

public:
    BotLid() {
        model.load("editor/trashbot/lid.obj");
    }

    void Update() override {}

    void Render() override {
        store::defaultProgram.Use();
        store::defaultProgram.setUniformMat4("view", beamlib::camera.getViewMatrix());
        store::defaultProgram.setUniformMat4("projection", beamlib::camera.getProjectionMatrix());
        model.Draw(store::defaultProgram);
    }
};

class BotLidInstance : public beamlib::Instance {
public:
    using Instance::Instance;

    void CustomRender() override {
        store::defaultProgram.Use();
        store::defaultProgram.setUniformMat4("model", transform.getModelMatrix());
    }
};

class BotLeg : public beamlib::Object {
    Model::Model model;

public:
    BotLeg() {
        model.load("editor/trashbot/leg.obj");
    }

    void Update() override {}

    void Render() override {
        store::defaultProgram.Use();
        store::defaultProgram.setUniformMat4("view", beamlib::camera.getViewMatrix());
        store::defaultProgram.setUniformMat4("projection", beamlib::camera.getProjectionMatrix());
        model.Draw(store::defaultProgram);
    }
};

class BotLegInstance : public beamlib::Instance {
public:
    using Instance::Instance;

    void CustomRender() override {
        store::defaultProgram.Use();
        store::defaultProgram.setUniformMat4("model", transform.getModelMatrix());
    }
};

class BotArmUpper : public beamlib::Object {
    Model::Model model;

public:
    BotArmUpper() {
        model.load("editor/trashbot/upper-arm.obj");
    }

    void Update() override {}

    void Render() override {
        store::defaultProgram.Use();
        store::defaultProgram.setUniformMat4("view", beamlib::camera.getViewMatrix());
        store::defaultProgram.setUniformMat4("projection", beamlib::camera.getProjectionMatrix());
        model.Draw(store::defaultProgram);
    }
};

class BotArmUpperInstance : public beamlib::Instance {
public:
    using Instance::Instance;

    void CustomRender() override {
        store::defaultProgram.Use();
        store::defaultProgram.setUniformMat4("model", transform.getModelMatrix());
    }
};

class BotArmLower : public beamlib::Object {
    Model::Model model;

public:
    BotArmLower() {
        model.load("editor/trashbot/lower-arm.obj");
    }

    void Update() override {}

    void Render() override {
        store::defaultProgram.Use();
        store::defaultProgram.setUniformMat4("view", beamlib::camera.getViewMatrix());
        store::defaultProgram.setUniformMat4("projection", beamlib::camera.getProjectionMatrix());
        model.Draw(store::defaultProgram);
    }
};

class BotArmLowerInstance : public beamlib::Instance {
public:
    using Instance::Instance;

    void CustomRender() override {
        store::defaultProgram.Use();
        store::defaultProgram.setUniformMat4("model", transform.getModelMatrix());
    }
};

class BotLower : public beamlib::Object {
    Model::Model model;

public:
    BotLower() {
        model.load("editor/trashbot/lower-arm.obj");
    }

    void Update() override {}

    void Render() override {
        store::defaultProgram.Use();
        store::defaultProgram.setUniformMat4("view", beamlib::camera.getViewMatrix());
        store::defaultProgram.setUniformMat4("projection", beamlib::camera.getProjectionMatrix());
        model.Draw(store::defaultProgram);
    }
};

class BotLowerInstance : public beamlib::Instance {
public:
    using Instance::Instance;

    void CustomRender() override {
        store::defaultProgram.Use();
        store::defaultProgram.setUniformMat4("model", transform.getModelMatrix());
    }
};
