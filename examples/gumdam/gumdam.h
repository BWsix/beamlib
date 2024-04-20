#pragma once

#include "gymbar.h"
#include <beamlib.h>

class Gundam {
    Blib::Instance body{"body", Blib::ResourceManager::GetModel("gumdam-body")};
        Blib::Instance head{"head", Blib::ResourceManager::GetModel("gumdam-head")};
        Blib::Instance beam{"beam", Blib::ResourceManager::GetModel("gumdam-beam")};
        Blib::Instance ball{"ball", Blib::ResourceManager::GetModel("gumdam-ball")};
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

    Gymbar gymbar;

public:
    enum class State {
        Idle, Walking, JumpingJack, Squating, GymbarGrabbing, GymbarPulling, PuruVictorying, Kemehanehaing,
    };

    Blib::Instance root{"gumdam-root"};
    Blib::Animator animator{"gundam animator", &body};
    State state = State::Idle;

    Gundam() {
        root.PushChild(&body);

        body.PushChild(&head);
        body.PushChild(&beam);
        body.PushChild(&ball);
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
    }

    void render() {
        auto prog = Blib::ResourceManager::GetShader("gumdam");
        prog.Use();
        prog.SetMat4("view", Blib::camera.getViewMatrix());
        prog.SetMat4("projection", Blib::camera.getProjectionMatrix());

        animator.Update();
        root.Render(prog);

        if (state == State::GymbarGrabbing || state == State::GymbarPulling) {
            gymbar.render();
        }
    }

    void renderUI() {
        ImGui::Begin("Gundam Animation Editor");
        animator.renderUI();
        ImGui::End();
        root.RenderUI();

        if (state == State::GymbarGrabbing || state == State::GymbarPulling) {
            gymbar.renderUI();
        }
    }

    void renderControlPanel() {
        ImGui::Begin("Gundam Control");

        if (ImGui::Button("Idle##Gundam")) idle();
        ImGui::SameLine();
        if (ImGui::Button("Walk##Gundam")) walk();
        ImGui::SameLine();
        if (ImGui::Button("Jumping Jack##Gundam")) jumping_jack();
        ImGui::SameLine();
        if (ImGui::Button("Squat##Gundam")) squat();

        if (ImGui::Button("Gymnastic Bar##Gundam")) gymbar_grabbing();
        ImGui::SameLine();
        if (ImGui::Button("Puru Victory##Gundam")) puru_victory();
        ImGui::SameLine();
        if (ImGui::Button("Kame hane ha##Gundam")) kamehaneha();

        ImGui::End();
    }

    void idle() {
        if (animator.playing) animator.End();
        animator.looping = false;
        animator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-idle"));
        state = State::Idle;
    }

    void walk() {
        if (animator.playing) animator.End();
        animator.looping = true;
        animator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-walking"));
        animator.Play();
        state = State::Walking;
    }

    void jumping_jack() {
        if (animator.playing) animator.End();
        animator.looping = true;
        animator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-jumping-jack"));
        animator.Play();
        state = State::JumpingJack;
    }

    void squat() {
        if (animator.playing) animator.End();
        animator.looping = true;
        animator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-squating"));
        animator.Play();
        state = State::Squating;
    }
    
    void gymbar_grabbing() {
        if (animator.playing) animator.End();
        animator.looping = false;
        animator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-gymbar-grabbing"));
        animator.Play([&](){gymbar_pulling();});
        state = State::GymbarGrabbing;
    }

    void gymbar_pulling() {
        animator.looping = true;
        animator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-gymbar-pulling"));
        animator.Play();
        state = State::GymbarPulling;
    }

    void puru_victory() {
        if (animator.playing) animator.End();
        animator.looping = false;
        animator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-puru-victory"));
        animator.Play();
        state = State::PuruVictorying;
    }

    void kamehaneha() {
        if (animator.playing) animator.End();
        animator.looping = false;
        animator.LoadJson(Blib::ResourceManager::GetAnimation("gumdam-kamehaneha"));
        animator.Play();
        state = State::Kemehanehaing;
    }

    static void LoadResources() {
        Gymbar::LoadResources();

        Blib::ResourceManager::LoadShader("gumdam", "shaders/gundam.vert.glsl", "shaders/gundam.frag.glsl");

        Blib::ResourceManager::LoadAnimation("gumdam-idle", "animations/idle.json");
        Blib::ResourceManager::LoadAnimation("gumdam-walking", "animations/walking-fast.json");
        Blib::ResourceManager::LoadAnimation("gumdam-jumping-jack", "animations/jumping-jack.json");
        Blib::ResourceManager::LoadAnimation("gumdam-squating", "animations/squating.json");
        Blib::ResourceManager::LoadAnimation("gumdam-gymbar-grabbing", "animations/gymbar-grabbing.json");
        Blib::ResourceManager::LoadAnimation("gumdam-gymbar-pulling", "animations/gymbar-pulling.json");
        Blib::ResourceManager::LoadAnimation("gumdam-puru-victory", "animations/puru-victory.json");
        Blib::ResourceManager::LoadAnimation("gumdam-kamehaneha", "animations/kamehaneha.json");

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

        Blib::ResourceManager::LoadModel("gumdam-beam", "models/gundam/beam.obj");
        Blib::ResourceManager::LoadModel("gumdam-ball", "models/gundam/ball.obj");
    }
};
