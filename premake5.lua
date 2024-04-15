
workspace "Titanium" 
    startproject "Titanium"
    architecture "x64"
	staticruntime "on"
    language "C"
    cdialect "C17"
    flags "MultiProcessorCompile"
    
    configurations { 
        "Debug", 
        "Release" 
    }

    filter "configurations:Debug"
    defines {
        "DBG_TRACE_EXECUTION",
        "DBG_STEP_EXECUTION",
        "TI_DEBUG"
    }

    symbols "On" 

    filter "configurations:Release" 
        defines { "TI_RELEASE" }      
        optimize "Speed" 

    filter { "action:gmake2" }
        buildoptions { "-Wall", "-pedantic-errors" }

    filter {}

    include "titanium-core"
    include "titanium-app"
    include "titanium-tests"
