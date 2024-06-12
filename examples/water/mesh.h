#pragma once
#include <beamlib.h>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

struct MyVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords = {0, 0};

    static MyVertex Load(const json& j) {
        return {
            glm::vec3(
                j["p"][0].is_null() ? 0.0 : float(j["p"][0]),
                j["p"][1].is_null() ? 0.0 : float(j["p"][1]),
                j["p"][2].is_null() ? 0.0 : float(j["p"][2])
            ),
            glm::vec3(
                j["n"][0].is_null() ? 0.0 : float(j["n"][0]),
                j["n"][1].is_null() ? 0.0 : float(j["n"][1]),
                j["n"][2].is_null() ? 0.0 : float(j["n"][2])
            ),
            glm::vec2(
                j["t"][0].is_null() ? 0.0 : float(j["t"][0]),
                j["t"][1].is_null() ? 0.0 : float(j["t"][1])
            ),
        };
    }

    json serialize() const {
        json j;
        j["p"] = json::array({position.x, position.y, position.z});
        j["n"] = json::array({normal.x, normal.y, normal.z});
        j["t"] = json::array({texCoords.x, texCoords.y});
        return j;
    }
};

class MyMesh {
    std::vector<MyVertex> my_vertices;
    std::vector<GLuint> my_indices;
    std::vector<GLuint> my_indices_original;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

public:
    std::string name;
    std::string texture_id;

    float scale = 1.0;
    bool flip_horizontally = false;
    bool flip_vertically = false;

    MyMesh() = default;
    MyMesh(std::string texture_id, std::string name, float scale) : name(name), texture_id(texture_id), scale(scale) {}

    static MyMesh Load(const json& j) {
        MyMesh m(j["texture_id"], j["name"], (j["scale"].is_number() ? float(j["scale"]) : 1.0));
        std::vector<MyVertex> my_vertices;
        for (const auto& v : j["vertices"]) {
            my_vertices.push_back(MyVertex::Load(v));
        }
        m.loadVertices(my_vertices, j["indices"], j["original_indices"]);

        if (j.find("fv") != j.end()) m.flip_vertically = j["fv"];
        if (j.find("fh") != j.end()) m.flip_horizontally= j["fh"];

        return m;
    }

    void loadVertices(std::vector<MyVertex> vertices, std::vector<GLuint> indices, std::vector<GLuint> original_indices) {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        my_vertices = vertices;

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, my_vertices.size() * sizeof(my_vertices[0]), my_vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (void *)offsetof(MyVertex, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (void *)offsetof(MyVertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (void *)offsetof(MyVertex, texCoords));

        my_indices = indices;
        my_indices_original = original_indices;

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, my_indices.size() * sizeof(my_indices[0]), my_indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    void render() const {
        auto prog = Blib::ResourceManager::GetShader("dragon");
        prog.SetFloat("scale", scale);
        prog.SetBool("fv", flip_vertically);
        prog.SetBool("fh", flip_horizontally);

        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glDrawElements(GL_TRIANGLES, my_indices.size(), GL_UNSIGNED_INT, NULL);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

};
