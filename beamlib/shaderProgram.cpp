#include "shaderProgram.h"

#define STB_INCLUDE_IMPLEMENTATION
#define STB_INCLUDE_LINE_GLSL
#include "stb_include.h"

void Blib::ShaderProgram::CheckShaderCompileStatus(GLuint shader) {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        std::fprintf(stderr, "Failed to compile shader: %s\n", infoLog);
        exit(1);
    }
}

void Blib::ShaderProgram::CheckProgramLinkStatus(GLuint program) {
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
        std::fprintf(stderr, "Failed to link shader program: %s\n", infoLog);
        exit(1);
    }
}

void Blib::ShaderProgram::Load(const char* vertexPath, const char* fragmentPath) {
    id = glCreateProgram();

    char keyword[] = "include";
    char folder[] = "shaders";

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexShaderSource = stb_include_file(const_cast<char *>(vertexPath), keyword, folder, NULL);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    CheckShaderCompileStatus(vertexShader);
    glAttachShader(id, vertexShader);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentShaderSource = stb_include_file(const_cast<char *>(fragmentPath), keyword, folder, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    CheckShaderCompileStatus(fragmentShader);
    glAttachShader(id, fragmentShader);

    glLinkProgram(id);
    CheckProgramLinkStatus(id);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

