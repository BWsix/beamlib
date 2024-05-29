#pragma once

#include <glad/gl.h>
#include <beamlib.h>

class Depthmap {
    GLuint fbo;

    int width = 7500;
    int height = 7500;

public:
    GLuint texture_depth;

    void loadResources() {
        Blib::ResourceManager::LoadShader("depthmap", "shaders/depthmap.vert.glsl", "shaders/depthmap.frag.glsl");

        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenTextures(1, &texture_depth);
        glBindTexture(GL_TEXTURE_2D, texture_depth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture_depth, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);  
    }

    glm::mat4 getLightProjection() {
        float near = 1;
        float far = 100;
        glm::mat4 lightProjection = glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, near, far);
        return lightProjection;
    }

    glm::mat4 getLightView(glm::vec3 lightPos) {
        glm::mat4 lightView = glm::lookAt(lightPos,
                                          glm::vec3( 0.0f, 0.0f,  0.0f), 
                                          glm::vec3( 0.0f, 1.0f,  0.0f));  
        return lightView;
    }

    Blib::ShaderProgram getProg() {
        return Blib::ResourceManager::GetShader("depthmap");
    }

    void bind() {
        glViewport(0, 0, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }
};
