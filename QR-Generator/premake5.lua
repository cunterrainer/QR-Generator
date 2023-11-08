project "QR-Generator"
    language "C++"
    cppdialect "C++20"
    characterset "Unicode"

    files {
        "src/**.cpp",
        "src/**.h",
        "vendor/stb/**.h",
        "vendor/stb/**.cpp",
        "vendor/MsgBoxX11/**.cpp",
        "vendor/MsgBoxX11/**.h",
        "vendor/qrcodegen/cpp/qrcodegen.hpp",
        "vendor/qrcodegen/cpp/qrcodegen.cpp",
    }

    includedirs {
        "src",
        "vendor",
        "../Dependencies/GLFW/include",
        "../Dependencies/ImGui/include",
        "../Dependencies/nativefiledialog/include"
    }

    externalincludedirs {
        "vendor",
        "../Dependencies/ImGui/include"
    }

    flags "FatalWarnings"

    links {
        "glfw",
        "nativefiledialog",
        "ImGui"
    }

    filter "system:windows"
        defines "_CRT_SECURE_NO_WARNINGS"
        links {
            "gdi32",
            "opengl32",
            "shell32",
            "ole32",
            "uuid"
        }

    filter "system:linux"
        links {
            "GL",
            "gtk-3",
            "glib-2.0",
            "gobject-2.0",
            "X11"
        }

    filter "system:macosx"
        defines "GL_SILENCE_DEPRECATION"
        linkoptions "-framework AppKit -framework iokit -framework OpenGl"
        disablewarnings { "sign-conversion" }
        files "src/**.m"

    filter { "toolset:gcc* or toolset:clang*", "platforms:x86" }
        linkoptions "res/iconx86.res"

    filter { "toolset:gcc* or toolset:clang*", "platforms:x64" }
        linkoptions "res/icon.res"

    --gcc* clang* msc*
    filter "toolset:msc*"
        warnings "High"
        externalwarnings "Default"
        files "res/icon.rc"
        buildoptions { "/sdl" }

    filter { "toolset:gcc* or toolset:clang*" }
        enablewarnings {
            "cast-align",
            "cast-qual",
            --"ctor-dtor-privacy",
            "disabled-optimization",
            "format=2",
            "init-self",
            "missing-include-dirs",
            "overloaded-virtual",
            "redundant-decls",
            "shadow",
            "sign-promo",
            "switch-default",
            "undef",
            "uninitialized",
            "unreachable-code",
            "unused",
            "alloca",
            --"conversion",
            "deprecated",
            "format-security",
            --"null-dereference",
            "stack-protector",
            "vla",
            "shift-overflow"
        }
        disablewarnings "unknown-warning-option"

    filter "toolset:gcc*"
        warnings "Extra"
        externalwarnings "Off"
        linkoptions "res/icon.res"
        filter { "toolset:gcc*", "not system:macosx" }
            linkgroups "on" -- activate position independent linking
        filter "toolset:gcc*"
        enablewarnings {
            "noexcept",
            "strict-null-sentinel",
            "array-bounds=2",
            "duplicated-branches",
            "duplicated-cond",
            "logical-op",
            "arith-conversion",
            "stringop-overflow=4",
            "implicit-fallthrough=3",
            "trampolines"
        }
        disablewarnings "cast-function-type"

    filter "toolset:clang*"
        warnings "Extra"
        externalwarnings "Everything"
        linkgroups "off"
        enablewarnings {
            "array-bounds",
            "long-long",
            "implicit-fallthrough", 
        }
        disablewarnings {"cast-align", "sign-conversion"}
    filter {}


    filter { "configurations:Debug" }
        kind "ConsoleApp"
    filter { "configurations:Release" }
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
