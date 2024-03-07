#include "beamlib.h"

#include <cstdio>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glad/gl.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

GLFWwindow *beamlib::CreateWindow(int width, int height, const char *title) {
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
