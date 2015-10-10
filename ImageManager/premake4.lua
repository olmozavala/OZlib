OZLIB="/home/olmozavala/Dropbox/OzOpenCL/OZlib"

solution "Solution"
 configurations { "Release" }
 --configurations { "Debug", "Release" }
-- location("build")
 
   -- A project defines one build target
   project "ImageManager"

      --kind "ConsoleApp"
      --kind "StaticLib"
      kind "sharedLib"

      language "C++"
      files { "**.h", "**.cpp" }	  
      targetdir("../")

      includedirs{ OZLIB, 
                    OZLIB .."/khronos" }

      libdirs{OZLIB}
      links{"freeimage"}

      buildoptions("-fPIC")

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }    
