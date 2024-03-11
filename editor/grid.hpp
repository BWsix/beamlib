#pragma once

#include <beamlib.h>

#include "shaderProgramStore.h"

const float grid_vertices[] = {
    -0.5f, 0.0f, 0.0,
    0.5f, 0.0f, 0.0,
    0.0f, 0.0f, -0.5,
    0.0f, 0.0f, 0.5,
};

class Grid : public beamlib::Object {
    GLuint vbo_vertices;
    GLuint vao_vertices;

    int width;
    int instanceNumber;

public:
    Grid(int width = 20) : width(width), instanceNumber(std::pow(width * 2 + 1, 2)) {
        glGenBuffers(1, &vbo_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices), grid_vertices, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vao_vertices);
        glBindVertexArray(vao_vertices);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glEnableVertexAttribArray(0);
    }

    void Update() override {}

    void Render() override {
        store::gridProgram.Use();
        store::gridProgram.setUniformMat4("view", beamlib::camera.getViewMatrix());
        store::gridProgram.setUniformMat4("projection", beamlib::camera.getProjectionMatrix());
        store::gridProgram.setUniformInt("width", width);
        glBindVertexArray(vao_vertices);
        glDrawArraysInstanced(GL_LINES, 0, sizeof(grid_vertices), instanceNumber);
    }
};

class GridInstance : public beamlib::Instance {
public:
    GridInstance(beamlib::Object *object, std::string name) : Instance(object, name) {}

    void CustomRender() override {
        store::gridProgram.Use();
        store::gridProgram.setUniformMat4("model", transform.getModelMatrix());
    }
};
