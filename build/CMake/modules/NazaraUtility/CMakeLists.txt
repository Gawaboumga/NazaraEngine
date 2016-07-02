project("NazaraUtility")

if (UNIX)
	set (OS_UTILITY_SPECIFICATION "X11")
elseif (WIN32)
	set (OS_UTILITY_SPECIFICATION "Win32")
else ()
	message(SEND_ERROR "No OS implementation")
endif ()

set(NAZARA_SOURCE_UTILITY_DIRECTORY ${NAZARA_SOURCE_DIRECTORY}Nazara/Utility/)

include_directories(
		${NAZARA_INCLUDE_DIRECTORY}
		${NAZARA_SOURCE_DIRECTORY}
		${NAZARA_EXTLIBS_INCLUDE_DIRECTORY}
	)

file(GLOB UTILITY_SOURCE
		${NAZARA_SOURCE_UTILITY_DIRECTORY}Debug/*.cpp
		${NAZARA_SOURCE_UTILITY_DIRECTORY}Formats/*.cpp
		${NAZARA_SOURCE_UTILITY_DIRECTORY}${OS_UTILITY_SPECIFICATION}/*.cpp
		${NAZARA_SOURCE_UTILITY_DIRECTORY}*.cpp
	)

link_directories(${NAZARA_EXTLIBS_OUTPUT_DIRECTORY})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${NAZARA_LIBS_OUTPUT_DIRECTORY})

add_library(${NAZARA_LIBRARY_NAME}Utility${NAZARA_LIBRARY_EXTENSION} ${BUILD_SHARED_LIBS} ${UTILITY_SOURCE})

if (UNIX)
	target_link_libraries(${NAZARA_LIBRARY_NAME}Utility${NAZARA_LIBRARY_EXTENSION}
		PUBLIC
			"freetype"
			"X11"
			"xcb"
			"xcb-cursor"
			"xcb-ewmh"
			"xcb-icccm"
			"xcb-keysyms"
			"xcb-randr"

		PRIVATE
			"stb_image-s"
	)
elseif (WIN32)
	target_link_libraries(${NAZARA_LIBRARY_NAME}Utility${NAZARA_LIBRARY_EXTENSION}
		PUBLIC
			"freetype"
			"gdi32"

		PRIVATE
			"stb_image-s"
	)
else ()
	message(SEND_ERROR "No OS implementation")
endif ()

