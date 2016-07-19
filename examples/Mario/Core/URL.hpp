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
				return Root() + "Levels/level_" + Nz::String::Number(info) + "_coins.xml";
			}

			static Nz::String GetEnemyPath(Level::Info info)
			{
				return Root() + "Levels/level_" + Nz::String::Number(info) + "_enemies.xml";
			}

			static Nz::String GetImage(Level::Info info)
			{
				return Root() + "mario_tileset.png";
			}

			static Nz::String GetTMXPath(Level::Info info)
			{
				return Root() + "Levels/level_" + Nz::String::Number(info) + ".tmx";
			}

			static Nz::String GetCoinAnimation()
			{
				return Root() + "coin_animation.xml";
			}

			static Nz::String GetGoompaAnimation()
			{
				return Root() + "Elements/goompa_animation.xml";
			}

			static Nz::String GetMarioAnimation()
			{
				return Root() + "Elements/mario_animation.xml";
			}

		private:

			static Nz::String Root()
			{
				return "/home/yhubaut/NazaraEngine/examples/bin/resources/Mario/";
			}
	};
}

#endif // __URL_HPP__
