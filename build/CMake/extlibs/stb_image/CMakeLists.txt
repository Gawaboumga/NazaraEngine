set(STB_SOURCE_DIRECTORY ${NAZARA_EXTLIBS_DIRECTORY}src/stb/)

include_directories(
		${NAZARA_EXTLIBS_DIRECTORY}/include/
	)

file(GLOB LUA_SOURCE
		${STB_SOURCE_DIRECTORY}/*.cpp
	)

link_directories(${NAZARA_EXTLIBS_DIRECTORY}/lib/gmake/x64/)

add_library(stb_image${NAZARA_LIBRARY_EXTENSION} ${BUILD_SHARED_LIBS} ${LUA_SOURCE})

target_link_libraries(stb_image${NAZARA_LIBRARY_EXTENSION})
