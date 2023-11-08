#include <iostream>
#include <algorithm>

#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "qrcodegen/cpp/qrcodegen.hpp"

#include "Log.h"
#include "Clang.h"
#include "Image.h"
#include "RenderWindow.h"


int main()
{
    Image img;
    RenderWindow window;
    
    while (window.IsOpen())
    {
        window.StartFrame();
        ImVec2 newPos = window.Pos();
        const float windowWidth = window.Size().x;
        const float windowHeight = window.Size().y;

        {
            ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
            ImGui::SetNextWindowSize({ windowWidth/2, windowHeight });
            ImGui::SetNextWindowPos(newPos);

            ImGui::Begin("InputWindow", nullptr, IMGUI_WINDOW_FLAGS);
            static bool recreate = true;

            static std::string s;
            recreate = ImGui::InputTextWithHint("##ContentInputText", (const char*)u8"Text einfügen", &s) || recreate;

            static int eccLevel = 0;
            recreate = ImGui::Combo("Fehlerkorrektur", &eccLevel, "Niedrig\0Mittel\0Quartil\0Hoch\0") || recreate;

            static int borderSize = 3;
            recreate = ImGui::InputInt("Rand", &borderSize, 1, 10, ImGuiInputTextFlags_CharsDecimal) || recreate;
            borderSize = std::clamp(borderSize, 0, 100);

            static int scale = 30;
            recreate = ImGui::InputInt("Skalierung", &scale, 1, 10, ImGuiInputTextFlags_CharsDecimal) || recreate;
            scale = std::clamp(scale, 1, 300);

            static float colorsDark[3] = { 0 };
            recreate = ImGui::ColorEdit3((const char*)u8"Primärfarbe", colorsDark, ImGuiColorEditFlags_DisplayHex) || recreate;

            static float colorsLight[3] = { 1, 1, 1 };
            recreate = ImGui::ColorEdit3((const char*)u8"Sekundärfarbe", colorsLight, ImGuiColorEditFlags_DisplayHex) || recreate;

            if (recreate)
            {
                qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(s.c_str(), (qrcodegen::QrCode::Ecc)eccLevel);
                img.Assign(qr, borderSize, scale);
                recreate = false;
            }

            newPos.x += ImGui::GetWindowWidth();
            ImGui::End();
        }

        {
            ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
            ImGui::SetNextWindowSize({ windowWidth / 2, windowHeight });
            ImGui::SetNextWindowPos(newPos);
            ImGui::Begin("QRCodeImage", nullptr, IMGUI_WINDOW_FLAGS);
        
            const float imgSize = std::min(ImGui::GetWindowHeight(), ImGui::GetWindowWidth()) - 2*std::max(ImGui::GetStyle().WindowPadding.x, ImGui::GetStyle().WindowPadding.y);
            const float xPos = (ImGui::GetWindowWidth() - imgSize) / 2.f;
            const float yPos = (ImGui::GetWindowHeight() - imgSize) / 2.f;
            const ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::SetCursorScreenPos(ImVec2(p.x + xPos, p.y + yPos));
            ImGui::Image((void*)(intptr_t)img.GetGpuImage(), { imgSize, imgSize });
            ImGui::End();
        }

        window.EndFrame();
    }
    return 0;
}