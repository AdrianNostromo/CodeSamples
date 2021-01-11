#include "ITextureUnit.h"
#include <base/opengl/IGL.h>
#include <base/exceptions/LogicException.h>

using namespace graphics;

int ITextureUnit::TextureBindSlots::temp = 0;

int ITextureUnit::TextureBindSlots::globalTexArray_nearest = 1;
int ITextureUnit::TextureBindSlots::globalTexArray_linear = 2;

int ITextureUnit::TextureBindSlots::shadowDepthMap3D = 3;

int ITextureUnit::TextureBindSlots::globalTexArray_customStartIndex = 4;

const ITextureUnit::TextureFilterMin ITextureUnit::TextureFilterMin::Nearest(std::string("Nearest"), IGL::NEAREST, false);
const ITextureUnit::TextureFilterMin ITextureUnit::TextureFilterMin::Linear(std::string("Linear"), IGL::LINEAR, false);
const ITextureUnit::TextureFilterMin ITextureUnit::TextureFilterMin::NearestMipMapNearest(std::string("NearestMipMapNearest"), IGL::NEAREST_MIPMAP_NEAREST, false);
const ITextureUnit::TextureFilterMin ITextureUnit::TextureFilterMin::LinearMipMapNearest(std::string("LinearMipMapNearest"), IGL::LINEAR_MIPMAP_NEAREST, false);
const ITextureUnit::TextureFilterMin ITextureUnit::TextureFilterMin::NearestMipMapLinear(std::string("NearestMipMapLinear"), IGL::NEAREST_MIPMAP_LINEAR, false);
const ITextureUnit::TextureFilterMin ITextureUnit::TextureFilterMin::LinearMipMapLinear(std::string("LinearMipMapLinear"), IGL::LINEAR_MIPMAP_LINEAR, false);

const ITextureUnit::TextureFilterMag ITextureUnit::TextureFilterMag::Nearest(std::string("Nearest"), IGL::NEAREST, false);
const ITextureUnit::TextureFilterMag ITextureUnit::TextureFilterMag::Linear(std::string("Linear"), IGL::LINEAR, false);

const ITextureUnit::TextureWrap ITextureUnit::TextureWrap::ClampToEdge(std::string("ClampToEdge"), IGL::CLAMP_TO_EDGE);
const ITextureUnit::TextureWrap ITextureUnit::TextureWrap::MirroredRepeat(std::string("MirroredRepeat"), IGL::MIRRORED_REPEAT);
const ITextureUnit::TextureWrap ITextureUnit::TextureWrap::Repeat(std::string("Repeat"), IGL::REPEAT);
const ITextureUnit::TextureWrap ITextureUnit::TextureWrap::ClampToBorder(std::string("ClampToBorder"), IGL::CLAMP_TO_BORDER);

ITextureUnit::TextureFilterMin::TextureFilterMin(std::string _stringId, int _glId, bool _usesMipMapping)
	: stringId(_stringId), glId(_glId), usesMipMapping(_usesMipMapping)
{
	//void
}

bool ITextureUnit::TextureFilterMin::equals(const TextureFilterMin* o) const {
	if (glId != o->glId || usesMipMapping != o->usesMipMapping) {
		return false;
	}

	return true;
}

const ITextureUnit::TextureFilterMin* ITextureUnit::TextureFilterMin::fromString(const std::string& value) {
	if (value == Nearest.stringId) {
		return &Nearest;
	} else if (value == Linear.stringId) {
		return &Linear;
	} else if (value == NearestMipMapNearest.stringId) {
		return &NearestMipMapNearest;
	} else if (value == LinearMipMapNearest.stringId) {
		return &LinearMipMapNearest;
	} else if (value == NearestMipMapLinear.stringId) {
		return &NearestMipMapLinear;
	} else if (value == LinearMipMapLinear.stringId) {
		return &LinearMipMapLinear;
	}

	throw LogicException(LOC);
}

ITextureUnit::TextureFilterMag::TextureFilterMag(std::string _stringId, int _glId, bool _usesMipMapping)
	: stringId(_stringId), glId(_glId), usesMipMapping(_usesMipMapping)
{
	//void
}

bool ITextureUnit::TextureFilterMag::equals(const TextureFilterMag* o) const {
	if (glId != o->glId || usesMipMapping != o->usesMipMapping) {
		return false;
	}

	return true;
}

const ITextureUnit::TextureFilterMag* ITextureUnit::TextureFilterMag::fromString(const std::string& value) {
	if (value == Nearest.stringId) {
		return &Nearest;
	} else if (value == Linear.stringId) {
		return &Linear;
	}

	throw LogicException(LOC);
}

ITextureUnit::TextureWrap::TextureWrap(std::string _stringId, int _glId)
	: stringId(_stringId), glId(_glId) {
	//void
}

bool ITextureUnit::TextureWrap::equals(const TextureWrap* o) const {
	if (glId != o->glId) {
		return false;
	}

	return true;
}

const ITextureUnit::TextureWrap* ITextureUnit::TextureWrap::fromString(const std::string& value) {
	if (value == ClampToEdge.stringId) {
		return &ClampToEdge;
	}

	throw LogicException(LOC);
}
