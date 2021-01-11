#pragma once

#include <base/gh.h>
#include "RH_Base.h"
#include <base/list/ListDL.h>
#include <memory>
#include <base/MM.h>

class IListEntry;
class IStage;
class IRenderableRenderer;

class RH_List : public RH_Base {priv typedef RH_Base super; pub dCtor(RH_List);
	priv class WrappedRenderer {pub dCtor(WrappedRenderer);
	pub int instanceId;

		pub IStage* stage;
		pub graphics::IEnvironment* environment;
		pub IRenderableRenderer* renderer;
		pub graphics::IRenderer* complexRenderer;

		// Can be the same for multiple renderer.
		/// This is mostly used to render 2d over 3d.
		pub int renderOrderGroup;
		pub RendererProfilerType profilerType;

		pub IListEntry* inHandlerListEntry = nullptr;

		pub explicit WrappedRenderer(
			int instanceId, IStage* stage, graphics::IEnvironment* environment, IRenderableRenderer* renderer, graphics::IRenderer* complexRenderer,
			int renderOrderGroup, RendererProfilerType profilerType);
	};

	priv int instanceIdCounter = 0;
	priv ListDL<std::shared_ptr<WrappedRenderer>> renderablesList{LOC};

	pub explicit RH_List();

	pub void render();

	pub int addRenderable(
		IStage* stage, graphics::IEnvironment* environment, IRenderableRenderer* renderer, graphics::IRenderer* complexRenderer,
		int renderOrderGroup, RendererProfilerType profilerType) final;
	priv std::shared_ptr<WrappedRenderer> getRenderable(int instanceId);
	pub void removeRenderableMustExist(int instanceId) final;

};
