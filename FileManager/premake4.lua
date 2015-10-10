solution "FileManager"
 configurations { "Debug", "Release" }

   -- A project defines one build target
   project "FileManager"

        --Shared are .o static are .a
      --kind "staticLib"
      kind "sharedLib"

      language "C++"
      files {"**.hpp", "**.h", "**.cpp" }     
      targetdir ("../")

      buildoptions("-fPIC")
 
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }
         --location "../"

