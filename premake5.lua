workspace "Bullet_Hell"
   filename "Bullet_Hell_Workspace"
   location("builds/" .. _ACTION)

   configurations { "Debug", "Release" }
   platforms { "Win32", "Win64", "Mac", "Linux" }

project "Bullet_Hell"
   filename "Bullet_Hell_Project"
   location("builds/" .. _ACTION .. "/project")

   kind "ConsoleApp"
   language "C"
   targetdir "%{prj.location}/%{cfg.buildcfg}_%{cfg.platform}"

   files { "./Include/*.h", "./Source/*.c" }

   links { "SDL2main", "SDL2", "SDL2_gpu_s" }

   includedirs { "./Include" }

   defines { "GPU_USE_INT_BOOL" }

   filter "action:vs2019"
      includedirs { "../../_Dev_Libs/SDL2_Windows/*/Include" }
      buildoptions { "/TC", "/Za" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

   filter "platforms:Win32"
      architecture "x32"
      system "windows"

      if _ACTION == "vs2019" then
         libdirs { "../../_Dev_Libs/SDL2_Windows/*/x86" }
      end

   filter "platforms:Win64"
      architecture "x86_64"
      system "windows"
      
      if _ACTION == "vs2019" then
         libdirs { "../../_Dev_Libs/SDL2_Windows/*/x64" }
      end

   filter "platforms:Mac"
      system "macosx"

   filter "platforms:Linux"
      system "linux"