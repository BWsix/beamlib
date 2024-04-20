#pragma once

#include <beamlib.h>

const float grid_vertices[] = {
    -0.5f, 0.0f, 0.0,
    0.5f, 0.0f, 0.0,
    0.0f, 0.0f, -0.5,
    0.0f, 0.0f, 0.5,
};

class Grid {
    int width;
    int instanceNumber;
    Blib::Transform transform;

public:
    Grid(int width = 20) : width(width), instanceNumber(std::pow(width * 2 + 1, 2)) {}

    void render() {
        auto prog = Blib::ResourceManager::GetShader("grid");
        prog.Use();
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetInt("width", width);

        glBindVertexArray(Blib::ResourceManager::GetGLuint("grid-vao"));
        glDrawArraysInstanced(GL_LINES, 0, sizeof(grid_vertices), instanceNumber);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadShader("grid", "shaders/grid.vert.glsl", "shaders/grid.frag.glsl");

        GLuint& vao = Blib::ResourceManager::GetGLuint("grid-vao");
        GLuint& vbo = Blib::ResourceManager::GetGLuint("grid-vbo");
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices), grid_vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    }
};
