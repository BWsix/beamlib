#pragma once

#include <beamlib.h>
#include <cstdlib>
#include <glad/gl.h>
#include <iostream>

const float waterQuadVertices[] = {
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};


class Screen {
    GLuint vao, vbo;

    GLuint fbo_reflection;
    GLuint texture_reflection;
    GLuint fbo_refraction;
    GLuint texture_refraction;

    GLuint texture_depth;
public:
    GLuint width, height;

    Screen(GLuint width, GLuint height): width(width), height(height){}

    void render(Blib::ShaderProgram prog) {
        glDisable(GL_DEPTH_TEST);

        glActiveTexture(GL_TEXTURE10);
        glBindTexture(GL_TEXTURE_2D, texture_reflection);
        prog.SetInt("screenTex", 10);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);
    }

    void bindReflection() {
        glViewport(0, 0, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_reflection);
    }

    void bindRefraction() {
        glViewport(0, 0, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_refraction);
    }

    void bindTextureForProgram(Blib::ShaderProgram prog) {
        glActiveTexture(GL_TEXTURE10);
        glBindTexture(GL_TEXTURE_2D, texture_reflection);
        prog.SetInt("reflectionTex", 10);
        glActiveTexture(GL_TEXTURE11);
        glBindTexture(GL_TEXTURE_2D, texture_refraction);
        prog.SetInt("refractionTex", 11);
    }

    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void resize(GLuint width, GLuint height) {
        this->width = width;
        this->height = height;
        Blib::setupBuffer(texture_refraction, width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, 0);
        Blib::setupBuffer(texture_reflection, width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, 1);

        glBindTexture(GL_TEXTURE_2D, texture_depth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture_depth, 0);
    }

    void loadResources() {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(waterQuadVertices), &waterQuadVertices, GL_STATIC_DRAW);
        size_t stride = sizeof(float) * 4;
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *)(sizeof(float) * 2));

        glGenTextures(1, &texture_depth);
        glBindTexture(GL_TEXTURE_2D, texture_depth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glGenFramebuffers(1, &fbo_reflection);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_reflection);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture_depth, 0);

        glGenTextures(1, &texture_reflection);
        Blib::setupBuffer(texture_reflection, width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "ERROR: Framebuffer is not complete\n";
            exit(1);
        }

        glGenFramebuffers(1, &fbo_refraction);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_refraction);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture_depth, 0);

        glGenTextures(1, &texture_refraction);
        Blib::setupBuffer(texture_refraction, width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "ERROR: Framebuffer is not complete\n";
            exit(1);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};
