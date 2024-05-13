#include "shaderProgram.h"
#include "stb_include.h"

const char *keyword = "include";
const char *folder = "shaders";

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

Blib::ShaderProgram Blib::ShaderProgram::Compile(const char* vertexPath, const char* fragmentPath) {
    ID = glCreateProgram();

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    char *vertexShaderSource = stb_include_file((char *)vertexPath, (char *)keyword, (char *)folder, NULL);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    CheckShaderCompileStatus(vertexShader);
    glAttachShader(ID, vertexShader);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char *fragmentShaderSource = stb_include_file((char *)fragmentPath, (char *)keyword, (char *)folder, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    CheckShaderCompileStatus(fragmentShader);
    glAttachShader(ID, fragmentShader);

    glLinkProgram(ID);
    CheckProgramLinkStatus(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    free(vertexShaderSource);
    free(fragmentShaderSource);
    
    return *this;
}
