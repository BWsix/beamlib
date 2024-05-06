#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <beamlib.h>

struct MyTraits : OpenMesh::DefaultTraits {
    // Define vertex and normal as double
    using Point = OpenMesh::Vec3d;
    using Normal = OpenMesh::Vec3d;

    // Add normal property to vertices and faces
    VertexAttributes(OpenMesh::Attributes::Normal);
    FaceAttributes(OpenMesh::Attributes::Normal);
};

struct MyVertex {
    GLuint FaceID;
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords = {0, 0};
    GLuint TexID = 0;
};

class MyMesh : public OpenMesh::TriMesh_ArrayKernelT<MyTraits> {
public:
    bool LoadFromFile(std::string filename);
    void setup();

    void draw(Blib::ShaderProgram &shader) const;
    void highlight(Blib::ShaderProgram &shader, const std::vector<uint>& indicies) const;

    glm::vec3 d2f(OpenMesh::Vec3d v);
private:
    // halfedge, face, and vertex normals
    OpenMesh::Vec3d normal(const HalfedgeHandle he) const;
    OpenMesh::Vec3d normal(const EdgeHandle e) const;
    OpenMesh::Vec3d normal(const FaceHandle f) const;
    OpenMesh::Vec3d normal(const VertexHandle v) const;


private:
    GLuint VAO;
    GLuint VAO_POSITIONS;
    GLuint VBO;
    GLuint VBO_POSITIONS;
};
