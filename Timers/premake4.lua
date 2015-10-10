OZLIB="/media/USBSimpleDrive/Olmo/OZlib/"

solution "Solution"
 configurations { "Debug", "Release" }
-- location("build")
 
   -- A project defines one build target
   project "GordonTimers"

      --kind "ConsoleApp"
      --kind "StaticLib"
      kind "sharedLib"

      language "C++"
      files { "**.h", "**.cpp" }	  
      targetdir("../")

      includedirs{OZLIB}
      libdirs{OZLIB}

      buildoptions("-fPIC")

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }    
