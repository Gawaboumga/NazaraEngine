#ifndef __URL_HPP__
#define __URL_HPP__

namespace SMB
{
	class URL
	{
		public:

			static Nz::String GetImage(Level::Info info)
			{
				return "/home/yhubaut/NazaraEngine/examples/bin/resources/Mario/mario_tileset.png";
			}

			static Nz::String GetTMXPath(Level::Info info)
			{
				return "/home/yhubaut/NazaraEngine/examples/bin/resources/Mario/Levels/level_" + Nz::String::Number(info) + ".tmx";
			}
	};
}

#endif // __URL_HPP__
