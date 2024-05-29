#pragma once

#include <beamlib.h>
#include <string>
#include <glm/gtc/random.hpp>

namespace WaterDemo {

const float grid_width = 0.05;
const float grid_vertices[] = {
     grid_width, -0.0f,  grid_width,
    -grid_width, -0.0f, -grid_width,
    -grid_width, -0.0f,  grid_width,
     grid_width, -0.0f,  grid_width,
     grid_width, -0.0f, -grid_width,
    -grid_width, -0.0f, -grid_width,
};


class Grid {
    int width;
    int instanceNumber;

    int iterations = 24;

public:
    Blib::Transform transform;
    Grid(int width = 300) : width(width), instanceNumber(std::pow(width * 2 + 1, 2)) {}

    bool mesh = false;

    void render(glm::vec3 lightPos) {
        auto prog = Blib::ResourceManager::GetShader("waterdemo-water");
        prog.Use();

        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());
        prog.SetInt("halfSideLength", width);

        prog.SetVec3("lightPos", lightPos);
        prog.SetVec3("lightColor", glm::vec3(1, 1, 1));
        prog.SetVec3("viewPos", Blib::camera.getPosition());
        prog.SetVec3("waterColor", glm::vec3(0, 0.3, 0.5));

        prog.SetFloat("time", Blib::getTimeElapsed());
        prog.SetFloat("delta", 2.0 * grid_width);
        prog.SetInt("iterations", iterations);

        glBindTexture(GL_TEXTURE_CUBE_MAP, Blib::ResourceManager::GetGLuint("waterdemo-skybox-texture-cubemap"));

        glBindVertexArray(Blib::ResourceManager::GetGLuint("waterdemo-grid-vao"));

        if (mesh) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instanceNumber);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void renderUI() {
        ImGui::SliderInt("Iterations", &iterations, 1, 50);
        ImGui::Checkbox("Mesh", &mesh);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadShader("waterdemo-water", "examples/water/shaders/water.vert.glsl", "examples/water/shaders/water.frag.glsl");

        GLuint& vao = Blib::ResourceManager::GetGLuint("waterdemo-grid-vao");
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint& vbo = Blib::ResourceManager::GetGLuint("waterdemo-grid-vbo");
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices), grid_vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

        glBindVertexArray(0);
    }
};

class Ball {
    glm::vec3 color;

public:
    Ball() : color(glm::vec3(1.0)) {}

    Blib::Transform transform;

    void render() {
        auto prog = Blib::ResourceManager::GetShader("waterdemo-light");
        prog.Use();
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());
        prog.SetVec3("lightColor", color);

        auto ball = Blib::ResourceManager::GetModel("waterdemo-ball");
        ball.draw(prog);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadShader("waterdemo-light", "examples/water/shaders/default.vert.glsl", "examples/water/shaders/light.frag.glsl");
        Blib::ResourceManager::LoadModel("waterdemo-ball", "models/misc/ball.obj");
    }
};

struct Dragon {
    Blib::Transform transform;

    void render(Blib::ShaderProgram& prog) {
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());

        auto dragon = Blib::ResourceManager::GetModel("waterdemo-dragon");
        dragon.draw(prog);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadShader("waterdemo-phong", "examples/toon/shaders/default.vert.glsl", "examples/toon/shaders/phong.frag.glsl");
        Blib::ResourceManager::LoadModel("waterdemo-dragon", "models/dragon/dragon.obj");
    }
};

struct Torus {
    Blib::Transform transform;

    void render(Blib::ShaderProgram& prog) {
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());

        auto torus = Blib::ResourceManager::GetModel("waterdemo-torus");
        torus.draw(prog);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadModel("waterdemo-torus", "models/misc/torus.obj");
    }
};

}
