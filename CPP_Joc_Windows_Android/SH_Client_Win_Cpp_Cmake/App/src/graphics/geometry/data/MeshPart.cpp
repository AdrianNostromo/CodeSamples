#include "MeshPart.h"

using namespace graphics;

MeshPart::MeshPart(
	Mesh* mesh,
	unsigned int verticesOffset, unsigned verticesCount,
	unsigned int indicesOffset, unsigned indicesCount)
	: super(),
	mesh(mesh), 
	verticesOffset(verticesOffset), verticesCount(verticesCount),
	indicesOffset(indicesOffset), indicesCount(indicesCount)
{
	//void
}

MeshPart::~MeshPart() {
	reservedDisposeIfNeeded();// Note1001
}
