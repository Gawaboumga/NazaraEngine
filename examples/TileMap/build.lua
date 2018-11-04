EXAMPLE.Name = "TileMap"

EXAMPLE.EnableConsole = true

EXAMPLE.Includes = {
	"../examples/TileMap/TMX/include",
	"../examples/TileMap/TMX/thirdparty",
}

EXAMPLE.Files = {
	"TMX/include/TMX/**.hpp",
	"TMX/include/TMX/**.inl",
	"TMX/src/TMX/**.cpp",
	"TMX/thirdparty/pugixml/src/**.cpp",
	"TMX/thirdparty/miniz/**.c",
	"main.cpp",
	"MapCreator.cpp"
}

EXAMPLE.Libraries = {
	"NazaraSDK"
}
