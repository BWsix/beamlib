#pragma once

#include <beamlib.h>

const float quadVerticesToon[] = {
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};

struct ScreenToon  {
    void render(Blib::ShaderProgram prog) {
        glDisable(GL_DEPTH_TEST);
        prog.Use();
        glBindVertexArray(Blib::ResourceManager::GetGLuint("toon-screen-vao"));
        glBindTexture(GL_TEXTURE_2D, Blib::ResourceManager::GetGLuint("toon-screen-texture"));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);
    }

    void bind() {
        GLuint& fbo = Blib::ResourceManager::GetGLuint("toon-screen-fbo");
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void resize(int width, int height) {
        GLuint& screen_texture = Blib::ResourceManager::GetGLuint("toon-screen-texture");
        glBindTexture(GL_TEXTURE_2D, screen_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Blib::WIDTH, Blib::HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, screen_texture, 0);

        GLuint& screen_texture_depth = Blib::ResourceManager::GetGLuint("toon-screen-texture-depth");
        glBindTexture(GL_TEXTURE_2D, screen_texture_depth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Blib::WIDTH, Blib::HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, screen_texture_depth, 0);
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadShader("toon-screen", "examples/toon/shaders/screen.vert.glsl", "examples/toon/shaders/screen.frag.glsl");

        GLuint& vao = Blib::ResourceManager::GetGLuint("toon-screen-vao");
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint& vbo = Blib::ResourceManager::GetGLuint("toon-screen-vbo");
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerticesToon), &quadVerticesToon, GL_STATIC_DRAW);
        size_t stride = sizeof(float) * 4;
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *)(sizeof(float) * 2));

        GLuint& fbo = Blib::ResourceManager::GetGLuint("toon-screen-fbo");
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        GLuint& screen_texture = Blib::ResourceManager::GetGLuint("toon-screen-texture");
        glGenTextures(1, &screen_texture);
        glBindTexture(GL_TEXTURE_2D, screen_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Blib::WIDTH, Blib::HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, screen_texture, 0);

        GLuint& screen_texture_depth = Blib::ResourceManager::GetGLuint("toon-screen-texture-depth");
        glGenTextures(1, &screen_texture_depth);
        glBindTexture(GL_TEXTURE_2D, screen_texture_depth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Blib::WIDTH, Blib::HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, screen_texture_depth, 0);

        GLuint buffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, buffers);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "ERROR: Framebuffer is not complete\n";
            exit(1);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};
