#ifndef __TMXPARSER_HPP__
#define __TMXPARSER_HPP__

#include <Nazara/Math/Vector2.hpp>

#include <Matrix/Matrix.hpp>

namespace pugi
{
	class xml_document;
	class xml_node;
}

namespace SMB
{
	class TMXParser
	{
		public:

			static Matrix<Nz::String, 2> GetMap(const Nz::String& filename);

			static Nz::Vector2ui GetTileSize(const Nz::String& filename);

		private:

			static pugi::xml_node GetMapNode(pugi::xml_document& doc, const Nz::String& filename);
	};
}

#endif // __TMXPARSER_HPP__
