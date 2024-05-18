#pragma once

#include <beamlib.h>
#include <string>
#include <vector>
#include <glm/gtc/random.hpp>

const float grid_width = 0.05;
const float grid_vertices[] = {
     grid_width, -0.0f,  grid_width,
    -grid_width, -0.0f, -grid_width,
    -grid_width, -0.0f,  grid_width,
     grid_width, -0.0f,  grid_width,
     grid_width, -0.0f, -grid_width,
    -grid_width, -0.0f, -grid_width,
};

struct Wave {
    float length = 1.0;
    float amplitude = 1.0;
    float speed = 1.0;
    float angle = 0.0;

    glm::vec2 direction() {
        return glm::vec2(glm::cos(angle), glm::sin(angle));
    }

    Wave() {
        length = glm::linearRand(1.0, 10.0);
        amplitude = glm::linearRand(0.1, 0.5);
        speed = glm::linearRand(1.0, 10.0);
        angle = glm::linearRand(0.0, 30.0);
    }
};

class Grid {
    int width;
    int instanceNumber;

    std::vector<Wave> waves;
    int iterations = 24;

public:
    Blib::Transform transform;
    Grid(int width = 300) : width(width), instanceNumber(std::pow(width * 2 + 1, 2)), waves(std::vector<Wave>(10)) {}

    bool mesh = false;

    void render(glm::vec3 lightPos) {
        auto prog = Blib::ResourceManager::GetShader("water");
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

        glBindTexture(GL_TEXTURE_CUBE_MAP, Blib::ResourceManager::GetGLuint("skybox-texture-cubemap"));

        glBindVertexArray(Blib::ResourceManager::GetGLuint("grid-vao"));

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
        Blib::ResourceManager::LoadShader("water", "examples/water/shaders/water.vert.glsl", "examples/water/shaders/water.frag.glsl");

        GLuint& vao = Blib::ResourceManager::GetGLuint("grid-vao");
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint& vbo = Blib::ResourceManager::GetGLuint("grid-vbo");
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
        auto prog = Blib::ResourceManager::GetShader("light");
        prog.Use();
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());
        prog.SetVec3("lightColor", color);

        auto ball = Blib::ResourceManager::GetModel("ball");
        ball.draw(prog);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadShader("light", "examples/water/shaders/default.vert.glsl", "examples/water/shaders/light.frag.glsl");
        Blib::ResourceManager::LoadModel("ball", "models/misc/ball.obj");
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
        Blib::ResourceManager::LoadShader("phong", "examples/toon/shaders/default.vert.glsl", "examples/toon/shaders/phong.frag.glsl");
        Blib::ResourceManager::LoadModel("dragon", "models/dragon/dragon.obj");
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

