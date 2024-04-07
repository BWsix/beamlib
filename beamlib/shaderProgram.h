#pragma once

#include "utils.h"

namespace Blib {

class ShaderProgram {
    GLuint id;

    void CheckShaderCompileStatus(GLuint shader);
    void CheckProgramLinkStatus(GLuint program);

public:
    GLuint getId() const { return id; }

    void Load(const char *vertexPath, const char *fragmentPath);
    void Use() const { glUseProgram(id); }

    void setUniformBool(const std::string &name, bool value) const { glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); }
    void setUniformInt(const std::string &name, int value) const { glUniform1i(glGetUniformLocation(id, name.c_str()), value); }
    void setUniformFloat(const std::string &name, float value) const { glUniform1f(glGetUniformLocation(id, name.c_str()), value); }
    void setUniformVec2(const std::string &name, const glm::vec2 &value) const { glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value)); }
    void setUniformVec3(const std::string &name, const glm::vec3 &value) const { glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value)); }
    void setUniformVec4(const std::string &name, const glm::vec4 &value) const { glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value)); }
    void setUniformMat4(const std::string &name, const glm::mat4 &mat) const { glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
};

} // namespace Blib
