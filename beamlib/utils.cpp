#include "utils.h"
#include "camera.h"
#include "resourceManager.h"

#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Blib {
    int WIDTH = 1920;
    int HEIGHT = 1080;
}

GLFWwindow *Blib::CreateWindow(const char *title, int width, int height) {
    GLFWwindow *window;

    glfwSetErrorCallback([](int error, const char *description) {
        std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    });
    if (!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW ._.\n");
    }

    // GL 4.5 + GLSL 450
    const char *glsl_version = "#version 450";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // setup window
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    assert(window && "Failed to create window ._.");
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // setup GLAD
    if (!gladLoadGL(glfwGetProcAddress)) {
        fprintf(stderr, "Something went wrong with glad ._.\n");
    }

    // setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return window;
}

bool Blib::WindowShouldClose(GLFWwindow *window) {
    glfwPollEvents();
    return glfwWindowShouldClose(window);
}

void Blib::DestroyWindow(GLFWwindow *window) {
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Blib::BeginUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Blib::EndUI() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

double Blib::getTimeElapsed() { return glfwGetTime(); }
float Blib::getFrameRate() { return ImGui::GetIO().Framerate; }
float Blib::getDeltaTime() { return ImGui::GetIO().DeltaTime; }
const char *Blib::SlurpFile(const char *path) {
    FILE *fp = std::fopen(path, "rb");
    if (!fp) {
        std::fprintf(stderr, "Failed to open %s\n", path);
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    size_t sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *src = new char[sz + 1];
    src[sz] = '\0';
    size_t bytes_read = fread(src, sizeof(char), sz, fp);
    if (bytes_read != sz) {
        std::fprintf(stderr, "Failed to fully read content of %s\n", path);
        exit(1);
    }
    fclose(fp);
    return src;
}

void Blib::dbg(glm::vec3 vec) { std::cout << vec[0] << ' ' << vec[1] << ' ' << vec[2] << '\n'; }
void Blib::dbg(glm::quat q) { std::cout << q[0] << ' ' << q[1] << ' ' << q[2] << ' ' << q[3] << '\n'; }
void Blib::dbg(glm::mat4 mat) { for (size_t i = 0; i < 4; i++) std::cout << mat[i][0] << ' ' << mat[i][1] << ' ' << mat[i][2] << ' ' << mat[i][3] << '\n'; }
void Blib::dbg(Blib::FrameData f) {
    printf("idx%zu:\nFROM:%s\nTO:%s\nprogress:%f\n", f.idx, f.from.dump().c_str(), f.to.dump().c_str(), f.progress);
}
std::vector<float> Blib::toVector(glm::vec3 data) { return std::vector<float>{data[0], data[1], data[2]}; }
std::vector<float> Blib::toVector(glm::quat data) { return std::vector<float>{data[0], data[1], data[2], data[3]}; }
std::vector<float> Blib::toVector(glm::mat4 data) {
    std::vector<float> v(16);
    for (size_t i = 0; i < 16; i++) v[i] = glm::value_ptr(data)[i];
    return v;
}
glm::vec3 Blib::vectorToVec3(std::vector<float> data) { return glm::vec3(data[0], data[1], data[2]); }
glm::quat Blib::vectorToQuat(std::vector<float> data) {
    glm::quat quat;
    for (size_t i = 0; i < 4; i++) { glm::value_ptr(quat)[i] = data[i]; }
    return quat;
}
glm::mat4 Blib::vectorToMat4(std::vector<float> data) {
    glm::mat4 m;
    for (size_t i = 0; i < 16; i++) glm::value_ptr(m)[i] = data[i];
    return m;
}

void Blib::useFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Blib::useFrameBuffer(const std::string& id) {
    GLuint& fbo = ResourceManager::GetGLuint(id);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Blib::setupBuffer(GLuint id, int width, int height, int internal, int format, int type, int attachment) {
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, format, type, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, id, 0);
}

