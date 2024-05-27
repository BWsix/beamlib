#pragma once

#include <beamlib.h>
#include <glad/gl.h>

#include <glm/gtc/random.hpp>
#include <vector>

struct FireBall {
    float time_elapsed = -420;

    int max_size;
    float max_radius;
    glm::vec3 direction;

    FireBall() {
        max_radius = 2;
        max_size = glm::linearRand(1000, 1200);
        direction = glm::sphericalRand(max_radius);
        direction.y = glm::abs(direction.y) * 1.2;
    }

    void init(){
        time_elapsed = 0.0;
    }

    void update() {
        time_elapsed += Blib::getDeltaTime();
    }

    glm::vec3 getWorldPos(glm::mat4 model) {
        return glm::vec3(model * glm::vec4(direction * max_radius, 1.0));
    }

    bool dead() {
        return time_elapsed < 0 || time_elapsed > 6.5;
    }

    float size() {
        if (time_elapsed < 2) {
            float t = time_elapsed / 2;
            return Blib::easefunc_easeOutBack(t) * max_size;
        }
        if (time_elapsed < 5.5) {
            return max_size;
        }
        float t = 1 - (time_elapsed - 5.5);
        return Blib::easefunc_easeInQuart(t) * max_size;
    }
};

struct Explosion {
    const float end_time = 6.5;
    const int max_new_points_per_frame = 1;

    int n;
    GLuint vao, vbo;

    std::vector<FireBall> fires;
    int idx = 0;

    float startTime = 0.0;

    Explosion(int n): n(n), fires(std::vector<FireBall>(n)){}

    void startTimer(){
        startTime = Blib::getTimeElapsed();
    }

    bool keepAnimating() {
        float now = Blib::getTimeElapsed();
        return now - startTime < end_time;
    }

    void render(Blib::ShaderProgram prog, glm::mat4 model) {
        if (!keepAnimating()) {
            return;
        }

        glEnablei(GL_BLEND, 0);
        glBlendFunci(0, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        prog.Use();
        prog.SetFloat("time", Blib::getTimeElapsed());
        prog.SetMat4("model", model);
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, n);

        glDisablei(GL_BLEND, 0);
    }

    void update(glm::mat4 model, glm::vec3 cameraPos) {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glm::vec4* g_points = (glm::vec4 *)glMapBufferRange(GL_ARRAY_BUFFER, 0, n * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

        int points_added = 0;
        for (int i = 0; i < n; i++) {
            if (keepAnimating() && points_added < max_new_points_per_frame && fires[i].dead()) {
                fires[i].init();
                points_added += 1;
            }
            fires[i].update();
            g_points[i] = glm::vec4(fires[i].getWorldPos(model), fires[i].size());
        }

        // std::sort(g_points, g_points + n, [cameraPos](const glm::vec4& a, const glm::vec4& b){
        //     return glm::distance(glm::vec3(a), cameraPos) > glm::distance(glm::vec3(b), cameraPos);
        // });

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
