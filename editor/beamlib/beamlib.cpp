#include "beamlib.h"

#include <cstdio>

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

namespace beamlib {
Mouse mouse;
Camera camera;
}

GLFWwindow *beamlib::CreateWindow(const char *title, int width, int height) {
    GLFWwindow *window;

    glfwSetErrorCallback([](int error, const char *description) {
        std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    });

    assert(glfwInit() && "Failed to initialize GLFW ._.");

    // GL 4.6 + GLSL 460
    const char *glsl_version = "#version 460";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // setup window
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    assert(window && "Failed to create window ._.");
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // setup resize callback
    glfwSetWindowSizeCallback(window, [](GLFWwindow *, int width, int height){
        glViewport(0, 0, width, glm::max<int>(height, 1));
        beamlib::camera.setAspect((float)width / height);
    });

    // setup GLAD
    assert(gladLoadGL(glfwGetProcAddress) && "Something went wrong with glad ._.");

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

bool beamlib::WindowShouldClose(GLFWwindow *window) {
    glfwPollEvents();
    glfwSwapBuffers(window);
    camera.Update(window);
    mouse.Update(window);
    return glfwWindowShouldClose(window);
}

void beamlib::DestroyWindow(GLFWwindow *window) {
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void beamlib::BeginUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void beamlib::EndUI() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

double beamlib::getTimeElapsed() { return glfwGetTime(); }
float beamlib::getFrameRate() { return ImGui::GetIO().Framerate; }
float beamlib::getDeltaTime() { return ImGui::GetIO().DeltaTime; }

void beamlib::dbg(glm::vec3 vec) { std::cout << vec[0] << ' ' << vec[1] << ' ' << vec[2] << '\n'; }
void beamlib::dbg(glm::quat q) { std::cout << q[0] << ' ' << q[1] << ' ' << q[2] << ' ' << q[3] << '\n'; }
void beamlib::dbg(glm::mat4 mat) { for (size_t i = 0; i < 4; i++) std::cout << mat[i][0] << ' ' << mat[i][1] << ' ' << mat[i][2] << ' ' << mat[i][3] << '\n'; }
std::vector<float> beamlib::toVector(glm::vec3 data) { return std::vector<float>{data[0], data[1], data[2]}; }
std::vector<float> beamlib::toVector(glm::quat data) { return std::vector<float>{data[0], data[1], data[2], data[3]}; }
std::vector<float> beamlib::toVector(glm::mat4 data) {
    std::vector<float> v(16);
    for (size_t i = 0; i < 16; i++) v[i] = glm::value_ptr(data)[i];
    return v;
}
glm::vec3 beamlib::vectorToVec3(std::vector<float> data) { return glm::vec3(data[0], data[1], data[2]); }
glm::quat beamlib::vectorToQuat(std::vector<float> data) {
    glm::quat quat;
    for (size_t i = 0; i < 4; i++) { glm::value_ptr(quat)[i] = data[i]; }
    return quat;
}
glm::mat4 beamlib::vectorToMat4(std::vector<float> data) {
    glm::mat4 m;
    for (size_t i = 0; i < 16; i++) glm::value_ptr(m)[i] = data[i];
    return m;
}

void beamlib::Mouse::Update(GLFWwindow *window) {
    auto rightButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (rightButtonState == GLFW_PRESS && rightButtonPrssed == false) {
        rightButtonPrssed = true;
        glfwGetCursorPos(window, &mouseDownX, &mouseDownY);
    } 
    if (rightButtonPrssed) {
        double mouseCurrentX, mouseCurrentY;
        glfwGetCursorPos(window, &mouseCurrentX, &mouseCurrentY);
        glfwSetCursorPos(window, mouseDownX, mouseDownY);

        deltaRight = {mouseCurrentX - mouseDownX, mouseCurrentY - mouseDownY};
    }
    if (rightButtonState == GLFW_RELEASE && rightButtonPrssed == true) {
        rightButtonPrssed = false;
        deltaRight = {0, 0};
    }
}

glm::mat4 beamlib::Transform::getModelMatrix() const {
    glm::mat4 model(1);
    glm::mat4 parentModel = parent ? parent->getModelMatrixForChildren() : glm::mat4(1);
    return parentModel
    * glm::toMat4(rotationQuat) 
    * glm::translate(model, position)
    * glm::toMat4(localRotationQuat)
    * glm::scale(model, scale);
}

glm::mat4 beamlib::Transform::getModelMatrixForChildren() const {
    glm::mat4 parentModel = parent ? parent->getModelMatrixForChildren() : glm::mat4(1);
    return parentModel
    * glm::toMat4(rotationQuat) 
    * glm::translate(glm::mat4(1), position);
}

void beamlib::Transform::RenderUI(std::string name) {
    ImGui::DragFloat3(("position##" + name).c_str(), glm::value_ptr(position), 0.05);
    ImGui::DragFloat3(("scale##" + name).c_str(), glm::value_ptr(scale), 0.05);

    glm::vec3 newRotation = getRotationEuler();
    ImGui::DragFloat3(("rotation##" + name).c_str(), glm::value_ptr(newRotation), 0.05);
    glm::vec3 deltaRotation = newRotation - getRotationEuler();
    Rotate(glm::angleAxis(deltaRotation[0], glm::vec3(1, 0, 0)));
    Rotate(glm::angleAxis(deltaRotation[1], glm::vec3(0, 1, 0)));
    Rotate(glm::angleAxis(deltaRotation[2], glm::vec3(0, 0, 1)));

    glm::vec3 newLocalRotation = getLocalRotationEuler();
    ImGui::DragFloat3(("localRotation##" + name).c_str(), glm::value_ptr(newLocalRotation), 0.05);
    glm::vec3 deltaLocalRotation = newLocalRotation - getLocalRotationEuler();
    RotateLocal(glm::angleAxis(deltaLocalRotation[0], glm::vec3(1, 0, 0)));
    RotateLocal(glm::angleAxis(deltaLocalRotation[1], glm::vec3(0, 1, 0)));
    RotateLocal(glm::angleAxis(deltaLocalRotation[2], glm::vec3(0, 0, 1)));
}

// TODO: implement `object lock`
void beamlib::Camera::Update(GLFWwindow* window) {
    if (beamlib::mouse.getRightButtonPressed()) {
        // TODO: change to euler based
        yaw += sensitivity * beamlib::mouse.getDeltaRight().x;
        pitch -= sensitivity * beamlib::mouse.getDeltaRight().y;
        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }
    }

    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::vec3 frontXZ = glm::normalize(glm::vec3(getFront().x, 0, getFront().z));
    glm::vec3 right = glm::normalize(glm::cross(frontXZ, up));

    if (ImGui::IsKeyDown(ImGuiKey_W)) {
        transform.Translate(movementSpeed * frontXZ * beamlib::getDeltaTime());
    }
    if (ImGui::IsKeyDown(ImGuiKey_S)) {
        transform.Translate(-movementSpeed * frontXZ * beamlib::getDeltaTime());
    }
    if (ImGui::IsKeyDown(ImGuiKey_D)) {
        transform.Translate(movementSpeed * right * beamlib::getDeltaTime());
    }
    if (ImGui::IsKeyDown(ImGuiKey_A)) {
        transform.Translate(-movementSpeed * right * beamlib::getDeltaTime());
    }
    if (ImGui::IsKeyDown(ImGuiKey_Space)) {
        transform.Translate(movementSpeed * up * beamlib::getDeltaTime());
    }
    if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
        transform.Translate(-movementSpeed * up * beamlib::getDeltaTime());
    }
}

void beamlib::ShaderProgram::CheckShaderCompileStatus(GLuint shader) {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        std::fprintf(stderr, "Failed to compile shader: %s\n", infoLog);
        exit(1);
    }
}

void beamlib::ShaderProgram::CheckProgramLinkStatus(GLuint program) {
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
        std::fprintf(stderr, "Failed to link shader program: %s\n", infoLog);
        exit(1);
    }
}

const char *beamlib::ShaderProgram::SlurpFile(const char *path) {
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

beamlib::ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath) {
    id = glCreateProgram();

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexShaderSource = SlurpFile(vertexPath);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    CheckShaderCompileStatus(vertexShader);
    glAttachShader(id, vertexShader);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentShaderSource = SlurpFile(fragmentPath);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    CheckShaderCompileStatus(fragmentShader);
    glAttachShader(id, fragmentShader);

    glLinkProgram(id);
    CheckProgramLinkStatus(id);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

