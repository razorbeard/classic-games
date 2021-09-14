workspace "classic-games"
    architecture "x64"
    configurations { "Debug", "Release" }

project "classic-games"
    kind "ConsoleApp"
    language "C++"
    
    targetdir "bin/%{cfg.architecture}/%{cfg.buildcfg}"
    objdir "bin/intermediates/%{cfg.architecture}/%{cfg.buildcfg}"

    files
    {
        "src/**.hpp",
        "src/**.cpp"
    }

    includedirs
    {
        "src"
    }
        
    postbuildcommands { "{COPYDIR} resources %{cfg.targetdir}/resources" }
	
	-- On windows, openal32.dll is needed even if we use static linking,
	-- due to the licence used (LGPL v2)
	configuration "windows"
	    postbuildcommands { "{COPYFILE} vendor/SFML-2.5.1/bin/openal32.dll %{cfg.targetdir}" }
    
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines { "SFML_STATIC" }
        
        libdirs { "vendor/SFML-2.5.1/lib" }
        
        includedirs { "vendor/SFML-2.5.1/include" }
        
        links
           {
              "flac.lib",
              "freetype.lib",
              "gdi32.lib",
              "ogg.lib",
              "openal32.lib",
              "opengl32.lib",
              "vorbisenc.lib",
              "vorbisfile.lib",
              "vorbis.lib",
              "winmm.lib",
           }
        
    filter { "system:windows", "configurations:Debug" }
       symbols "on"

       defines { "DEBUG" }

       links
       {
          "sfml-audio-s-d.lib", 
          "sfml-graphics-s-d.lib",
          "sfml-system-s-d.lib", 
          "sfml-window-s-d.lib" 
       }

    filter { "system:windows", "configurations:Release" }
       symbols "on"
       optimize "on"

       defines { "NDEBUG" }

       links
       {
          "sfml-audio-s.lib", 
          "sfml-graphics-s.lib",
          "sfml-system-s.lib", 
          "sfml-window-s.lib" 
       }

    -- Under Linux, we only work with a release + dynamic librairies setup 
    filter "system:linux"
        removeconfigurations { "Debug" }
        
        cppdialect "C++17"
        systemversion "latest"  
        
        symbols "on"
        optimize "on"
        
        links
           {
              "pthread",
              "sfml-graphics",
              "sfml-window",
              "sfml-audio", 
              "sfml-system",
           }