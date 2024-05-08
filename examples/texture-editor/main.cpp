#include "grid.h"
#include "mesh.h"
#include <beamlib.h>
#include <iostream>
#include <limits>

const unsigned int max_uint = std::numeric_limits<unsigned int>::max();
size_t WIDTH = 800;
size_t HEIGHT = 600;

MyMesh mesh;
GLuint fbo = max_uint;
GLuint fboFaceID = max_uint;
GLuint rbo = max_uint;

void setupFramebuffer() {
    if (fbo == max_uint) {
        glGenFramebuffers(1, &fbo);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    if (fboFaceID != max_uint) {
        glDeleteTextures(1, &fboFaceID);
    }
    glGenTextures(1, &fboFaceID);
    glBindTexture(GL_TEXTURE_2D, fboFaceID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, WIDTH, HEIGHT, 0, GL_RED_INTEGER,
                 GL_UNSIGNED_INT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fboFaceID, 0);

    if (fbo != max_uint) {
        glDeleteRenderbuffers(1, &rbo);
    }
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
        exit(1);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void init() {
    setupFramebuffer();

    if (!mesh.LoadFromFile("models/dragon/xyzrgb_dragon_100k.obj")) {
        fprintf(stderr, "failed to load model\n");
        exit(1);
    } else {
        std::cout << "loaded gragon\n";
    }
    mesh.setup();
}

int main() {
    const auto window = Blib::CreateWindow("texture editor", Blib::WIDTH, Blib::HEIGHT);
    glfwSetWindowSizeCallback(window, [](GLFWwindow *, int width, int height) {
        WIDTH = width;
        HEIGHT = height;
        glViewport(0, 0, width, glm::max<int>(height, 1));
        Blib::camera.setAspect((float)width / height);
        setupFramebuffer();
    });

    Blib::camera.setMovementSpeed(0.5);

    Grid::LoadResources();
    Grid g;

    init();
    auto prog = Blib::ResourceManager::LoadShader(
        "prog", "shaders/editor.vert.glsl", "shaders/editor.frag.glsl");
    auto progFaceId = Blib::ResourceManager::LoadShader(
        "prog-faceid", "shaders/editor.vert.glsl",
        "shaders/editor-faceid.frag.glsl");

    std::set<uint> highlightedFaceIDs;

    unsigned int *ids = new unsigned int[WIDTH * HEIGHT];
    while (!Blib::WindowShouldClose(window)) {
        Blib::mouse.Update(window);
        Blib::camera.Update();

        ids = (unsigned int *)realloc(ids, sizeof(unsigned int) * WIDTH * HEIGHT);

        // draw face id to custom frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClearColor(0, 0, 0, 1);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        progFaceId.Use();
        progFaceId.SetMat4("model", glm::mat4(1));
        progFaceId.SetMat4("view", Blib::camera.getViewMatrix());
        progFaceId.SetMat4("projection", Blib::camera.getProjectionMatrix());
        mesh.draw(progFaceId);

        glBindTexture(GL_TEXTURE_2D, fboFaceID);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, ids);
        glBindTexture(GL_TEXTURE_2D, 0);

        auto pos = ImGui::GetMousePos();
        size_t id = ids[size_t((HEIGHT - pos.y) * WIDTH + pos.x)];
        if (ImGui::IsKeyDown(ImGuiKey_MouseRight)) {
            highlightedFaceIDs.clear();
        }
        if (ImGui::IsKeyDown(ImGuiKey_MouseLeft)) {
            if (id != 0) { 
                highlightedFaceIDs.insert(id);
            }
        }

        std::vector<uint> highlightedVerticies;
        for (auto id : highlightedFaceIDs) {
            const auto fh = mesh.face_handle(id - 1);
            for (auto it = mesh.fv_begin(fh); it != mesh.fv_end(fh); ++it) {
                highlightedVerticies.push_back((uint)it->idx());
            }
        }

        // draw color to default frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0, 0, 0, 1);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        prog.Use();
        prog.SetMat4("model", glm::mat4(1));

        prog.SetVec3("color", glm::vec3(1, 0, 0));
        mesh.highlight(prog, highlightedVerticies);

        prog.SetVec3("color", glm::vec3(1));
        mesh.draw(prog);

        Blib::BeginUI();
        {
            ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
            {
                ImGui::Text("fps: %.0f", Blib::getFrameRate());
                ImGui::Text("frametime: %f", Blib::getDeltaTime());
            }
            ImGui::End();
        }
        Blib::EndUI();

        glfwSwapBuffers(window);
    }

    Blib::DestroyWindow(window);

    return 0;
}
