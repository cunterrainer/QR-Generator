#include "Log.h"

#include "GLFW/glfw3.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_stdlib.h"

#include "Core.h"
#include "Icon.h"
#include "Arial.h"
#include "NotoSans.h"
#include "RenderWindow.h"

bool g_WindowResized = false;

RenderWindow::RenderWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) noexcept
{
    glfwSetErrorCallback([]([[maybe_unused]] int error, [[maybe_unused]] const char* description) { Err("[GLFW] %d: %s", error, description); });

    if (!glfwInit())
    {
        Err("Failed to initialize GLFW");
        return;
    }
    Log("Initialized GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window with graphics context
    m_Window = glfwCreateWindow(width, height, title, monitor, share);
    if (m_Window == NULL)
    {
        Err("Failed to create window w: %d h: %d t: %s", width, height, title);
        return;
    }
    Log("Created window w: %d h: %d t: %s", width, height, title);

    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow*, int w, int h) { glViewport(0, 0, w, h); g_WindowResized = true; });

    // center the window
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(m_Window, (mode->width - width) / 2, (mode->height - height) / 2);

    #ifdef SYSTEM_WINDOWS // macos and linux usually don't have icons
        // set window icon
        GLFWimage icon_s;
        icon_s.pixels = sg_QrcodescanData;
        icon_s.width = sg_QrcodescanWidth;
        icon_s.height = sg_QrcodescanHeight;
        glfwSetWindowIcon(m_Window, 1, &icon_s);
    #endif

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(0);

    glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
    if (!ImGuiInit()) return;
    ImGuiSetTheme();
    g_WindowResized = true;
}


RenderWindow::~RenderWindow() noexcept
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    Log("Shut down ImGui");

    glfwDestroyWindow(m_Window);
    glfwTerminate();
    Log("Shut down GLFW");
}


bool RenderWindow::ImGuiInit(const char* iniFileName) const noexcept
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    {
        static const ImWchar ranges[] =
        {
            0x0020, 0xFFFF,
            0,
        };
        io.Fonts->AddFontFromMemoryCompressedTTF(sg_ArialCompressedData, sg_ArialCompressedSize, 19, nullptr, ranges);
    }

    //io.Fonts->AddFontFromMemoryCompressedTTF(NotoSans_compressed_data, NotoSans_compressed_size, 19, nullptr, io.Fonts->GetGlyphRangesJapanese());
    io.IniFilename = iniFileName;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    io.ConfigViewportsNoDecoration = false;
    io.ConfigViewportsNoAutoMerge = true;

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    #ifdef SYSTEM_MACOS
        if (!ImGui_ImplOpenGL3_Init("#version 330 core"))
    #else
        if (!ImGui_ImplOpenGL3_Init("#version 130"))
    #endif
    {
        Err("Failed to initialize ImGui OpenGL3 implementation");
        return false;
    }
    Log("Initialized ImGui");
    return true;
}


void RenderWindow::ImGuiStartFrame() const noexcept
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


void RenderWindow::ImGuiRender() const noexcept
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}


void RenderWindow::ImGuiSetTheme() const noexcept
{
    /*
        Styles from this project:
        https://github.com/MohitSethi99/ArcGameEngine/blob/main/Arc-Editor/src/Utils/EditorTheme.h
    */
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.275f, 0.275f, 0.275f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.44f, 0.88f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.000f, 0.439f, 0.878f, 0.824f);
    colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.188f, 0.529f, 0.780f, 1.000f);


    ImGuiStyle* style = &ImGui::GetStyle();
    style->AntiAliasedFill = true;
    style->AntiAliasedLines = true;
    style->AntiAliasedLinesUseTex = true;

    style->Alpha = 1;
    style->TabMinWidthForCloseButton = 0.1f;
    style->CellPadding = ImVec2(8.0f, 4.0f);
    style->ItemSpacing = ImVec2(8.0f, 3.0f);
    style->ItemInnerSpacing = ImVec2(2.0f, 4.0f);
    style->TouchExtraPadding = ImVec2(0.0f, 0.0f);
    style->IndentSpacing = 12;
    style->ScrollbarSize = 14;
    style->GrabMinSize = 10;

    style->WindowBorderSize = 0.0f;
    style->ChildBorderSize = 0.0f;
    style->PopupBorderSize = 1.5f;
    style->FrameBorderSize = 0.5f;
    style->TabBorderSize = 0.0f;

    style->WindowRounding = 0.0f;
    style->ChildRounding = 0.0f;
    style->FrameRounding = 2.0f;
    style->PopupRounding = 2.0f;
    style->ScrollbarRounding = 3.0f;
    style->GrabRounding = 2.0f;
    style->LogSliderDeadzone = 4.0f;
    style->TabRounding = 3.0f;

    style->WindowTitleAlign = ImVec2(0.0f, 0.5f);
    style->ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style->SelectableTextAlign = ImVec2(0.0f, 0.0f);
    style->DisplaySafeAreaPadding = ImVec2(8.0f, 8.0f);
}


void RenderWindow::Show() noexcept
{
    ImGuiStartFrame();
    //const float width = Size().x;
    //
    //ImGui::SetNextWindowPos({ 0, 0 });
    //ImGui::SetNextWindowSize({ width, SettingsHeight });
    //ImGui::Begin("##Input", nullptr, IMGUI_WINDOW_FLAGS);
    //static std::string s;
    //s.shrink_to_fit();
    //ImGui::InputTextWithHint("sa", "test", &s);
    //ImGui::SameLine();
    //static bool render = false;
    //if (ImGui::Button("Hello"))
    //{
    //    render = true;
    //}
    //ImGui::End();

    ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
    ImGui::Begin("askl");
    ImGui::End();
}