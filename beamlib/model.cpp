#include "model.h"
#include "stb_image.h"
#include <iostream>

void Blib::Model::loadModel(std::string path) {
    Assimp::Importer importer;
    importer.SetPropertyFloat("PP_GSN_MAX_SMOOTHING_ANGLE", 90);
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << importer.GetErrorString() << std::endl;
        exit(1);
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Blib::Model::processNode(aiNode *node, const aiScene *scene) {
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for (size_t i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Blib::Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.TexCoords = glm::vec2(0.0f);
        }
        vertices.push_back(vertex);
    }

    for (auto faceIt = mesh->mFaces; faceIt < mesh->mFaces + mesh->mNumFaces; faceIt++) {
        for (auto indexIt = faceIt->mIndices; indexIt < faceIt->mIndices + faceIt->mNumIndices; indexIt++) {
            indices.push_back(*indexIt);
        }
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    Lighting lighting;
    aiColor3D diffuse(0.0f);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    lighting.diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);

    aiColor3D ambient(0.0f);
    material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    lighting.ambient = glm::vec3(ambient.r, ambient.g, ambient.b);

    aiColor3D specular(0.0f);
    material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    lighting.specular = glm::vec3(specular.r, specular.g, specular.b);

    material->Get(AI_MATKEY_SHININESS, lighting.shininess);

    return Mesh(vertices, indices, textures, lighting);
}

std::vector<Texture> Blib::Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;

    for(size_t i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string filename = directory + "/" + str.C_Str();

        if (texturesLoaded.count(filename) != 0) {
            textures.push_back(texturesLoaded[filename]);
            continue;
        }

        Texture texture;
        texture.id = loadTexture(filename);
        texture.type = typeName;
        texture.path = filename;
        textures.push_back(texture);
        texturesLoaded[filename] = texture;
    }

    return textures;
}

GLuint Blib::loadTexture(const std::string &filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    GLenum format = GL_RGB;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

    if (data) {
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

GLuint Blib::loadCubeMap(const std::vector<std::string> &faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    GLenum format = GL_RGB;
    stbi_set_flip_vertically_on_load(false);

    for (size_t i = 0; i < faces.size(); i++) {
        std::string filename = faces[i];

        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data) {
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);


            stbi_image_free(data);
        } else {
            std::cout << "Cube map failed to load at path: " << filename << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

GLuint Blib::loadCubeMap(const std::string &folder, const std::string& ext) {
    std::vector<std::string> faces = {
        folder + "/px" + ext,
        folder + "/nx" + ext,
        folder + "/py" + ext,
        folder + "/ny" + ext,
        folder + "/pz" + ext,
        folder + "/nz" + ext,
    };
    return loadCubeMap(faces);
}
