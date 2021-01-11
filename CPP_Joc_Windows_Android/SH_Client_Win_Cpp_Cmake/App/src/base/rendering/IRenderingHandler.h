#pragma once

#include <base/gh.h>

class IStage;
class IRenderableRenderer;

namespace graphics {
	class IEnvironment;
	class IRenderer;
};

class IRenderingHandler {
	pub enum RenderOrderGroup { ROG_3D = 0, ROG_3D_DEBUG = 1, ROG_2D = 2 };
	pub enum RendererProfilerType { RPT_2D, RPT_3D, RPT_SPECIAL};

	pub virtual int addRenderable(
		IStage* stage, graphics::IEnvironment* environment, IRenderableRenderer* renderer, graphics::IRenderer* complexRenderer,
		int renderOrderGroup, RendererProfilerType profilerType) = 0;
	pub virtual void removeRenderableMustExist(int id) = 0;

	pub virtual ~IRenderingHandler() {};
};
