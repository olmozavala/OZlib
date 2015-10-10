OZLIB="/media/USBSimpleDrive/Olmo/OZlib/"

solution "Solution"
 configurations { "Debug", "Release" }
-- location("build")
 
   -- A project defines one build target
   project "LibTester"

      kind "ConsoleApp"

      language "C++"
      files { "**.h", "**.cpp" }	  
      targetdir(".")

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
