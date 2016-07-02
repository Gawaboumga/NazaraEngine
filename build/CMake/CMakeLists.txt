cmake_minimum_required (VERSION 2.8.11)
project (Nazara)

set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14")
set (OS_AUDIO_SPECIFICATION OpenAL)
set (OS_CORE_SPECIFICATION Posix)
set (OS_NETWORK_SPECIFICATION Posix)
set (OS_RENDERER_SPECIFICATION GLX)
set (OS_UTILITY_SPECIFICATION X11)
set (BUILD_SHARED_LIBS SHARED)
set (NAZARA_LIBRARY_EXTENSION "-d")
set (SDK_LIBRARY_EXTENSION "-d")

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../../../lib)
set (EXECUTABLE_OUTPUT_PATH ../../../../demo)

set (NAZARA_EXAMPLES_DIRECTORY ../../../../examples/)
set (NAZARA_EXTLIBS_DIRECTORY ../../../../extlibs/)
set (NAZARA_INCLUDE_DIRECTORY ../../../../include/)
set (NAZARA_SOURCE_DIRECTORY ../../../../src/)

set (NAZARA_NDK_INCLUDE_DIRECTORY ../../../../SDK/include/)

set (NAZARA_LIBRARY_NAME Nazara)
set (NAZARA_LIBRARY_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set (NAZARA_LIB_AUDIO NazaraAudio-d)
set (NAZARA_LIB_CORE NazaraCore-d)
set (NAZARA_LIB_GRAPHICS NazaraGraphics-d)
set (NAZARA_LIB_LUA NazaraLua-d)
set (NAZARA_LIB_NOISE NazaraNoise-d)
set (NAZARA_LIB_NETWORK NazaraNetwork-d)
set (NAZARA_LIB_PHYSICS NazaraPhysics-d)
set (NAZARA_LIB_RENDERER NazaraRenderer-d)
set (NAZARA_LIB_UTILITY NazaraUtility-d)

set (NAZARA_LIB_SDK NazaraSDK-d)

add_subdirectory (extlibs)

add_subdirectory (modules)
add_subdirectory (tools)
add_subdirectory (examples)
