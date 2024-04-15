
group "Vendor"
    include "vendor/Unity"
group ""

project "Titanium-Tests"
    kind "ConsoleApp"

    targetdir "%{prj.location}/bin/"
    objdir "%{prj.location}/bin-int/"

    includedirs { 
        "%{wks.location}/titanium-core/src",
        "vendor/Unity/src"
    }
    
    files { 
        "tests/**.c",
        "tests/**.h",
    }

    links {
        "Titanium-Core",
        "Unity"
    }
