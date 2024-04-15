
project "Titanium-Core"
    kind "StaticLib"

    targetdir "%{prj.location}/bin/%{cfg.buildcfg}-%{cfg.architecture}"
    objdir "%{prj.location}/bin-int/%{cfg.buildcfg}-%{cfg.architecture}"

    includedirs { 
        "src/" 
    }
    
    files { 
        "src/**.h", 
        "src/**.c" 
    }
