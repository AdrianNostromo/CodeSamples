#pragma once

#include <base/gh.h>
#include <string>

class AssetType {pub dCtor(AssetType);
    // Textures create an Image dependency and are uploaded to VRam directly.
	pub static const AssetType* Texture;
	pub static const AssetType* TextureAtlas;
	pub static const AssetType* GlyphsPacker;
	pub static const AssetType* Text;
	pub static const AssetType* BinaryData;
	pub static const AssetType* Image;

	pub static const AssetType* Font;
	// Containers don't have data.
	// Are used for easy assets groups unloading (eg: all scenario assets can be loader in a group and unloading the group unloads all sub_assets ass needed).
	pub static const AssetType* Container_Font;

	pub static const AssetType* Sound;
	pub static const AssetType* Container_Sound;

	pub int id;
	pub std::string name;

	pub explicit AssetType(int id, std::string name);

	pub ~AssetType();
};
