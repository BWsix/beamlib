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

    void SetBool(const std::string &name, bool value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); }
    void SetInt(const std::string &name, int value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
    void SetFloat(const std::string &name, float value) const { glUniform1f(glGetUniformLocation(ID, name.c_str()), value); }
    void SetVec2(const std::string &name, const glm::vec2 &value) const { glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value)); }
    void SetVec3(const std::string &name, const glm::vec3 &value) const { glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value)); }
    void SetVec4(const std::string &name, const glm::vec4 &value) const { glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value)); }
    void SetMat4(const std::string &name, const glm::mat4 &mat) const { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat)); }

private:
    void CheckShaderCompileStatus(unsigned int shader);
    void CheckProgramLinkStatus(unsigned int program);
};

} // namespace Blib
