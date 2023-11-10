project "nativefiledialog-extended"
    kind "StaticLib"
    language "C"

    files "src/include/**"

    includedirs "src/include"

    filter "system:windows"
        files "src/nfd_win.cpp"

    filter "system:linux"
        files "src/nfd_gtk.cpp"
        includedirs {
            "/usr/include/gtk-3.0",
            "/usr/include/glib-2.0",
            "/usr/lib/x86_64-linux-gnu/glib-2.0/include",
            "/usr/lib/glib-2.0/include",
            "/usr/include/pango-1.0",
            "/usr/include/harfbuzz",
            "/usr/include/cairo",
            "/usr/include/gdk-pixbuf-2.0",
            "/usr/include/atk-1.0"
        }

   filter "system:macosx"
        files "src/nfd_cocoa.m"
   filter {}
