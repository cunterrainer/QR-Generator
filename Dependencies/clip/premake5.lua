project "Clip"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    warnings "Off"

    files {
        "clip.cpp",
        "image.cpp"
    }

    filter "system:windows"
        defines "_SCL_SECURE_NO_WARNINGS"
        files "clip_win.cpp"

    filter "system:macosx"
        files "clip_osx.mm"

    filter "system:linux"
        defines { "HAVE_XCB_XLIB_H", "HAVE_PNG_H" }
        files "clip_x11.cpp"
