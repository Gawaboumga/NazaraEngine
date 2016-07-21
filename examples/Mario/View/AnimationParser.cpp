#include "AnimationParser.hpp"

#include "pugixml.hpp"

namespace SMB
{

	std::map<Nz::String, Nz::String> AnimationParser::GetSpriteSheets(const Nz::String& filepath)
	{
		pugi::xml_document doc;
		pugi::xml_node animationsNode = GetAnimationsNode(doc, filepath);

		std::map<Nz::String, Nz::String> texture;
		for (auto animationNode : animationsNode)
		{
			auto animationName = animationNode.attribute("name").value();
			auto imageNode = animationNode.child("image");
			Nz::String texturePath = imageNode.attribute("source").value();
			auto directoryPosition = filepath.FindLast('/');
			texturePath = filepath.SubString(0, directoryPosition) + texturePath;
			texture.insert(std::make_pair(animationName, texturePath));
		}
		return texture;
	}

	std::map<Nz::String, SMB::Animation> AnimationParser::Load(const Nz::String& filepath)
	{
		pugi::xml_document doc;
		pugi::xml_node animationsNode = GetAnimationsNode(doc, filepath);

		std::map<Nz::String, SMB::Animation> animations;
		for (auto animationNode : animationsNode)
		{
			std::pair<Nz::String, SMB::Animation> result = GetAnimation(animationNode);
			animations.insert(result);
		}
		return animations;
	}

	std::pair<Nz::String, SMB::Animation> AnimationParser::GetAnimation(pugi::xml_node& animationNode)
	{
		auto name = animationNode.attribute("name").value();
		auto width = static_cast<unsigned int>(std::stoi(animationNode.attribute("width").value()));
		auto height = static_cast<unsigned int>(std::stoi(animationNode.attribute("height").value()));
		auto framesNode = animationNode.child("frames");
		auto numberOfFrames = static_cast<unsigned int>(std::stoi(framesNode.attribute("number").value()));
		SMB::Animation animation{};
		if (!framesNode.first_child())
		{
			auto imageNode = animationNode.child("image");
			auto imageWidth = std::stoi(imageNode.attribute("width").value());

			for (auto i = 0u; i < numberOfFrames; ++i)
			{
				auto x = i % (imageWidth / width);
				auto y = i / (imageWidth / width);
				animation.AddFrame({ x * width, y * width, width, height });
			}
		}
		else
		{
			for (auto frameNode : framesNode.children())
			{
				auto frameX = static_cast<unsigned int>(std::stoi(frameNode.attribute("x").value()));
				auto frameY = static_cast<unsigned int>(std::stoi(frameNode.attribute("y").value()));
				animation.AddFrame({ frameX, frameY, width, height });
			}
		}
		return std::make_pair(name, animation);
	}

	pugi::xml_node AnimationParser::GetAnimationsNode(pugi::xml_document& doc, const Nz::String& filepath)
	{
		pugi::xml_parse_result result = doc.load_file(filepath.GetConstBuffer());
		if (!result)
		{
			NazaraError("XML [" + filepath + "] could not be opened" + "] due to: " + result.description());
			return {};
		}

		return doc.child("animations");
	}

}
