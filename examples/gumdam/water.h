#pragma once

#include <beamlib.h>
#include <string>
#include <vector>
#include <glm/gtc/random.hpp>

const float water_width = 0.05;
const float water_vertices[] = {
     water_width, -0.0f,  water_width,
    -water_width, -0.0f, -water_width,
    -water_width, -0.0f,  water_width,
     water_width, -0.0f,  water_width,
     water_width, -0.0f, -water_width,
    -water_width, -0.0f, -water_width,
};

class Water {
    int width;
    int instanceNumber;

    int iterations = 18;

public:
    Blib::Transform transform;
    Water(int width = 200) : width(width), instanceNumber(std::pow(width * 2 + 1, 2)) {}

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
        prog.SetFloat("delta", 2.0 * water_width);
        prog.SetInt("iterations", iterations);

        glBindTexture(GL_TEXTURE_CUBE_MAP, Blib::ResourceManager::GetGLuint("skybox-texture-cubemap"));

        glBindVertexArray(Blib::ResourceManager::GetGLuint("water-grid-vao"));

        if (mesh) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instanceNumber);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void renderUI() {
        ImGui::SeparatorText("Water");
        ImGui::SliderInt("Iterations", &iterations, 1, 50);
        ImGui::Checkbox("Show Mesh", &mesh);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadShader("water", "shaders/water.vert.glsl", "shaders/water.frag.glsl");

        GLuint& vao = Blib::ResourceManager::GetGLuint("water-grid-vao");
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint& vbo = Blib::ResourceManager::GetGLuint("water-grid-vbo");
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(water_vertices), water_vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

        glBindVertexArray(0);
    }
};
