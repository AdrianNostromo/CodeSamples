#pragma once

#include <base/gh.h>
#include <string>

namespace graphics {
class ITextureUnit {
	// This class contains the bind locations.
	pub class TextureBindSlots {
		pub static int temp;
		
		pub static int globalTexArray_nearest;
		pub static int globalTexArray_linear;

		pub static int shadowDepthMap3D;
		
		pub static int globalTexArray_customStartIndex;
	};

	pub class TextureFilterMin {
        pub static const TextureFilterMin Nearest;
		pub static const TextureFilterMin Linear;
		pub static const TextureFilterMin NearestMipMapNearest;
		pub static const TextureFilterMin LinearMipMapNearest;
		pub static const TextureFilterMin NearestMipMapLinear;
		pub static const TextureFilterMin LinearMipMapLinear;
		
		pub const std::string stringId;
		pub const int glId;
		pub bool usesMipMapping;
	
		pub TextureFilterMin(std::string _stringId, int _glId, bool _usesMipMapping);

		pub bool equals(const TextureFilterMin* o) const;

		pub static const TextureFilterMin* fromString(const std::string& value);
	};
	pub class TextureFilterMag {
        pub static const TextureFilterMag Nearest;
		pub static const TextureFilterMag Linear;

		pub const std::string stringId;
		pub const int glId;
		pub bool usesMipMapping;
	
		pub TextureFilterMag(std::string _stringId, int _glId, bool _usesMipMapping);

		pub bool equals(const TextureFilterMag* o) const;

		pub static const TextureFilterMag* fromString(const std::string& value);
	};

	pub class TextureWrap {
		pub static const TextureWrap ClampToEdge;
		pub static const TextureWrap MirroredRepeat;
		pub static const TextureWrap Repeat;
		pub static const TextureWrap ClampToBorder;

		pub const std::string stringId;
		pub const int glId;
	
		pub TextureWrap(std::string _stringId, int _glId);

		pub bool equals(const TextureWrap* o) const;

		pub static const TextureWrap* fromString(const std::string& value);
	};

	pub virtual ~ITextureUnit() = default;
};
};
