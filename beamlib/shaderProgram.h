#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace Blib {

class ShaderProgram {
public:
    static ShaderProgram From(const char *vertexPath, const char *fragmentPath) {
        return ShaderProgram().Compile(vertexPath, fragmentPath);
    }

    unsigned int ID; 
    ShaderProgram Compile(const char *vertexPath, const char *fragmentPath);
    void Use() const { glUseProgram(ID); }

    ShaderProgram& SetBool(const std::string &name, bool value) { return glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value), *this; }
    ShaderProgram& SetInt(const std::string &name, int value) { return glUniform1i(glGetUniformLocation(ID, name.c_str()), value), *this; }
    ShaderProgram& SetFloat(const std::string &name, float value) { return glUniform1f(glGetUniformLocation(ID, name.c_str()), value), *this; }
    ShaderProgram& SetVec2(const std::string &name, const glm::vec2 &value) { return glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value)), *this; }
    ShaderProgram& SetVec3(const std::string &name, const glm::vec3 &value) { return glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value)), *this; }
    ShaderProgram& SetVec4(const std::string &name, const glm::vec4 &value) { return glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value)), *this; }
    ShaderProgram& SetMat4(const std::string &name, const glm::mat4 &mat) { return glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat)), *this; }

private:
    void CheckShaderCompileStatus(unsigned int shader);
    void CheckProgramLinkStatus(unsigned int program);
};

} // namespace Blib
