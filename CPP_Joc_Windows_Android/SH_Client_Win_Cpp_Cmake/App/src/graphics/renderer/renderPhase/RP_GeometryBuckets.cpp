#include "RP_GeometryBuckets.h"
#include <base/exceptions/LogicException.h>
#include <graphics/material/Material.h>
#include <graphics/environment/IEnvironment.h>
#include <graphics/camera/Camera.h>
#include <graphics/geometry/data/MeshPart.h>
#include <graphics/geometry/Mesh.h>
#include <graphics/model/INodePart.h>
#include <graphics/model/Node.h>
#include <graphics/geometry/data/VertexBufferObject.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/geometry/vertexAttribute/VertexAttribute.h>
#include <graphics/geometry/data/IndexBufferObject.h>
#include <graphics/shader/Shader.h>
#include <graphics/renderer/renderPhase/util/IRenderable.h>
#include <graphics/renderer/renderablesManager/util/Entry_RenderablesManager.h>
#include <graphics/renderer/IRenderer.h>

using namespace graphics;

RP_GeometryBuckets::RP_GeometryBuckets(int renderOrder, bool useDepthTest)
	: super(renderOrder, useDepthTest)
{
	//void
}

ListDL<GeometryBucketUser>& RP_GeometryBuckets::getGeometryBucketUsers() {
	return geometryBucketUsers;
}

RP_GeometryBuckets::~RP_GeometryBuckets() {
	//void
}
