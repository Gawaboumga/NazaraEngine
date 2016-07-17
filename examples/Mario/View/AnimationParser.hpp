#ifndef __ANIMATIONPARSER_HPP__
#define __ANIMATIONPARSER_HPP__

#include <Nazara/Core/String.hpp>

#include "Animation.hpp"

#include "pugixml.hpp"

#include <map>

namespace SMB
{
	class AnimationParser
	{
		public:

			static std::map<Nz::String, Nz::String> GetSpriteSheets(const Nz::String& filepath);

			static std::map<Nz::String, SMB::Animation> Load(const Nz::String& filepath);

		private:

			static std::pair<Nz::String, SMB::Animation> GetAnimation(pugi::xml_node& animationNode);
			static pugi::xml_node GetAnimationsNode(pugi::xml_document& doc, const Nz::String& filepath);
	};
}

#endif // __ANIMATIONPARSER_HPP__
