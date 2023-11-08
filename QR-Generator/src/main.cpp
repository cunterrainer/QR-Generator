#include <iostream>

#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "qrcodegen/cpp/qrcodegen.hpp"

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
        float previousHeight = 0;

        {
            ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
            ImGui::SetNextWindowSize({ windowWidth, -1 });
            ImGui::SetNextWindowPos(newPos);

            ImGui::Begin("InputWindow", nullptr, IMGUI_WINDOW_FLAGS);
            static std::string s;
            if (ImGui::InputTextWithHint("##ContentInputText", (const char*)u8"Text einfügen", &s))
            {
                int border = 2;
                qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(s.c_str(), qrcodegen::QrCode::Ecc::LOW);
                img.Assign(qr, border, (size_t)(windowHeight - ImGui::GetWindowHeight()) / (qr.getSize() + border*2));
            }

            newPos.y += ImGui::GetWindowHeight();
            previousHeight = ImGui::GetWindowHeight();
            ImGui::End();
        }

        {
            ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
            ImGui::SetNextWindowSize({ windowWidth, windowHeight - previousHeight });
            ImGui::SetNextWindowPos(newPos);
            ImGui::Begin("QRCodeImage", nullptr, IMGUI_WINDOW_FLAGS);

            const float imgSize = std::min(ImGui::GetWindowHeight(), ImGui::GetWindowWidth()) - 2*ImGui::GetStyle().WindowPadding.y;
            const float xPos = (windowWidth - imgSize) / 2.f;
            const ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::SetCursorScreenPos(ImVec2(p.x + xPos, p.y));
            ImGui::Image((void*)(intptr_t)img.GetGpuImage(), { imgSize, imgSize });
            ImGui::End();
        }

        window.EndFrame();
    }
    return 0;
}