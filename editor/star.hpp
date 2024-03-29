#pragma once

#include <beamlib.h>

#include "shaderProgramStore.h"

const float star_vertices_front[] = {
    // position      // brightness
    0.0,  0.0, 0.3, 1.0, // front
    0.0,  0.7, 0.0, 0.4, // top
    -0.2,  0.4, 0.0, 0.4, // top > left
    -0.6,  0.4, 0.0, 0.4, // left
    -0.3,  0.0, 0.0, 0.4, // left > bottom-left
    -0.5, -0.4, 0.0, 0.4, // bottom-left
    0.0, -0.2, 0.0, 0.4, // bottom-left > bottom-right
    0.5, -0.4, 0.0, 0.4, // bottom-right
    0.3,  0.0, 0.0, 0.4, // bottom-right > right
    0.6,  0.4, 0.0, 0.4, // right
    0.2,  0.4, 0.0, 0.4, // right > top
    0.0,  0.7, 0.0, 0.4, // top
};

const float star_vertices_back[] = {
    // position      // brightness
    0.0,  0.0, -0.3, 1.0, // back
    0.0,  0.7,  0.0, 0.4, // top
    -0.2,  0.4,  0.0, 0.4, // top > left
    -0.6,  0.4,  0.0, 0.4, // left
    -0.3,  0.0,  0.0, 0.4, // left > bottom-left
    -0.5, -0.4,  0.0, 0.4, // bottom-left
    0.0, -0.2,  0.0, 0.4, // bottom-left > bottom-right
    0.5, -0.4,  0.0, 0.4, // bottom-right
    0.3,  0.0,  0.0, 0.4, // bottom-right > right
    0.6,  0.4,  0.0, 0.4, // right
    0.2,  0.4,  0.0, 0.4, // right > top
    0.0,  0.7,  0.0, 0.4, // top
};

class Star : public beamlib::Object {
    GLuint vbo_star_front, vbo_star_back;
    GLuint vao_star_front, vao_star_back;

public:
    Star() {
        float stride = 4 * sizeof(float);

        glGenBuffers(1, &vbo_star_front);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_star_front);
        glBufferData(GL_ARRAY_BUFFER, sizeof(star_vertices_front), star_vertices_front, GL_STATIC_DRAW);
        glGenVertexArrays(1, &vao_star_front);
        glBindVertexArray(vao_star_front);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &vbo_star_back);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_star_back);
        glBufferData(GL_ARRAY_BUFFER, sizeof(star_vertices_back), star_vertices_back, GL_STATIC_DRAW);
        glGenVertexArrays(1, &vao_star_back);
        glBindVertexArray(vao_star_back);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void Update() override {}

    void Render() override {
        store::starProgram.Use();
        store::starProgram.setUniformMat4("view", beamlib::camera.getViewMatrix());
        store::starProgram.setUniformMat4("projection", beamlib::camera.getProjectionMatrix());

        glBindVertexArray(vao_star_front);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(star_vertices_front) / 4);
        glBindVertexArray(vao_star_back);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(star_vertices_back) / 4);
    }
};

class StarInstance : public beamlib::Instance {
public:
    using Instance::Instance;

    glm::vec3 color = {1, 1, 1};

    json CustomSerialize() override {
        return {
            {"color", beamlib::toVector(color)},
        };
    }

    void CustomLoad(json j) override {
        color = beamlib::vectorToVec3(j["color"]);
    }

    json CustomInterpolator(json from, json to, float progress) override {
        return {
            {"color", beamlib::toVector(beamlib::lerp(beamlib::vectorToVec3(from["color"]), beamlib::vectorToVec3(to["color"]), progress))},
        };
    }

    void CustomRender() override {
        store::starProgram.Use();
        store::starProgram.setUniformMat4("model", transform.getModelMatrix());
        store::starProgram.setUniformVec3("lightColor", color);
    }

    void CustomRenderUI() override {
        ImGui::ColorEdit3(("color##" + name).c_str(), glm::value_ptr(color), ImGuiColorEditFlags_Float);
        if (ImGui::Button(("focus##" + name).c_str())) {
            beamlib::camera.setTargetInstance(this);
        }
    }
};
