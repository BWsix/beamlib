#pragma once

#include <beamlib.h>
#include <cstdlib>
#include <glad/gl.h>
#include <iostream>

const float quadVertices[] = {
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};

class Screen {
    GLuint vao, vbo, fbo;
    GLuint texture, texture_bright_color, texture_depth, texture_motion;

    unsigned int pingpongFBO[2];
    unsigned int pingpongBuffers[2];

public:
    uint width, height;

    Screen(uint width, uint height): width(width), height(height){}

    bool pixelation = false;
    float pixelSize = 5.0;
    float exposure = 1.0;
    bool motionBlur = false;
    // int blurness = 10;
    void render(Blib::ShaderProgram prog) {
        glDisable(GL_DEPTH_TEST);
        glViewport(0, 0, width, height < 1 ? 1 : height);

        prog.Use();
        // pixelation
        prog.SetBool("pixelation", pixelation);
        prog.SetVec2("screenSize", glm::vec2(width, height));
        prog.SetFloat("pixelSize", pixelSize);

        // motion blur
        // prog.SetInt("blurness", blurness);
        prog.SetBool("motionBlur", motionBlur);
        prog.SetMat4("prevViewProjection", Blib::camera.getPrevViewProjectionMatrix());
        auto mat = Blib::camera.getProjectionMatrix() * Blib::camera.getViewMatrix();
        prog.SetMat4("invViewProjection", glm::inverse(mat));

        // HDR
        prog.SetFloat("exposure", exposure);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        prog.SetInt("screenTex", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_motion);
        prog.SetInt("motion", 1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture_depth);
        prog.SetInt("depth", 2);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffers[0]);
        prog.SetInt("blur", 3);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);
    }


    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void resize(uint width, uint height) {
        this->width = width;
        this->height = height;
        Blib::setupBuffer(texture, width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        Blib::setupBuffer(texture_bright_color, width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT, 1);
        Blib::setupBuffer(texture_motion, width, height, GL_RGB32F, GL_RGB, GL_FLOAT, 2);

        Blib::setupBuffer(pingpongBuffers[0], width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT, 0);
        Blib::setupBuffer(pingpongBuffers[1], width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT, 0);

        glBindTexture(GL_TEXTURE_2D, texture_depth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture_depth, 0);
    }

    void blur(Blib::ShaderProgram prog) {
        bool horizontal = true;
        bool first_iteration = true;
        uint amount = 2 * 10;
        prog.Use();

        for (uint i = 0; i < amount; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]); 
            prog.SetInt("horizontal", horizontal);
            glBindTexture(GL_TEXTURE_2D, first_iteration ? texture_bright_color : pingpongBuffers[!horizontal]); 

            glDisable(GL_DEPTH_TEST);
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glEnable(GL_DEPTH_TEST);

            horizontal = !horizontal;
            if (first_iteration) {
                first_iteration = false;
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void loadResources() {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        size_t stride = sizeof(float) * 4;
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *)(sizeof(float) * 2));

        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenTextures(1, &texture);
        Blib::setupBuffer(texture, width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        glGenTextures(1, &texture_bright_color);
        Blib::setupBuffer(texture_bright_color, width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT, 1);

        glGenTextures(1, &texture_motion);
        Blib::setupBuffer(texture_motion, width, height, GL_RGB32F, GL_RGB, GL_FLOAT, 2);

        glGenTextures(1, &texture_depth);
        glBindTexture(GL_TEXTURE_2D, texture_depth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture_depth, 0);

        GLuint buffers[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        glDrawBuffers(3, buffers);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "ERROR: Framebuffer is not complete\n";
            exit(1);
        }

        glGenTextures(2, pingpongBuffers);
        glGenFramebuffers(2, pingpongFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[0]);
        Blib::setupBuffer(pingpongBuffers[0], width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[1]);
        Blib::setupBuffer(pingpongBuffers[1], width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};
