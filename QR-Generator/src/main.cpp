#include <new>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "qrcodegen/cpp/qrcodegen.hpp"
#include "clip/clip.h"

#include "Log.h"
#include "Clang.h"
#include "Image.h"
#include "ImageWriter.h"
#include "RenderWindow.h"


int main()
{
    Image img;
    RenderWindow window(1130, 740, "QR-Code-Generator");
    float yPosCursor = ImGui::GetStyle().WindowPadding.y;
    
    while (window.IsOpen())
    {
        window.StartFrame();
        ImVec2 newPos = window.Pos();
        const float windowWidth = window.Size().x;
        const float windowHeight = window.Size().y;

        if (g_WindowResized)
        {
            yPosCursor = ImGui::GetStyle().WindowPadding.y;
            g_WindowResized = false;
        }

        {
            ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
            ImGui::SetNextWindowSize({ windowWidth/2, windowHeight });
            ImGui::SetNextWindowPos(newPos);

            ImGui::Begin("InputWindow", nullptr, IMGUI_WINDOW_FLAGS);
            static bool rerender = true;
            static bool qrContentChanged = true;

            static std::string s;
            ImGui::SetCursorPosY(yPosCursor);
            qrContentChanged = ImGui::InputTextWithHint("##ContentInputText", (const char*)u8"Text einfügen", &s) || qrContentChanged;

            static int eccLevel = 0;
            qrContentChanged = ImGui::Combo("Fehlerkorrektur", &eccLevel, "Niedrig\0Mittel\0Quartil\0Hoch\0") || qrContentChanged;

            static bool boostEccl = true;
            qrContentChanged = ImGui::Checkbox((const char*)u8"Erhöhe das Fehlerkorrekturlevel automatisch", &boostEccl) || qrContentChanged;
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip((const char*)u8"Es wird automatisch die kleinstmögliche QR-Code-Version innerhalb des angegebenen Bereichs verwendet.\nWenn aktiviert wird das Fehlerkorrekturlevel erhöht sofern dies ohne Erhöhung der Version möglich ist");

            static int borderSize = 3;
            rerender = ImGui::InputInt("Rand", &borderSize, 1, 10, ImGuiInputTextFlags_CharsDecimal) || rerender;
            borderSize = std::clamp(borderSize, 0, 30);

            static int scale = 30;
            rerender = ImGui::InputInt("Skalierung", &scale, 1, 10, ImGuiInputTextFlags_CharsDecimal) || rerender;
            scale = std::clamp(scale, 1, 300);

            ImGui::SameLine();
            ImGui::Text("(%ux%u)", img.Width(), img.Height());

            static float colorPrimary[3] = { 0 };
            rerender = ImGui::ColorEdit3((const char*)u8"Primärfarbe", colorPrimary, ImGuiColorEditFlags_DisplayHex) || rerender;

            static float colorSecondary[3] = { 1, 1, 1 };
            rerender = ImGui::ColorEdit3((const char*)u8"Sekundärfarbe", colorSecondary, ImGuiColorEditFlags_DisplayHex) || rerender;

            static int minVersion = 1;
            static int maxVersion = 40;
            qrContentChanged = ImGui::InputInt("Mindest Version", &minVersion, 1, 10, ImGuiInputTextFlags_CharsDecimal) || qrContentChanged;
            qrContentChanged = ImGui::InputInt("Maximal Version", &maxVersion, 1, 10, ImGuiInputTextFlags_CharsDecimal) || qrContentChanged;
            minVersion = std::clamp(minVersion, 1, std::max(maxVersion, 1));
            maxVersion = std::clamp(maxVersion, minVersion, 40);

            static int maskPattern = -1;
            qrContentChanged = ImGui::InputInt("Maske", &maskPattern, 1, 10, ImGuiInputTextFlags_CharsDecimal) || qrContentChanged;
            maskPattern = std::clamp(maskPattern, -1, 7);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("(-1 automatisch, 0 bis 7 manuell)");

            if (rerender || qrContentChanged)
            {
                static qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(s.c_str(), (qrcodegen::QrCode::Ecc)eccLevel);
                if (qrContentChanged)
                {
                    try
                    {
                        const std::vector<qrcodegen::QrSegment> qrSegments = qrcodegen::QrSegment::makeSegments(s.c_str());
                        qr = qrcodegen::QrCode::encodeSegments(qrSegments, (qrcodegen::QrCode::Ecc)eccLevel, minVersion, maxVersion, maskPattern, boostEccl);
                    }
                    catch (const std::length_error& e)
                    {
                        Err("%s\nQR-Code konnte nicht generiert werden da er zu groß ist.\nZum Beheben:\n- Verringere das Fehlerkorrekturlevel\n- Erhöhe das Maximale Level\n- Kürze die Texteingabe", e.what());
                    }
                }
                img.Assign(qr, borderSize, scale, colorPrimary, colorSecondary);
                rerender = false;
                qrContentChanged = false;
            }

            newPos.x += ImGui::GetWindowWidth();
            if (yPosCursor == ImGui::GetStyle().WindowPadding.y)
                yPosCursor = (windowHeight / 2 - ImGui::GetCursorPosY() / 2);
            ImGui::End();
        }

        {
            ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
            ImGui::SetNextWindowSize({ windowWidth / 2, windowHeight });
            ImGui::SetNextWindowPos(newPos);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
            ImGui::Begin("QRCodeImage", nullptr, IMGUI_WINDOW_FLAGS);
        
            const float imgSize = std::min(ImGui::GetWindowHeight(), ImGui::GetWindowWidth()) * 0.65f;
            const float xPos = (ImGui::GetWindowWidth() - imgSize) / 2.f;
            const float yPos = (ImGui::GetWindowHeight() - imgSize) / 2.f;
            ImGui::SetCursorPos(ImVec2(xPos, yPos));
            if (ImGui::ImageButton((void*)(intptr_t)img.GetGpuImage(), { imgSize, imgSize }))
            {
                clip::image_spec spec;
                spec.width = img.Width();
                spec.height = img.Height();
                spec.bits_per_pixel = 8 * 4;
                spec.bytes_per_row = spec.width * 4;
                spec.red_mask = 0xff;
                spec.green_mask = 0xff00;
                spec.blue_mask = 0xff0000;
                spec.alpha_mask = 0xff000000;
                spec.red_shift = 0;
                spec.green_shift = 8;
                spec.blue_shift = 16;
                spec.alpha_shift = 24;
                try { clip::image cimg(img.Data32().data(), spec);  clip::set_image(cimg); }
                catch (const std::bad_alloc& e)  { Log("Failed to create uint32 image vector (%s)", e.what()); }
                catch (const std::runtime_error& e) { Log("Failed to copy image to clipboard (%s)", e.what()); }
            }
            ImGui::SetCursorPosX(xPos);
            if (ImGui::Button("Speichern", ImVec2(ImGui::GetItemRectSize().x, 28))) SaveImage(img);
            ImGui::End();
            ImGui::PopStyleVar();
        }

        window.EndFrame();
    }
    return 0;
}