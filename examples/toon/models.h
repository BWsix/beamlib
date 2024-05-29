#pragma once

#include <beamlib.h>

struct ToonBall {
    Blib::Transform transform;

    void render(Blib::ShaderProgram& prog) {
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());

        auto ball = Blib::ResourceManager::GetModel("toon-ball");
        ball.draw(prog);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadModel("toon-ball", "models/misc/ball.obj");
    }
};

struct Torus {
    Blib::Transform transform;

    void render(Blib::ShaderProgram& prog) {
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());

        auto torus = Blib::ResourceManager::GetModel("toon-torus");
        torus.draw(prog);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadModel("toon-torus", "models/misc/torus.obj");
    }
};

struct Monkey {
    Blib::Transform transform;

    void render(Blib::ShaderProgram& prog) {
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());

        auto monkey = Blib::ResourceManager::GetModel("toon-monkey");
        monkey.draw(prog);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadModel("toon-monkey", "models/misc/monkey.obj");
    }
};

struct Dragon {
    Blib::Transform transform;

    void render(Blib::ShaderProgram& prog) {
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());

        auto dragon = Blib::ResourceManager::GetModel("toon-dragon");
        dragon.draw(prog);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadModel("toon-dragon", "models/dragon/dragon.obj");
    }
};
