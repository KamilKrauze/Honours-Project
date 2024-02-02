workspace "Honours Project"
    architecture "x64"
    configurations { "Debug", "Release" }

project "HonsProject-OpenCV-GUI"
    location "GUI-Build"
    kind "ConsoleApp"
    language "C++"
    
    targetdir "build/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    files { 
        "./GUI-Build/**.h",
        "./GUI-Build/**.hpp",
        "./GUI-Build/**.c",
        "./GUI-Build/**.cpp",

        -- ImGUI CPP files
        "./include/**.cpp"
    }

    includedirs {
        -- Project source code
        "./GUI-Build/",
        
        -- Thirdparty
        "./include/",
        "./include/imgui/" --ImGUI has to be explicitly done this way
    }

-- Windows system

    filter "system:windows"
        system "windows"
        cppdialect "C++17"
        systemversion "latest"    

-- Linux system
    filter "system:linux"
        system "linux"
        cppdialect "gnu++17"
        links { "glfw", "GL", "m", "X11", "pthread", "Xi", "Xrandr", "dl" }

-- Build configurations
    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"
        libdirs { "./lib/" }
        links { "glfw3D.lib", "opengl32.lib", "opencv_world490d.lib" }

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "On"
        libdirs { "./lib/" }
        links { "glfw3.lib", "opengl32.lib", "opencv_world490.lib" }


project "HonsProject-OpenCV-Console"
    location "Console-Build"
    kind "ConsoleApp"
    language "C++"
    
    targetdir "build/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    files { 
        "./Console-Build/**.h",
        "./Console-Build**.hpp",
        "./Console-Build/**.c",
        "./Console-Build/**.cpp"
    }

    includedirs {
        -- Project source code
        "./Console-Build/",
        
        -- Thirdparty
        "./include/"
    }

    libdirs {
        "./lib/"
    }

-- Windows system

    filter "system:windows"
        system "windows"
        cppdialect "C++17"
        systemversion "latest"

-- Build configurations
    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"
        links { "opencv_world490d.lib" }

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "On"
        links { "opencv_world490.lib" }