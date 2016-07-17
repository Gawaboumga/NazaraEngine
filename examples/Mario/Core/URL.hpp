#ifndef __URL_HPP__
#define __URL_HPP__

#include "Level.hpp"

namespace SMB
{
	class URL
	{
		public:

			static Nz::String GetCoinPath(Level::Info info)
			{
				return "/home/yhubaut/NazaraEngine/examples/bin/resources/Mario/Levels/level_" + Nz::String::Number(info) + "_coins.xml";
			}

			static Nz::String GetEnemyPath(Level::Info info)
			{
				return "/home/yhubaut/NazaraEngine/examples/bin/resources/Mario/Levels/level_" + Nz::String::Number(info) + "_enemies.xml";
			}

			static Nz::String GetImage(Level::Info info)
			{
				return "/home/yhubaut/NazaraEngine/examples/bin/resources/Mario/mario_tileset.png";
			}

			static Nz::String GetTMXPath(Level::Info info)
			{
				return "/home/yhubaut/NazaraEngine/examples/bin/resources/Mario/Levels/level_" + Nz::String::Number(info) + ".tmx";
			}

			static Nz::String GetMarioSpriteSheet()
			{
				return "/home/yhubaut/NazaraEngine/examples/bin/resources/Mario/Elements/smb_mario_sheet.png";
			}

			static Nz::String GetEnemiesSpriteSheet()
			{
				return "/home/yhubaut/NazaraEngine/examples/bin/resources/Mario/Elements/smb_enemies_sheet.png";
			}

			static Nz::String GetCoinAnimation()
			{
				return "/home/yhubaut/NazaraEngine/examples/bin/resources/Mario/coin_animation.xml";
			}

			static Nz::String GetGoompaAnimation()
			{
				return "/home/yhubaut/NazaraEngine/examples/bin/resources/Mario/Elements/goompa_animation.xml";
			}

			static Nz::String GetMarioAnimation()
			{
				return "/home/yhubaut/NazaraEngine/examples/bin/resources/Mario/Elements/mario_animation.xml";
			}
	};
}

#endif // __URL_HPP__
