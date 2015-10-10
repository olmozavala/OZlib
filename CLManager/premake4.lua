-- OPENCL="/opt/AMDAPP/"
OPENCL="/usr/local/cuda"
OZLIB=".."


solution "OpenCL_Wrapper_lib"
 configurations { "Debug", "Release" }
-- location("build")

   -- A project defines one build target
   project "OpenCL_Wrapper_lib"

      --Shared are .o static are .a
      --kind "StaticLib"
      kind "SharedLib"
      --kind "ConsoleApp"
      
      language "C++"
      includedirs{OPENCL.."/include", OZLIB,
                    OZLIB .."khronos" }
                    
      libdirs{OZLIB}

      targetdir("../")
      targetname("CLManager")

      location "."
      --This options seems to be requiered for AMD64 shared libraries
      buildoptions("-fPIC")

      files {"**.hpp", "**.h", "**.cpp" }     

      --links {"OpenCL","FileManager"}
      links({"OpenCL","GL","GLU","glut","GLEW","X11","m","FileManager",
         "GordonTimers","freeimage"})
 
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }
