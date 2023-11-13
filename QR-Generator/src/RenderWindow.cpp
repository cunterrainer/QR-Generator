#include "GLFW/glfw3.h"
#include "stb/stb_image.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_stdlib.h"

#include "Log.h"
#include "Core.h"
#include "Icon.h"
#include "Noto.h"
#include "RenderWindow.h"

#ifdef SYSTEM_WINDOWS
    #undef APIENTRY
    #include <dwmapi.h>
    #include <Windows.h>
    #define GLFW_EXPOSE_NATIVE_WIN32
    #define GLFW_NATIVE_INCLUDE_NONE
    #include "GLFW/glfw3native.h"
#endif

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

    #ifdef SYSTEM_WINDOWS // macos and linux usually don't have icons, and are already styled according to the platforms theme
        GLFWimage icon_s;
        icon_s.pixels = stbi_load_from_memory(sg_RawIcon_whiteData, sg_RawIcon_whiteDataRelativeSize, &icon_s.width, &icon_s.height, NULL, sg_Icon_whiteChannels);
        if (icon_s.pixels != NULL)
        {
            glfwSetWindowIcon(m_Window, 1, &icon_s);
            stbi_image_free(icon_s.pixels);
        }

        // set dark mode (Title bar etc.)
        const HWND hwnd = glfwGetWin32Window(m_Window);
        constexpr DWORD dwmwaUseImmersiveDarkMode = 20;
        constexpr DWORD dwmwaUseImmersiveDarkModeBefore20H1 = 19;
        const wchar_t* const darkModeStringName = L"DarkMode_Explorer";

        if (SetWindowTheme(hwnd, darkModeStringName, nullptr) != S_OK)
            return;

        constexpr BOOL isDarkMode = true;
        if (DwmSetWindowAttribute(hwnd, dwmwaUseImmersiveDarkMode, &isDarkMode, sizeof(isDarkMode)) != S_OK)
        {
            DwmSetWindowAttribute(hwnd, dwmwaUseImmersiveDarkModeBefore20H1, &isDarkMode, sizeof(isDarkMode));
        }
        // a neat workaround to get windows to redraw the title bar without anyone actually being able to notice any change in the window
        glfwSetWindowOpacity(m_Window, 0.99f);
        glfwSetWindowOpacity(m_Window, 1.f);
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


void RenderWindow::ImGuiLoadFonts() const noexcept
{
    //static const ImWchar ranges[] =
    //{
    //    0x0000, 0x007F, // Basic Latin
    //    0x0080, 0x00FF, // C1 Controls and Latin-1 Supplement
    //    0x0100, 0x017F, // Latin Extended-A
    //    0x0180, 0x024F, // Latin Extended-B
    //    0x0250, 0x02AF, // IPA Extensions
    //    0x02B0, 0x02FF, // Spacing Modifier Letters
    //    0x0300, 0x036F, // Combining Diacritical Marks
    //    0x0370, 0x03FF, // Greek/Coptic
    //    0x0400, 0x04FF, // Cyrillic
    //    0x0500, 0x052F, // Cyrillic Supplement
    //    0x0530, 0x058F, // Armenian
    //    0x0590, 0x05FF, // Hebrew
    //    0x0600, 0x06FF, // Arabic
    //    0x0700, 0x074F, // Syriac
    //    0x0780, 0x07BF, // Thaana
    //    0x0A00, 0x0A7F, // Gurmukhi
    //    0x0A80, 0x0AFF, // Gujarati
    //    0x0B00, 0x0B7F, // Oriya
    //    0x0B80, 0x0BFF, // Tamil
    //    0x0C00, 0x0C7F, // Telugu
    //    0x0C80, 0x0CFF, // Kannada
    //    0x0D00, 0x0D7F, // Malayalam
    //    0x0D80, 0x0DFF, // Sinhala
    //    0x0E00, 0x0E7F, // Thai
    //    0x0E80, 0x0EFF, // Lao
    //    0x0F00, 0x0FFF, // Tibetan
    //    0x1000, 0x109F, // Myanmar
    //    0x10A0, 0x10FF, // Georgian
    //    0x1100, 0x11FF, // Hangul Jamo
    //    0x1200, 0x137F, // Ethiopic
    //    0x13A0, 0x13FF, // Cherokee
    //    0x1400, 0x167F, // Unified Canadian Aboriginal Syllabics
    //    0x1680, 0x169F, // Ogham
    //    0x16A0, 0x16FF, // Runic
    //    0x1700, 0x171F, // Tagalog
    //    0x1720, 0x173F, // Hanunoo
    //    0x1740, 0x175F, // Buhid
    //    0x1760, 0x177F, // Tagbanwa
    //    0x1780, 0x17FF, // Khmer
    //    0x1800, 0x18AF, // Mongolian
    //    0x1900, 0x194F, // Limbu
    //    0x1950, 0x197F, // Tai Le
    //    0x19E0, 0x19FF, // Khmer Symbols
    //    0x1D00, 0x1D7F, // Phonetic Extensions
    //    0x1E00, 0x1EFF, // Latin Extended Additional
    //    0x1F00, 0x1FFF, // Greek Extended
    //    0x2000, 0x206F, // General Punctuation
    //    0x2070, 0x209F, // Superscripts and Subscripts
    //    0x20A0, 0x20CF, // Currency Symbols
    //    0x20D0, 0x20FF, // Combining Diacritical Marks for Symbols
    //    0x2100, 0x214F, // Letterlike Symbols
    //    0x2150, 0x218F, // Number Forms
    //    0x2190, 0x21FF, // Arrows
    //    0x2200, 0x22FF, // Mathematical Operators
    //    0x2300, 0x23FF, // Miscellaneous Technical
    //    0x2400, 0x243F, // Control Pictures
    //    0x2440, 0x245F, // Optical Character Recognition
    //    0x2460, 0x24FF, // Enclosed Alphanumerics
    //    0x2500, 0x257F, // Box Drawing
    //    0x2580, 0x259F, // Block Elements
    //    0x25A0, 0x25FF, // Geometric Shapes
    //    0x2600, 0x26FF, // Miscellaneous Symbols
    //    0x2700, 0x27BF, // Dingbats
    //    0x27C0, 0x27EF, // Miscellaneous Mathematical Symbols-A
    //    0x27F0, 0x27FF, // Supplemental Arrows-A
    //    0x2800, 0x28FF, // Braille Patterns
    //    0x2900, 0x297F, // Supplemental Arrows-B
    //    0x2980, 0x29FF, // Miscellaneous Mathematical Symbols-B
    //    0x2A00, 0x2AFF, // Supplemental Mathematical Operators
    //    0x2B00, 0x2BFF, // Miscellaneous Symbols and Arrows
    //    0x2E80, 0x2EFF, // CJK Radicals Supplement
    //    0x2F00, 0x2FDF, // Kangxi Radicals
    //    0x2FF0, 0x2FFF, // Ideographic Description Characters
    //    0x3000, 0x303F, // CJK Symbols and Punctuation
    //    0x3040, 0x309F, // Hiragana
    //    0x30A0, 0x30FF, // Katakana
    //    0x3100, 0x312F, // Bopomofo
    //    0x3130, 0x318F, // Hangul Compatibility Jamo
    //    0x3190, 0x319F, // Kanbun (Kunten)
    //    0x31A0, 0x31BF, // Bopomofo Extended
    //    0x31F0, 0x31FF, // Katakana Phonetic Extensions
    //    0x3200, 0x32FF, // Enclosed CJK Letters and Months
    //    0x3300, 0x33FF, // CJK Compatibility
    //    0x3400, 0x4DBF, // CJK Unified Ideographs Extension A
    //    0x4DC0, 0x4DFF, // Yijing Hexagram Symbols
    //    0x4E00, 0x9FAF, // CJK Unified Ideographs
    //    0xA000, 0xA48F, // Yi Syllables
    //    0xA490, 0xA4CF, // Yi Radicals
    //    0xAC00, 0xD7AF, // Hangul Syllables
    //    0xD800, 0xDBFF, // High Surrogate Area
    //    0xDC00, 0xDFFF, // Low Surrogate Area
    //    0xE000, 0xF8FF, // Private Use Area
    //    0xF900, 0xFAFF, // CJK Compatibility Ideographs
    //    0xFB00, 0xFB4F, // Alphabetic Presentation Forms
    //    0xFB50, 0xFDFF, // Arabic Presentation Forms-A
    //    0xFE00, 0xFE0F, // Variation Selectors
    //    0xFE20, 0xFE2F, // Combining Half Marks
    //    0xFE30, 0xFE4F, // CJK Compatibility Forms
    //    0xFE50, 0xFE6F, // Small Form Variants
    //    0xFE70, 0xFEFF, // Arabic Presentation Forms-B
    //    0xFF00, 0xFFEF, // Halfwidth and Fullwidth Forms
    //    0xFFF0, 0xFFFF, // Specials
    //    0
    //};


    //static const ImWchar rangesDefault[] =
    //{
    //    0x0020, 0x007F, // Basic Latin
    //    0x0080, 0x00FF, // C1 Controls and Latin-1 Supplement
    //    0x0100, 0x017F, // Latin Extended-A
    //    0x0180, 0x024F, // Latin Extended-B
    //    0x1E00, 0x1EFF, // Latin Extended Additional
    //    0x1D00, 0x1D7F, // Phonetic Extensions
    //    0x1F00, 0x1FFF, // Greek Extended
    //    0x0400, 0x04FF, // Cyrillic
    //    0x0500, 0x052F, // Cyrillic Supplement
    //    0x0370, 0x03FF, // Greek/Coptic
    //    0x2500, 0x257F, // Box Drawing
    //    0x2300, 0x23FF, // Miscellaneous Technical
    //    0x0300, 0x036F, // Combining Diacritical Marks
    //    0x2200, 0x22FF, // Mathematical Operators
    //    0x0250, 0x02AF, // IPA Extensions
    //    0x2000, 0x206F, // General Punctuation
    //    0x25A0, 0x25FF, // Geometric Shapes
    //    0x02B0, 0x02FF, // Spacing Modifier Letters
    //    0x20A0, 0x20CF, // Currency Symbols
    //    0x2100, 0x214F, // Letterlike Symbols
    //    0x2580, 0x259F, // Block Elements
    //    0x20D0, 0x20FF, // Combining Diacritical Marks for Symbols
    //    0x2070, 0x209F, // Superscripts and Subscripts
    //    0x2190, 0x21FF, // Arrows
    //    0xFE20, 0xFE2F, // Combining Half Marks
    //    0x27C0, 0x27EF, // Miscellaneous Mathematical Symbols-A
    //    0x2980, 0x29FF, // Miscellaneous Mathematical Symbols-B
    //    0x2700, 0x27BF, // Dingbats
    //
    //    // -- Not defined --
    //    0x2C60, 0x2C7F, // Latin Extended-C
    //    0xa720, 0xa7ff, // Latin Extended-D
    //    0xAB30, 0xAB6F, // Latin Extended-E
    //    0x2DE0, 0x2DFF, // Cyrillic Extended-A
    //    0xA640, 0xA69F, // Cyrillic Extended-B
    //    0x1C80, 0x1C8F, // Cyrillic Extended-C
    //    0xA700, 0xA71F, // Modifier Tone Letters
    //    0x2E00, 0x2E7F, // Supplemental Punctuation
    //    0x1D80, 0x1DBF, // Phonetic Extensions Supplement
    //    0x0300, 0x036F, // Combining Diacritical Marks Extended
    //    0x1DC0, 0x1DFF, // Combining Diacritical Marks Supplement
    //    0
    //};
    //
    //
    //static const ImWchar rangesNotoJP[] =
    //{
    //    0x1100, 0x11FF, // Hangul Jamo
    //    0x2460, 0x24FF, // Enclosed Alphanumerics
    //    0x2600, 0x26FF, // Miscellaneous Symbols
    //    0x2B00, 0x2BFF, // Miscellaneous Symbols and Arrows
    //    0x2E80, 0x2EFF, // CJK Radicals Supplement
    //    0x2F00, 0x2FDF, // Kangxi Radicals
    //    0x2FF0, 0x2FFF, // Ideographic Description Characters
    //    0x3000, 0x303F, // CJK Symbols and Punctuation
    //    0x3040, 0x309F, // Hiragana
    //    0x30A0, 0x30FF, // Katakana
    //    0x3100, 0x312F, // Bopomofo
    //    0x3130, 0x318F, // Hangul Compatibility Jamo
    //    0x31A0, 0x31BF, // Bopomofo Extended
    //    0x31F0, 0x31FF, // Katakana Phonetic Extensions
    //    0x3200, 0x32FF, // Enclosed CJK Letters and Months
    //    0x3300, 0x33FF, // CJK Compatibility
    //    0x3400, 0x4DBF, // CJK Unified Ideographs Extension A
    //    0x4E00, 0x9FAF, // CJK Unified Ideographs
    //    0xAC00, 0xD7AF, // Hangul Syllables
    //    0xF900, 0xFAFF, // CJK Compatibility Ideographs
    //    0xFB00, 0xFB4F, // Alphabetic Presentation Forms
    //    0xFE30, 0xFE4F, // CJK Compatibility Forms
    //    0xFE50, 0xFE6F, // Small Form Variants
    //    0xFF00, 0xFFEF, // Halfwidth and Fullwidth Forms
    //    0x3190, 0x319F, // Kanbun (Kunten)
    //    
    //    // -- Not defined --
    //    0x31C0, 0x31EF, // CJK Strokes
    //    0xFE10, 0xFE1F, // Vertical Forms
    //    0xA960, 0xA97F, // Hangul Jamo Extended-A
    //    0xD7B0, 0xD7FF, // Hangul Jamo Extended-B
    //    0x2C60, 0x2C7F, // CJK Unified Ideographs Extension B
    //    0
    //};
    //
    //
    //static const ImWchar rangesNotoHebrew[] =
    //{
    //    0x0590, 0x05FF, // Hebrew
    //    0
    //};
    //
    //
    //static const ImWchar rangesNotoArabic[] =
    //{
    //    0x0600, 0x06FF, // Arabic
    //    0xFB50, 0xFDFF, // Arabic Presentation Forms-A
    //    0xFE70, 0xFEFF, // Arabic Presentation Forms-B
    //
    //    // -- Not defined --
    //    0x0750, 0x077F, // Arabic Supplement
    //    0x08A0, 0x08FF, // Arabic Extended-A
    //    0
    //};
    //
    //
    //static const ImWchar rangesNotoSyriac[] =
    //{
    //    0x0700, 0x074F, // Syriac
    //    0
    //};
    //
    //
    //static const ImWchar rangesNotoTibetan[] =
    //{
    //    0x0F00, 0x0FFF, // Tibetan
    //    0
    //};
    //
    //
    //static const ImWchar rangesNotoThai[] =
    //{
    //    0x0E00, 0x0E7F, // Thai
    //    0
    //};
    //
    //
    //static const ImWchar rangesNotoMyanmar[] =
    //{
    //    0x1000, 0x109F, // Myanmar
    //    0xAA60, 0xAA7F, // Myanmar Extended-A
    //    0xA9E0, 0xA9FF, // Myanmar Extended-B
    //    0
    //};
    //
    //
    //static const ImWchar rangesNotoArmenian[] =
    //{
    //    0x0530, 0x058F, // Armenian
    //    0
    //};


    //static const ImWchar ranges[] =
    //{
    //    0x0780, 0x07BF, // Thaana
    //    0x0A00, 0x0A7F, // Gurmukhi
    //    0x0A80, 0x0AFF, // Gujarati
    //    0x0B00, 0x0B7F, // Oriya
    //    0x0B80, 0x0BFF, // Tamil
    //    0x0C00, 0x0C7F, // Telugu
    //    0x0C80, 0x0CFF, // Kannada
    //    0x0D00, 0x0D7F, // Malayalam
    //    0x0D80, 0x0DFF, // Sinhala
    //    0x0E80, 0x0EFF, // Lao
    //    0x10A0, 0x10FF, // Georgian
    //    0x1200, 0x137F, // Ethiopic
    //    0x13A0, 0x13FF, // Cherokee
    //    0x1400, 0x167F, // Unified Canadian Aboriginal Syllabics
    //    0x1680, 0x169F, // Ogham
    //    0x16A0, 0x16FF, // Runic
    //    0x1700, 0x171F, // Tagalog
    //    0x1720, 0x173F, // Hanunoo
    //    0x1740, 0x175F, // Buhid
    //    0x1760, 0x177F, // Tagbanwa
    //    0x1780, 0x17FF, // Khmer
    //    0x1800, 0x18AF, // Mongolian
    //    0x1900, 0x194F, // Limbu
    //    0x1950, 0x197F, // Tai Le
    //    0x19E0, 0x19FF, // Khmer Symbols
    //    0x2150, 0x218F, // Number Forms
    //    0x2400, 0x243F, // Control Pictures
    //    0x2440, 0x245F, // Optical Character Recognition
    //    0x27F0, 0x27FF, // Supplemental Arrows-A
    //    0x2800, 0x28FF, // Braille Patterns
    //    0x2900, 0x297F, // Supplemental Arrows-B
    //    0x2A00, 0x2AFF, // Supplemental Mathematical Operator
    //    0x4DC0, 0x4DFF, // Yijing Hexagram Symbols
    //    0xA000, 0xA48F, // Yi Syllables
    //    0xA490, 0xA4CF, // Yi Radicals
    //    0xD800, 0xDBFF, // High Surrogate Area
    //    0xDC00, 0xDFFF, // Low Surrogate Area
    //    0xE000, 0xF8FF, // Private Use Area
    //    0xFE00, 0xFE0F, // Variation Selectors
    //    0xFFF0, 0xFFFF, // Specials
    //    0
    //};


    static const ImWchar rangesAll[] =
    {
        0x0020, 0xFFFF,
        0
    };


    ImFontConfig config;
    config.OversampleH = 3;
    constexpr float fontSize = 22;
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromMemoryCompressedTTF(NotoSans_compressed_data, NotoSans_compressed_size, fontSize, &config, rangesAll);

    config.MergeMode = true;
    //io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansJP_compressed_data, NotoSansJP_compressed_size, fontSize, &config, rangesNotoJP);
    //io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansSC_compressed_data, NotoSansSC_compressed_size, fontSize, &config, rangesAll);
    //io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansTC_compressed_data, NotoSansTC_compressed_size, fontSize, &config, rangesAll);
    //io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansHebrew_compressed_data, NotoSansHebrew_compressed_size, fontSize, &config, rangesNotoHebrew);
    //io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansArabic_compressed_data, NotoSansArabic_compressed_size, fontSize, &config, rangesNotoArabic);
    //io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansSyriac_compressed_data, NotoSansSyriac_compressed_size, fontSize, &config, rangesNotoSyriac);
    //io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansThai_compressed_data, NotoSansThai_compressed_size, fontSize, &config, rangesNotoThai);
    //io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansMyanmar_compressed_data, NotoSansMyanmar_compressed_size, fontSize, &config, rangesNotoMyanmar);
    //io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansTibetan_compressed_data, NotoSansTibetan_compressed_size, fontSize, &config, rangesNotoTibetan);
    //io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansArmenian_compressed_data, NotoSansArmenian_compressed_size, fontSize, &config, rangesNotoArmenian);
    
    io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansJP_compressed_data, NotoSansJP_compressed_size, fontSize, &config, rangesAll);
    io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansSC_compressed_data, NotoSansSC_compressed_size, fontSize, &config, rangesAll);
    io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansTC_compressed_data, NotoSansTC_compressed_size, fontSize, &config, rangesAll);
    io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansHebrew_compressed_data, NotoSansHebrew_compressed_size, fontSize, &config, rangesAll);
    io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansArabic_compressed_data, NotoSansArabic_compressed_size, fontSize, &config, rangesAll);
    io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansSyriac_compressed_data, NotoSansSyriac_compressed_size, fontSize, &config, rangesAll);
    io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansThai_compressed_data, NotoSansThai_compressed_size, fontSize, &config, rangesAll);
    io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansMyanmar_compressed_data, NotoSansMyanmar_compressed_size, fontSize, &config, rangesAll);
    io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansTibetan_compressed_data, NotoSansTibetan_compressed_size, fontSize, &config, rangesAll);
    io.Fonts->AddFontFromMemoryCompressedTTF(NotoSansArmenian_compressed_data, NotoSansArmenian_compressed_size, fontSize, &config, rangesAll);
    io.Fonts->Build();
}


bool RenderWindow::ImGuiInit(const char* iniFileName) const noexcept
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGuiLoadFonts();
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