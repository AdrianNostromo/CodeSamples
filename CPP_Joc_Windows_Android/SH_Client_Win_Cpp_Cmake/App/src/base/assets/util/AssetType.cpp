#include "AssetType.h"

// Textures create an Image dependency and are uploaded to VRam directly.
const AssetType* AssetType::Texture = new AssetType(0, "Texture");
const AssetType* AssetType::TextureAtlas = new AssetType(1, "TextureAtlas");
const AssetType* AssetType::GlyphsPacker = new AssetType(2, "GlyphsPacker");
const AssetType* AssetType::Text = new AssetType(3, "Text");
const AssetType* AssetType::BinaryData = new AssetType(4, "BinaryData");
const AssetType* AssetType::Image = new AssetType(5, "Image");

const AssetType* AssetType::Font = new AssetType(6, "Font");
// Containers don't have data.
// Are used for easy assets groups unloading (eg: all scenario assets can be loader in a group and unloading the group unloads all sub_assets ass needed).
const AssetType* AssetType::Container_Font = new AssetType(-1, "Container_Font");

const AssetType* AssetType::Sound = new AssetType(7, "Sound");
const AssetType* AssetType::Container_Sound = new AssetType(-2, "Container_Sound");

AssetType::AssetType(int id, std::string name)
	: id(id), name(name)
{
	//void
}

AssetType::~AssetType() {
	//void
}
