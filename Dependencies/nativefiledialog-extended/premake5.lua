project "nativefiledialog-extended"
    kind "StaticLib"
    language "C"

    files "src/include/**"

    includedirs "src/include"

    filter "system:windows"
        files "src/nfd_win.cpp"

    filter "system:linux"
        files "src/nfd_gtk.cpp"

   filter "system:macosx"
        files "src/nfd_cocoa.m"
   filter {}
