-- premake5.lua

workspace "sharkyEngine"
   configurations { "Debug", "Release" }
   startproject     "sharkyEngine"

   -- Choose the compiler toolset ("gcc", "clang", "msc", etc.)
   toolset "clang"               -- or "clang" if you prefer :contentReference[oaicite:1]{index=1}

   -- Where to place generated build files (Makefiles, Ninja, etc.)
   location "build"

project "sharkyEngine"
   kind        "ConsoleApp"
   language    "C++"
   cppdialect  "C++23"
   targetdir   "build/%{cfg.buildcfg}"

   -- Include search paths
   includedirs { "include", "extern" }

   -- Source files
   files {
      "src/**.cpp",       -- main.cpp and any other .cpp in src/
      "src/**.c",         -- glad.c
      "src/engine/**.cpp",
      "src/engine/window/**.cpp",
      "src/engine/render/**.cpp",
      "src/engine/physics/**.cpp",
      "src/engine/objects/**.cpp",
      "src/imgui/**.cpp", -- all ImGui backends & core
   }

   -- Compiler flags
   -- -Wall and C++17 automatically handled by Premake,
   -- extra flags can be appended:
   buildoptions { "-Wall", "-pedantic-errors", "-Weffc++", "-Wextra", "-Wconversion", "-Wsign-conversion", "-Wsign-compare"}

   -- Linker settings
   links { "glfw", "GL", "X11", "pthread", "Xrandr", "dl" }
   -- Configuration-specific settings
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols  "On"

   filter "configurations:Release"
      defines   { "NDEBUG" }
      optimize  "On"
