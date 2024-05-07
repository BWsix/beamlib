#include <beamlib.h>
#include "gumdam.h"
#include "screen.h"
#include "skybox.h"
#include "lilypad.h"

void generateTexture(GLuint &id, int width, int height, int color_attachment) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, color_attachment, id, 0);
}
void regenerateTexture(GLuint &id, int width, int height, int color_attachment) {
    glDeleteTextures(1, &id);
    generateTexture(id, width, height, color_attachment);
}

void generateRenderBuffer(GLuint &id, int width, int height) {
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id);
}
void regenerateRenderBuffer(GLuint &id, int width, int height) {
    glDeleteRenderbuffers(1, &id);
    generateRenderBuffer(id, width, height);
}

void setup() {
    GLuint& custom_framebuffer_object = Blib::ResourceManager::GetGLuint("custom-fbo");
    glGenFramebuffers(1, &custom_framebuffer_object);
    glBindFramebuffer(GL_FRAMEBUFFER, custom_framebuffer_object);

    GLuint& custom_render_buffer_id = Blib::ResourceManager::GetGLuint("custom-render-buffer-id");
    GLuint& custom_texture_id = Blib::ResourceManager::GetGLuint("custom-screen-texture");
    generateTexture(custom_texture_id, 800, 600, GL_COLOR_ATTACHMENT0);
    generateRenderBuffer(custom_render_buffer_id, 800, 600);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
        exit(1);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool devMode = false;

int main() {
    const auto window = Blib::CreateWindow("gundam");

    glfwSetWindowSizeCallback(window, [](GLFWwindow *, int width, int height){
        glViewport(0, 0, width, glm::max<int>(height, 1));
        Blib::camera.setAspect((float)Blib::WIDTH / Blib::HEIGHT);

        GLuint& custom_framebuffer_object = Blib::ResourceManager::GetGLuint("custom-fbo");
        glBindFramebuffer(GL_FRAMEBUFFER, custom_framebuffer_object);
        GLuint& custom_render_buffer_id = Blib::ResourceManager::GetGLuint("custom-render-buffer-id");
        GLuint& custom_texture_id = Blib::ResourceManager::GetGLuint("custom-screen-texture");
        generateTexture(custom_texture_id, width, height, GL_COLOR_ATTACHMENT0);
        generateRenderBuffer(custom_render_buffer_id, width, height);
    });

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setup();

    Blib::Instance scene("DEFAULT SCENE");
    scene.PushChild(&Blib::camera);

    Gundam::LoadResources();
    Gundam gumdam;
    scene.PushChild(&gumdam.root);
    gumdam.root.transform.Translate({0.0, 21.2, 0.0});
    gumdam.init();

    Skybox::LoadResources();
    Skybox skybox;

    Lilypad::LoadResources();
    Lilypad lilypad;
    lilypad.root.transform.Scale({1.0, 40.0, 40.0});
    lilypad.root.transform.RotateLocal({0.0, 0.0, 1.571});
    lilypad.root.transform.Translate({0.55, 0.1, 40});

    Screen::LoadResources();
    Screen::LoadOtherPostProcessingResources();
    Screen screen;

    while (!Blib::WindowShouldClose(window)) {
        glfwPollEvents();
        Blib::mouse.Update(window);
        Blib::camera.Update();

        Blib::useFrameBuffer("custom-fbo");
        skybox.render();
        gumdam.render();
        lilypad.render();

        Blib::useFrameBuffer();

        static bool pixelation = false;
        static int pixelation_size = 10;
        if (pixelation) {
            auto prog = Blib::ResourceManager::GetShader("pixelation");
            prog.Use();
            prog.SetVec2("screenSize", {Blib::WIDTH, Blib::HEIGHT});
            prog.SetFloat("pixelSize", pixelation_size);
            screen.render(prog);
        } else {
            screen.render(Blib::ResourceManager::GetShader("screen"));
        }

        if (!ImGui::GetIO().WantCaptureKeyboard) {
            if (ImGui::IsKeyPressed(ImGuiKey_T)) devMode = !devMode;
        }

        Blib::BeginUI();
        {
            ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
            {
                ImGui::Text("fps: %.0f", Blib::getFrameRate());
                ImGui::Text("frametime: %f", Blib::getDeltaTime());
                ImGui::Text("dev mode: %s (press t to toggle)", devMode ? "on" : "off");
                ImGui::Checkbox("Enable Pixelation", &pixelation);
                ImGui::SliderInt("Size", &pixelation_size, 1, Blib::HEIGHT);
            }
            ImGui::End();

            gumdam.renderControlPanel();
            if (devMode) {
                ImGui::Begin("Objects");
                {
                    gumdam.renderUI();
                    lilypad.renderUI();
                    Blib::camera.RenderUI();
                }
                ImGui::End();
            }
        }
        Blib::EndUI();

        glfwSwapBuffers(window);
    }

    Blib::DestroyWindow(window);

    return 0;
}
