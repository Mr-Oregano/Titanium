    
project "Titanium-App"
    kind "ConsoleApp"

    targetdir "%{prj.location}/bin/%{cfg.buildcfg}-%{cfg.architecture}"
    objdir "%{prj.location}/bin-int/%{cfg.buildcfg}-%{cfg.architecture}"

    includedirs { 
        "%{wks.location}/titanium-core/src"
    }

    files { 
        "%{prj.location}/src/**.h", 
        "%{prj.location}/src/**.c" 
    }

    links "Titanium-Core"
