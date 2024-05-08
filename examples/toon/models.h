#pragma once

#include <beamlib.h>

struct Ball {
    Blib::Transform transform;

    void render(Blib::ShaderProgram& prog) {
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());

        auto ball = Blib::ResourceManager::GetModel("ball");
        ball.draw(prog);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadModel("ball", "models/misc/ball.obj");
    }
};

struct Torus {
    Blib::Transform transform;

    void render(Blib::ShaderProgram& prog) {
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());

        auto torus = Blib::ResourceManager::GetModel("torus");
        torus.draw(prog);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadModel("torus", "models/misc/torus.obj");
    }
};

struct Monkey {
    Blib::Transform transform;

    void render(Blib::ShaderProgram& prog) {
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());

        auto monkey = Blib::ResourceManager::GetModel("monkey");
        monkey.draw(prog);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadModel("monkey", "models/misc/monkey.obj");
    }
};

struct Dragon {
    Blib::Transform transform;

    void render(Blib::ShaderProgram& prog) {
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());

        auto dragon = Blib::ResourceManager::GetModel("dragon");
        dragon.draw(prog);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadModel("dragon", "models/dragon/dragon.obj");
    }
};
