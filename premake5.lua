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

   files { "Include/*.h", "src/*.c" }

   links { "SDL2", "SDL2main" }

   includedirs{ "./Include" }

   filter "action:codeblocks"
      includedirs { "../../_Dev_Libs/SDL_mingw32/include/SDL2" }

      linkoptions {  "-lmingw32", "-lSDL2main", "-lSDL2", "-pg" }

   filter "action:vs2019"
      includedirs { "../../_Dev_Libs/SDL2_Windows/*/include" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

   filter "platforms:Win32"
      architecture "x32"
      system "windows"

      if _ACTION == "codeblocks" then
         libdirs { "../../_Dev_Libs/SDL_mingw32/lib" }
      end
      if _ACTION == "vs2019" then
         libdirs { "../../_Dev_Libs/SDL2_Windows/*/x86" }
      end

   filter "platforms:Win64"
      architecture "x86_64"
      system "windows"
      
      if _ACTION == "codeblocks" then
         libdirs { "../../_Dev_Libs/SDL_x64_mingw32/lib" }
      end
      if _ACTION == "vs2019" or _ACTION == "vs2017" then
         libdirs { "../../_Dev_Libs/SDL2_Windows/*/x64" }
      end

   filter "platforms:Mac"
      system "macosx"

   filter "platforms:Linux"
      system "linux"