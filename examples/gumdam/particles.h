#pragma once

#include <beamlib.h>
#include <glad/gl.h>

#include <glm/gtc/random.hpp>
#include <vector>

struct Point {
    const float radius = 7.0;
    const int max_size = 10;
    const float duration = 1.0;

    glm::vec3 initialPos;
    float progress = -420;

    Point() {
        initialPos = glm::sphericalRand(radius);
    }

    void init(){
        progress = 0.0;
    }

    void update() {
        progress += Blib::getDeltaTime() * (1 / duration);
    }

    glm::vec3 getWorldPos(glm::mat4 model) {
        return glm::vec3(model * glm::vec4(initialPos * r(), 1.0));
    }

    float getProgress() {
        return progress;
    }

    bool dead() {
        return progress < 0 || progress > 1;
    }

    float r() {
        return 1.0 - Blib::easefunc_easeInCubic(getProgress());
    }

    float size() {
        if (progress > 1) {
            return 0;
        }
        return Blib::easefunc_easeOutExpo(getProgress()) * max_size;
    }
};

struct Particles {
    int n;
    GLuint vao, vbo;

    std::vector<Point> points;
    int idx = 0;

    float startTime = 0.0;

    Particles(int n): n(n), points(std::vector<Point>(n)){}

    void startTimer(){
        startTime = Blib::getTimeElapsed();
    }

    bool keepGenerate() {
        float now = Blib::getTimeElapsed();
        return now - startTime < 1;
    }

    void render(Blib::ShaderProgram prog) {
        prog.Use();
        prog.SetMat4("model", glm::mat4(1));
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, n);
    }

    void update(glm::mat4 model) {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        int points_added = 0;
        int max_new_points_per_frame = 3;

        glm::vec4* g_points = (glm::vec4 *)glMapBufferRange(GL_ARRAY_BUFFER, 0, n * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
        for (int i = 0; i < n; i++) {
            if (keepGenerate() && points_added < max_new_points_per_frame && points[i].dead()) {
                points[i].init();
                points_added += 1;
            }
            points[i].update();
            g_points[i] = glm::vec4(points[i].getWorldPos(model), points[i].size());
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    void loadResources() {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, n * 3 * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, (void *)0);
    }
};
