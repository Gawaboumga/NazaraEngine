set(NAZARA_SOURCE_CORE_DIRECTORY ${NAZARA_SOURCE_DIRECTORY}Nazara/Utility/)

include_directories(
		${NAZARA_INCLUDE_DIRECTORY}
		${NAZARA_SOURCE_DIRECTORY}
		${NAZARA_EXTLIBS_DIRECTORY}/include/
	)

file(GLOB UTILITY_SOURCE
		${NAZARA_SOURCE_CORE_DIRECTORY}/Debug/*.cpp
		${NAZARA_SOURCE_CORE_DIRECTORY}/Formats/*.cpp
		${NAZARA_SOURCE_CORE_DIRECTORY}/${OS_UTILITY_SPECIFICATION}/*.cpp
		${NAZARA_SOURCE_CORE_DIRECTORY}/*.cpp
	)

link_directories(${NAZARA_EXTLIBS_DIRECTORY}/lib/gmake/x64/)

add_library(${NAZARA_LIBRARY_NAME}Utility${NAZARA_LIBRARY_EXTENSION} ${BUILD_SHARED_LIBS} ${UTILITY_SOURCE})

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
