workspace "Honours Project"
    architecture "x64"
    configurations { "Debug", "Release" }

project "HonsProject-OpenCV-GUI"
    location "src"
    kind "ConsoleApp"
    language "C++"
    
    targetdir "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    files { 
        "./src/**.h",
        "./src/**.hpp",
        "./src/**.c",
        "./src/**.cpp"
    }

    includedirs {
        -- Project source code
        
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

-- Linux system
    filter "system:linux"
        system "linux"
        cppdialect "gnu++17"
        links { "glfw", "GL", "m", "X11", "pthread", "Xi", "Xrandr", "dl" }

-- Build configurations
    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "On"