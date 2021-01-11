#include "RH_List.h"
#include <graphics/renderer/IRenderer.h>
#include <graphics/environment/IEnvironment.h>
#include <graphics/camera/Camera.h>
#include <base/opengl/util/GLUtil.h>
#include <graphics/visual/drawable/IStage.h>
#include <base/rendering/util/IRenderableRenderer.h>

RH_List::WrappedRenderer::WrappedRenderer(
	int instanceId, IStage* stage, graphics::IEnvironment* environment, IRenderableRenderer* renderer, graphics::IRenderer* complexRenderer,
	int renderOrderGroup, RendererProfilerType profilerType)
	: instanceId(instanceId),
	stage(stage), environment(environment), 
	renderer(renderer), complexRenderer(complexRenderer),
	renderOrderGroup(renderOrderGroup), profilerType(profilerType)
{
	//void
}

RH_List::RH_List()
        : super()
{
    //void
}

void RH_List::render() {
	if (GLUtil::gl->isProfilerEnabled()) {
		GLUtil::gl->getGlobalsProfiler()->clear();
		GLUtil::gl->getProfiler2D()->clear();
		GLUtil::gl->getProfiler3D()->clear();
		GLUtil::gl->getProfilerSpecial()->clear();
	}

	renderablesList.enumerateDirect([](ListDL<std::shared_ptr<WrappedRenderer>>::Entry* listEntry, std::shared_ptr<WrappedRenderer>& wrappedRenderer, bool& stopEnumeration) ->void {
		if (wrappedRenderer->profilerType == RendererProfilerType::RPT_2D) {
			if (GLUtil::gl->isProfilerEnabled()) {
				GLUtil::gl->setActiveProfiler2D();
			}
		} else if (wrappedRenderer->profilerType == RendererProfilerType::RPT_3D) {
			if (GLUtil::gl->isProfilerEnabled()) {
				GLUtil::gl->setActiveProfiler3D();
			}
		} else if (wrappedRenderer->profilerType == RendererProfilerType::RPT_SPECIAL) {
			if (GLUtil::gl->isProfilerEnabled()) {
				GLUtil::gl->setActiveProfilerSpecial();
			}
		} else {
			throw LogicException(LOC);
		}

		if (wrappedRenderer->stage != nullptr) {
			if (wrappedRenderer->complexRenderer == nullptr) {
				// Stage sync requires the complex renderer.
				throw LogicException(LOC);
			}
			int r = wrappedRenderer->stage->syncStageDrawSpecs(0, *wrappedRenderer->complexRenderer);
			if (r != 0) {
				throw LogicException(LOC);
			}
		}

		if (wrappedRenderer->environment != nullptr) {
			wrappedRenderer->environment->syncIfNeeded();
		}

		// This is used to update the gl buffers.
		wrappedRenderer->renderer->prepareForRendering(wrappedRenderer->environment);
	});

	renderablesList.enumerateDirect([](ListDL<std::shared_ptr<WrappedRenderer>>::Entry* listEntry, std::shared_ptr<WrappedRenderer>& wrappedRenderer, bool& stopEnumeration) ->void {
		if (wrappedRenderer->profilerType == RendererProfilerType::RPT_2D) {
			if (GLUtil::gl->isProfilerEnabled()) {
				GLUtil::gl->setActiveProfiler2D();
			}
		} else if (wrappedRenderer->profilerType == RendererProfilerType::RPT_3D) {
			if (GLUtil::gl->isProfilerEnabled()) {
				GLUtil::gl->setActiveProfiler3D();
			}
		} else if (wrappedRenderer->profilerType == RendererProfilerType::RPT_SPECIAL) {
			if (GLUtil::gl->isProfilerEnabled()) {
				GLUtil::gl->setActiveProfilerSpecial();
			}
		} else {
			throw LogicException(LOC);
		}

		wrappedRenderer->renderer->render(wrappedRenderer->environment);
	});
}

int RH_List::addRenderable(
	IStage* stage, graphics::IEnvironment* environment, IRenderableRenderer* renderer, graphics::IRenderer* complexRenderer,
	int renderOrderGroup, RendererProfilerType profilerType)
{
	std::shared_ptr<WrappedRenderer> wrappedRenderer = std::make_shared<WrappedRenderer>(
		instanceIdCounter++, stage, environment, renderer, complexRenderer,
		renderOrderGroup, profilerType);
    
	bool isInserted = false;
	for (ListDL<std::shared_ptr<WrappedRenderer>>::Entry* entry = renderablesList.getFirst(); entry != nullptr; entry = entry->next) {
		if (renderOrderGroup < entry->data->renderOrderGroup) {
			wrappedRenderer->inHandlerListEntry = renderablesList.insertBeforeDirect(entry, wrappedRenderer);

			isInserted = true;

			break;
		}
	}
	if (!isInserted) {
		wrappedRenderer->inHandlerListEntry = renderablesList.appendDirect(wrappedRenderer);
	}

	return wrappedRenderer->instanceId;
}

std::shared_ptr<RH_List::WrappedRenderer> RH_List::getRenderable(int instanceId) {
	for (ListDL<std::shared_ptr<WrappedRenderer>>::Entry* entry = renderablesList.getFirst(); entry != nullptr; entry = entry->next) {
		if (entry->data->instanceId == instanceId) {
			return entry->data;
		}
	}

	return nullptr;
}

void RH_List::removeRenderableMustExist(int instanceId) {
	std::shared_ptr<WrappedRenderer> wrappedRenderer = getRenderable(instanceId);
	if (wrappedRenderer == nullptr || wrappedRenderer->inHandlerListEntry == nullptr) {
		throw LogicException(LOC);
	}

	wrappedRenderer->inHandlerListEntry->remove();
	wrappedRenderer->inHandlerListEntry = nullptr;
}
