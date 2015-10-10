OZLIB="/home/olmozavala/Dropbox/OzOpenCL/OZlib"

solution "Solution"
 configurations { "Debug", "Release" }
-- location("build")
 
   -- A project defines one build target
   project "GLManager"

      --kind "ConsoleApp"
      --kind "StaticLib"
      kind "sharedLib"

      language "C++"
      files { "**.h", "**.cpp" }	  
      targetdir("../")

      includedirs{OZLIB}
      libdirs{OZLIB}

      buildoptions("-fPIC")

      links({"GL","GLU","glut","GLEW","X11","m","FileManager"})
 
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }    
