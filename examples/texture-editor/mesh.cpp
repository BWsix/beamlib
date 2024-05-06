#include "mesh.h"
#include <vector>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

bool MyMesh::LoadFromFile(std::string filename) {
    OpenMesh::IO::Options opt = OpenMesh::IO::Options::VertexNormal;
    bool isRead = OpenMesh::IO::read_mesh(*this, filename, opt);
    if (isRead) {
        // If the file did not provide vertex normals and mesh has vertex normal, then calculate them
        if (!opt.check(OpenMesh::IO::Options::VertexNormal) &&
            this->has_vertex_normals()) {
            this->update_normals();
        }
    }
    return isRead;
}

void MyMesh::setup() {
    std::vector<MyVertex> vertices;
    for (FaceHandle f : this->faces()) {
        // this is basically a triangle fan for any face valence
        MyMesh::ConstFaceVertexCCWIter it = this->cfv_ccwbegin(f);
        VertexHandle first = *it;
        ++it;
        int face_triangles = this->valence(f) - 2;
        for (int i = 0; i < face_triangles; ++i) {
            vertices.push_back({ (GLuint)f.idx() + 1, d2f(point(first)),d2f(normal(first))});
            vertices.push_back({ (GLuint)f.idx() + 1, d2f(point(*it)),d2f(normal(*it))});
            ++it;
            vertices.push_back({ (GLuint)f.idx() + 1, d2f(point(*it)),d2f(normal(*it))});
        }
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MyVertex), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (void*)offsetof(MyVertex, Position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (void*)offsetof(MyVertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (void*)offsetof(MyVertex, TexCoords));
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(MyVertex), (void*)offsetof(MyVertex, FaceID));
    glEnableVertexAttribArray(4);
    glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT, sizeof(MyVertex), (void*)offsetof(MyVertex, TexID));

    std::vector<glm::vec3> positions;
    for (VertexHandle vh : this->vertices()) {
        positions.push_back(d2f(point(vh)));
    }

    glGenVertexArrays(1, &VAO_POSITIONS);
    glBindVertexArray(VAO_POSITIONS);

    glGenBuffers(1, &VBO_POSITIONS);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_POSITIONS);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
}

void MyMesh::draw(Blib::ShaderProgram &shader) const {
    shader.Use();
    shader.SetMat4("view", Blib::camera.getViewMatrix());
    shader.SetMat4("projection", Blib::camera.getProjectionMatrix());

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->n_faces() * 3);
    glBindVertexArray(0);
}

void MyMesh::highlight(Blib::ShaderProgram &shader, const std::vector<uint>& indicies) const {
    shader.Use();
    shader.SetMat4("view", Blib::camera.getViewMatrix());
    shader.SetMat4("projection", Blib::camera.getProjectionMatrix());

    glBindVertexArray(VAO_POSITIONS);
    glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, indicies.data());
    glBindVertexArray(0);
}

OpenMesh::Vec3d MyMesh::normal(const HalfedgeHandle he) const {
    const FaceHandle f = face_handle(he);
    if (f.is_valid()) {
        return normal(f);
    } else {
        return OpenMesh::Vec3d(0, 0, 0);
    }
}

OpenMesh::Vec3d MyMesh::normal(const EdgeHandle e) const {
    const HalfedgeHandle he0 = halfedge_handle(e, 0);
    const HalfedgeHandle he1 = halfedge_handle(e, 1);
    if(!is_boundary(he0) || !is_boundary(he1)) {
        // free edge, bad
        exit(1);
    }
    if (is_boundary(he0)) {
        return normal(face_handle(he1));
    } else if (is_boundary(he1)) {
        return normal(face_handle(he0));
    } else {
        return (normal(face_handle(he0)) + normal(face_handle(he1))).normalized();
    }
}

OpenMesh::Vec3d MyMesh::normal(const FaceHandle f) const {
    return OpenMesh::TriMesh_ArrayKernelT<MyTraits>::normal(f);
}

OpenMesh::Vec3d MyMesh::normal(const VertexHandle v) const {
    return OpenMesh::TriMesh_ArrayKernelT<MyTraits>::normal(v);
}

glm::vec3 MyMesh::d2f(OpenMesh::Vec3d v) { return glm::vec3(v[0], v[1], v[2]); }
