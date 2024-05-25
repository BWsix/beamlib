#pragma once

#include "gymbar.h"
#include <string>
#include <beamlib.h>
#include "particles.h"

class LazerInstance : public Blib::Instance {
public:
    using Blib::Instance::Instance;

    void CustomRender(Blib::ShaderProgram prog) override {
        prog = Blib::ResourceManager::GetShader("gumdam-lazer");
        prog.Use();
        prog.SetMat4("model", transform.getModelMatrix());
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());
        model.draw(prog);
    }
};

class Gumdam {
    Blib::Instance body{"body", Blib::ResourceManager::GetModel("gumdam-body")};
        Blib::Instance head{"head", Blib::ResourceManager::GetModel("gumdam-head")};
        Blib::Instance back{"back", Blib::ResourceManager::GetModel("gumdam-back")};
        Blib::Instance lshouder{"lshouder", Blib::ResourceManager::GetModel("gumdam-lshouder")};
            Blib::Instance ulefthand{"ulefthand", Blib::ResourceManager::GetModel("gumdam-ulefthand")};
                Blib::Instance dlefthand{"dlefthand", Blib::ResourceManager::GetModel("gumdam-dlefthand")};
                    Blib::Instance lefthand{"lefthand", Blib::ResourceManager::GetModel("gumdam-lefthand")};
        Blib::Instance rshouder{"rshouder", Blib::ResourceManager::GetModel("gumdam-rshouder")};
            Blib::Instance urighthand{"urighthand", Blib::ResourceManager::GetModel("gumdam-urighthand")};
                Blib::Instance drighthand{"drighthand", Blib::ResourceManager::GetModel("gumdam-drighthand")};
                    Blib::Instance righthand{"righthand", Blib::ResourceManager::GetModel("gumdam-righthand")};
        Blib::Instance dbody{"dbody", Blib::ResourceManager::GetModel("gumdam-dbody")};
            Blib::Instance uleftleg{"uleftleg", Blib::ResourceManager::GetModel("gumdam-uleftleg")};
                Blib::Instance dleftleg{"dleftleg", Blib::ResourceManager::GetModel("gumdam-dleftleg")};
                    Blib::Instance leftfoot{"leftfoot", Blib::ResourceManager::GetModel("gumdam-leftfoot")};
            Blib::Instance urightleg{"urightleg", Blib::ResourceManager::GetModel("gumdam-urightleg")};
                Blib::Instance drightleg{"drightleg", Blib::ResourceManager::GetModel("gumdam-drightleg")};
                    Blib::Instance rightfoot{"rightfoot", Blib::ResourceManager::GetModel("gumdam-rightfoot")};

        LazerInstance lazer{"beam", Blib::ResourceManager::GetModel("gumdam-lazer")};
        LazerInstance fireball{"ball", Blib::ResourceManager::GetModel("gumdam-fireball")};

    Gymbar gymbar;

public:
    enum class State {
        Idle, Walking, JumpingJack, Squating, Gymbaring, PuruVictorying, Kemehanehaing,
    };
    State state = State::Idle;

    Blib::Instance root{"gumdam-root"};
    Blib::Animator gumdamAnimator{"gundam animator", &body};

    Blib::Animator cameraAnimator{"camera animator", &Blib::camera};

    Gumdam() {
        Blib::camera.setTargetInstance(&root);
        root.PushChild(&body);

        body.PushChild(&head);
        body.PushChild(&back);
        body.PushChild(&lshouder);
            lshouder.PushChild(&ulefthand);
                ulefthand.PushChild(&dlefthand);
                    dlefthand.PushChild(&lefthand);
        body.PushChild(&rshouder);
            rshouder.PushChild(&urighthand);
                urighthand.PushChild(&drighthand);
                    drighthand.PushChild(&righthand);
        body.PushChild(&dbody);
            dbody.PushChild(&urightleg);
                urightleg.PushChild(&drightleg);
                    drightleg.PushChild(&rightfoot);
            dbody.PushChild(&uleftleg);
                uleftleg.PushChild(&dleftleg);
                    dleftleg.PushChild(&leftfoot);
        body.PushChild(&lazer);
        body.PushChild(&fireball);
    }

    bool particle_on = false;
    Particles kame_particle = Particles(500);

    void update() {
        gumdamAnimator.Update();
        cameraAnimator.Update();
        if (particle_on) {
            glm::mat4 m = lazer.transform.getModelMatrixNoScale();
            kame_particle.update(m);
        }
    }

    void render(Blib::ShaderProgram prog, glm::vec3 lightPos, bool withPrevModel = false) {
        prog.SetVec3("viewPos", Blib::camera.getPosition());
        prog.SetVec3("lightPos", lightPos);
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());
        prog.SetMat4("prevViewProjection", Blib::camera.getPrevViewProjectionMatrix());
        if (withPrevModel) {
            root.RenderWithPrevModel(prog);
        } else {
            root.Render(prog);
        }

        if (particle_on) {
            kame_particle.render(Blib::ResourceManager::GetShader("gumdam-particles"));
        }

        if (state == State::Gymbaring) {
            gymbar.render();
        }
    }

    void renderUI() {
        ImGui::Begin("Gundam Animation Editor");
        gumdamAnimator.renderUI();
        ImGui::End();

        ImGui::Begin("Camera Animation Editor");
        cameraAnimator.renderUI();
        ImGui::End();

        root.RenderUI();
        gymbar.renderUI();
    }

    void renderControlPanel() {
        ImGui::Begin("Gundam Control");

        if (state != State::Idle) {
            if (state != State::PuruVictorying && state != State::Kemehanehaing) {
                if (ImGui::Button("Stop##Gundam")) end();
            }
        } else {
            if (ImGui::Button("Walk##Gundam")) walk();
            ImGui::SameLine();
            if (ImGui::Button("Jumping Jack##Gundam")) jumping_jack();
            ImGui::SameLine();
            if (ImGui::Button("Squat##Gundam")) squat();
            ImGui::SameLine();
            if (ImGui::Button("Gymnastic Bar##Gundam")) gymbaring();

            if (ImGui::Button("Puru Victory##Gundam")) puru_victory();
            ImGui::SameLine();
            if (ImGui::Button("Kame hane ha##Gundam")) kamehaneha();
        }

        ImGui::End();
    }

    void init() {
        state = State::Idle;

        gumdamAnimator.looping = false;
        gumdamAnimator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-idle"));

        cameraAnimator.looping = false;
        cameraAnimator.LoadJson(Blib::ResourceManager::GetAnimation("camera-idle"));

        kame_particle.loadResources();
    }

    std::function<void()> end;

    void walk() {
        state = State::Walking;

        gumdamAnimator.looping = true;
        gumdamAnimator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-walking"));
        gumdamAnimator.Play([&](){
            gumdamAnimator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-idle"));

            cameraAnimator.LoadJson(Blib::ResourceManager::GetAnimation("camera-walking-to-idle"));
            cameraAnimator.Play();
            state = State::Idle;
        });

        cameraAnimator.looping = false;
        cameraAnimator.LoadJson(Blib::ResourceManager::GetAnimation("camera-idle-to-walking"));
        cameraAnimator.Play();

        end = [&](){
            gumdamAnimator.End();
        };
    }

    void jumping_jack() {
        state = State::JumpingJack;

        gumdamAnimator.looping = true;
        gumdamAnimator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-jumping-jack"));
        gumdamAnimator.Play([&](){
            gumdamAnimator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-idle"));

            cameraAnimator.LoadJson(Blib::ResourceManager::GetAnimation("camera-jumping-jack-to-idle"));
            cameraAnimator.Play();
            state = State::Idle;
        });

        cameraAnimator.looping = false;
        cameraAnimator.LoadJson(Blib::ResourceManager::GetAnimation("camera-idle-to-jumping-jack"));
        cameraAnimator.Play();

        end = [&](){
            gumdamAnimator.End();
        };
    }

    void squat() {
        state = State::Squating;

        gumdamAnimator.looping = true;
        gumdamAnimator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-squating"));
        gumdamAnimator.Play([&](){
            gumdamAnimator.looping = false;
            gumdamAnimator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-idle"));
            state = State::Idle;
        });

        cameraAnimator.looping = false;
        cameraAnimator.LoadJson(Blib::ResourceManager::GetAnimation("camera-idle-to-squating"));
        cameraAnimator.Play([&](){
            cameraAnimator.looping = true;
            cameraAnimator.LoadJson(Blib::ResourceManager::GetAnimation("camera-squating"));
            cameraAnimator.Play([&](){
                gumdamAnimator.looping = false;
                cameraAnimator.LoadJson(Blib::ResourceManager::GetAnimation("camera-idle"));
            });
        });

        end = [&](){
            gumdamAnimator.End();
            cameraAnimator.End();
        };
    }
    
    void gymbaring() {
        state = State::Gymbaring;

        gumdamAnimator.looping = false;
        gumdamAnimator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-gymbar-grabbing"));
        gumdamAnimator.Play([&](){
            gumdamAnimator.looping = true;
            gumdamAnimator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-gymbar-pulling"));
            gumdamAnimator.Play([&](){
                gumdamAnimator.looping = false;
                gumdamAnimator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-gymbar-ungrabbing"));
                gumdamAnimator.Play([&](){
                    gumdamAnimator.looping = false;
                    gumdamAnimator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-idle"));

                    cameraAnimator.looping = false;
                    cameraAnimator.LoadJson(Blib::ResourceManager::GetAnimation("camera-gymbar-to-idle"));
                    cameraAnimator.Play();
                    state = State::Idle;
                });
            });

            cameraAnimator.looping = true;
            cameraAnimator.LoadJson(Blib::ResourceManager::GetAnimation("camera-gymbar"));
            cameraAnimator.Play();
        });

        cameraAnimator.looping = false;
        cameraAnimator.LoadJson(Blib::ResourceManager::GetAnimation("camera-idle-to-gymbar"));
        cameraAnimator.Play();

        end = [&](){
            gumdamAnimator.End();
        };
    }

    void puru_victory() {
        state = State::PuruVictorying;

        gumdamAnimator.looping = false;
        gumdamAnimator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-puru-victory"));
        gumdamAnimator.Play([&](){
            state = State::Idle;
        });

        cameraAnimator.looping = false;
        cameraAnimator.LoadJson(Blib::ResourceManager::GetAnimation("camera-idle-to-puru-victory"));
        cameraAnimator.Play();
    }

    void kamehaneha() {
        state = State::Kemehanehaing;

        gumdamAnimator.looping = false;
        gumdamAnimator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-kamehaneha-1"));
        gumdamAnimator.Play([&]() {
            particle_on = true;
            kame_particle.startTimer();
            gumdamAnimator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-kamehaneha-2"));
            gumdamAnimator.Play([&]() {
                particle_on = false;
                state = State::Idle;
            });
        });

        cameraAnimator.looping = false;
        cameraAnimator.LoadJson(Blib::ResourceManager::GetAnimation("camera-idle-to-kamehaneha"));
        cameraAnimator.Play();
    }

    static void LoadResources() {
        Blib::ResourceManager::LoadShader("gumdam", "shaders/gundam.vert.glsl", "shaders/gundam.frag.glsl");
        Blib::ResourceManager::LoadShader("gumdam-lazer", "shaders/lazer.vert.glsl", "shaders/lazer.frag.glsl");
        Blib::ResourceManager::LoadShader("gumdam-particles", "shaders/particles.vert.glsl", "shaders/particles.frag.glsl");

        Blib::ResourceManager::LoadAnimation("gumdam-idle", "animations/gumdam-idle.json");
        Blib::ResourceManager::LoadAnimation("camera-idle", "animations/camera-idle.json");

        Blib::ResourceManager::LoadAnimation("gumdam-walking", "animations/gumdam-walking.json");
        Blib::ResourceManager::LoadAnimation("camera-idle-to-walking", "animations/camera-idle-to-walking.json");
        Blib::ResourceManager::LoadAnimation("camera-walking-to-idle", "animations/camera-walking-to-idle.json");

        Blib::ResourceManager::LoadAnimation("gumdam-jumping-jack", "animations/gumdam-jumping-jack.json");
        Blib::ResourceManager::LoadAnimation("camera-idle-to-jumping-jack", "animations/camera-idle-to-jumping-jack.json");
        Blib::ResourceManager::LoadAnimation("camera-jumping-jack-to-idle", "animations/camera-jumping-jack-to-idle.json");

        Blib::ResourceManager::LoadAnimation("gumdam-squating", "animations/gumdam-squating.json");
        Blib::ResourceManager::LoadAnimation("camera-idle-to-squating", "animations/camera-idle-to-squating.json");
        Blib::ResourceManager::LoadAnimation("camera-squating", "animations/camera-squating.json");

        Gymbar::LoadResources();
        Blib::ResourceManager::LoadAnimation("gumdam-gymbar-grabbing", "animations/gumdam-gymbar-grabbing.json");
        Blib::ResourceManager::LoadAnimation("gumdam-gymbar-pulling", "animations/gumdam-gymbar-pulling.json");
        Blib::ResourceManager::LoadAnimation("gumdam-gymbar-ungrabbing", "animations/gumdam-gymbar-ungrabbing.json");
        Blib::ResourceManager::LoadAnimation("camera-idle-to-gymbar", "animations/camera-idle-to-gymbar.json");
        Blib::ResourceManager::LoadAnimation("camera-gymbar", "animations/camera-gymbar.json");
        Blib::ResourceManager::LoadAnimation("camera-gymbar-to-idle", "animations/camera-gymbar-to-idle.json");

        Blib::ResourceManager::LoadAnimation("gumdam-puru-victory", "animations/gumdam-puru-victory.json");
        Blib::ResourceManager::LoadAnimation("camera-idle-to-puru-victory", "animations/camera-idle-to-puru-victory.json");

        Blib::ResourceManager::LoadAnimation("gumdam-kamehaneha-1", "animations/gumdam-kamehaneha-1.json");
        Blib::ResourceManager::LoadAnimation("gumdam-kamehaneha-2", "animations/gumdam-kamehaneha-2.json");
        Blib::ResourceManager::LoadAnimation("camera-idle-to-kamehaneha", "animations/camera-idle-to-kamehaneha.json");

        Blib::ResourceManager::LoadModel("gumdam-back", "models/gundam/back.obj");
        Blib::ResourceManager::LoadModel("gumdam-body", "models/gundam/body.obj");
        Blib::ResourceManager::LoadModel("gumdam-dbody", "models/gundam/dbody.obj");
        Blib::ResourceManager::LoadModel("gumdam-dlefthand", "models/gundam/dlefthand.obj");
        Blib::ResourceManager::LoadModel("gumdam-dleftleg", "models/gundam/dleftleg.obj");
        Blib::ResourceManager::LoadModel("gumdam-drighthand", "models/gundam/drighthand.obj");
        Blib::ResourceManager::LoadModel("gumdam-drightleg", "models/gundam/drightleg.obj");
        Blib::ResourceManager::LoadModel("gumdam-head", "models/gundam/head.obj");
        Blib::ResourceManager::LoadModel("gumdam-leftfoot", "models/gundam/leftfoot.obj");
        Blib::ResourceManager::LoadModel("gumdam-lefthand", "models/gundam/lefthand.obj");
        Blib::ResourceManager::LoadModel("gumdam-lshouder", "models/gundam/lshouder.obj");
        Blib::ResourceManager::LoadModel("gumdam-rightfoot", "models/gundam/rightfoot.obj");
        Blib::ResourceManager::LoadModel("gumdam-righthand", "models/gundam/righthand.obj");
        Blib::ResourceManager::LoadModel("gumdam-rshouder", "models/gundam/rshouder.obj");
        Blib::ResourceManager::LoadModel("gumdam-ulefthand", "models/gundam/ulefthand.obj");
        Blib::ResourceManager::LoadModel("gumdam-uleftleg", "models/gundam/uleftleg.obj");
        Blib::ResourceManager::LoadModel("gumdam-urighthand", "models/gundam/urighthand.obj");
        Blib::ResourceManager::LoadModel("gumdam-urightleg", "models/gundam/urightleg.obj");

        Blib::ResourceManager::LoadModel("gumdam-lazer", "models/misc/cylinder.obj");
        Blib::ResourceManager::LoadModel("gumdam-fireball", "models/fireball/fireball.obj");
    }
};
