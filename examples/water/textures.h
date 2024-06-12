#pragma once

#include "imgui.h"
#include "utils/texture.h"
#include <map>
#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

class Textures {
public:
    std::map<std::string, Texture> store;
    std::string selected_texture = "_grid.jpg";

    void loadAll() {
        for (const auto& entry : fs::directory_iterator("textures")) {
            if (fs::is_regular_file(entry.path())) {
                std::cout << "path" << entry.path() << std::endl;
                std::cout << "filename" << entry.path().filename() << std::endl;
                store[entry.path().filename().string()] = Texture::Load(entry.path().string());
            } else if (fs::is_directory(entry.path())) {
                std::cout << entry.path() << "\n";
            }
        }
    }

    void bindSelected(Shader prog) {
        store.at(selected_texture).Bind(prog);
    }

    void bind(std::string path, Shader prog) {
        store.at(path).Bind(prog);
    }

    bool renderPicker(int col = 0) {
        bool picked = false;

        ImGuiStyle& style = ImGui::GetStyle();
        int buttons_count = 20;
        float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
        int n = 0;
        int sz = 150;
        for (auto [name, texture] : store) {
            ImGui::PushID(n);

            if (ImGui::ImageButton((ImTextureID)texture.diffuse, {(float)sz, (float)sz}, {0, 1}, {1, 0})) {
                selected_texture = name;
                ImGui::CloseCurrentPopup();
                picked = true;
            }

            if (col) {
                if (n % col != col - 1) {
                    ImGui::SameLine();
                }
            } else {
                float last_button_x2 = ImGui::GetItemRectMax().x;
                float next_button_x2 = last_button_x2 + style.ItemSpacing.x + sz; // Expected position if next button was on same line
                if (n + 1 < buttons_count && next_button_x2 < window_visible_x2) {
                    ImGui::SameLine();
                }
            }

            ImGui::PopID();
            n += 1;
        }

        return picked;
    }
};

extern Textures textures;
