#pragma once

#include <beamlib.h>

const float quadVertices[] = {
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};

class Screen  {
public:
    void render(Blib::ShaderProgram prog) {
        glDisable(GL_DEPTH_TEST);
        prog.Use();
        glBindVertexArray(Blib::ResourceManager::GetGLuint("screen-vao"));
        glBindTexture(GL_TEXTURE_2D, Blib::ResourceManager::GetGLuint("custom-screen-texture"));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);
    }

    static void LoadOtherPostProcessingResources() {
        Blib::ResourceManager::LoadShader("pixelation", "shaders/screen.vert.glsl", "shaders/pixelation.frag.glsl");
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadShader("screen", "shaders/screen.vert.glsl", "shaders/screen.frag.glsl");

        GLuint& vao = Blib::ResourceManager::GetGLuint("screen-vao");
        GLuint& vbo = Blib::ResourceManager::GetGLuint("screen-vbo");
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        size_t stride = sizeof(float) * 4;
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *)(sizeof(float) * 2));
    }
};
