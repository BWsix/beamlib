#pragma once

#include <beamlib.h>

const float grid_vertices[] = {
    -0.5f, 0.0f, 0.0,
    0.5f, 0.0f, 0.0,
    0.0f, 0.0f, -0.5,
    0.0f, 0.0f, 0.5,
};

const float planeVertices[] = {
    // positions            // normals         // texcoords
     50.0f, -0.0f,  50.0f,  0.0f, 1.0f, 0.0f,  50.0f,  0.0f,
    -50.0f, -0.0f, -50.0f,  0.0f, 1.0f, 0.0f,   0.0f, 50.0f,
    -50.0f, -0.0f,  50.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,

     50.0f, -0.0f,  50.0f,  0.0f, 1.0f, 0.0f,  50.0f,  0.0f,
     50.0f, -0.0f, -50.0f,  0.0f, 1.0f, 0.0f,  50.0f, 50.0f,
    -50.0f, -0.0f, -50.0f,  0.0f, 1.0f, 0.0f,   0.0f, 50.0f,
};

class Grid {
    int width;
    int instanceNumber;

public:
    Blib::Transform transform;
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

    void renderFloor(Blib::ShaderProgram prog) {
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());
        
        GLuint planeVAO = Blib::ResourceManager::GetGLuint("floor-vao");
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadShader("grid", "shaders/grid.vert.glsl", "shaders/grid.frag.glsl");
        Blib::ResourceManager::LoadShader("floor", "shaders/floor.vert.glsl", "shaders/floor.frag.glsl");

        GLuint& vao = Blib::ResourceManager::GetGLuint("grid-vao");
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint& vbo = Blib::ResourceManager::GetGLuint("grid-vbo");
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices), grid_vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);


        // floor
        GLuint& planeVAO = Blib::ResourceManager::GetGLuint("floor-vao");
        glGenVertexArrays(1, &planeVAO);
        glBindVertexArray(planeVAO);

        GLuint& planeVBO = Blib::ResourceManager::GetGLuint("floor-vbo");;
        glGenBuffers(1, &planeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

        glBindVertexArray(0);
    }
};
