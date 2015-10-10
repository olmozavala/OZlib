CUDA="/usr/local/cuda"
OZLIB="/media/USBSimpleDrive/Olmo/OZlib/"

solution "Solution"
 configurations { "Debug", "Release" }
-- location("build")
 
   -- A project defines one build target
   project "MatrixUtils"

      --kind "ConsoleApp"
      --kind "StaticLib"
      kind "sharedLib"

      language "C++"
      files { "**.h", "**.cpp" }      
      targetdir("../")

      includedirs{CUDA.."/include",
                    OZLIB, 
                    OZLIB .."/khronos" }

      libdirs{OZLIB}
      links{"freeimage","OpenCL"}

      buildoptions("-fPIC")

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }    
