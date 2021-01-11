#include "DeV_PageMisc.h"
#include <base/app/IApp.h>
#include <base/threadsManager/IThreadsManager.h>
#include "entryUser/IDebugEntryUser.h"
#include <base/MM.h>
#include <base/math/Math.h>
#include <base/opengl/util/GLUtil.h>
#include <base/runtimeSettings/IRuntimeSettings.h>
#include <rpg3D/gw/physics3D/IGamePartPhysics3D.h>
#include <base/exceptions/LogicException.h>
#include <base/memory/SharedPointer.h>
#include <base/memory/IMemoryManager.h>
#include <base/memory/util/MemoryStats.h>
#include <graphics/visual/Texture.h>
#include <graphics/visual/TextureArray.h>

using namespace base;

DeV_PageMisc::DeV_PageMisc(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList)
	: super(app, viewItemConfigs, viewEaseDurationsSList)
{
	//void
}

void DeV_PageMisc::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	cPageIndex = registerNewPage(true/*isDefaultPage*/, true/*usesDynamicEntries*/);

	threadsManager = app->getThreadsManager();
	
	{
		dynamicDebugEntry_threads = addDebugEntry(1/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dynamicDebugEntry_threads->setPlaceholderedText(">> Threads; managed:#0#, native:#1#");

		dynamicDebugEntry_threads->appendTrackedDataValue(dynamicDebugEntry_threads_managedThreadsCount = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dynamicDebugEntry_threads->appendTrackedDataValue(dynamicDebugEntry_threads_nativeThreadsCount = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
	}
	{
		dde_memInfoGeneral = addDebugEntry(4/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dde_memInfoGeneral->setPlaceholderedText("\
>> Memory: RAM_MB: #0#, ActiveAllocations_MB: #1#\n\
ActivesCount: void*:#2#, objects:#3#, spObjects:#4#\n\
FunctionCallsCounts: malloc:#5#, realloc:#6#, free:#7#, new:#8#, delete:#9#, newSP:#10#, deleteSP:#11#\n\
Cumulative_MB: malloc:#12#, realloc:#13#, free:#14#, new:#15#, delete:#16#"
		);

		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_ramMB = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_activeAllocationsMB = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));

		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_activesCount_voidPointers = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_activesCount_objects = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_activesCount_spObjects = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));

		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_functionCallsCount_malloc = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_functionCallsCount_realloc = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_functionCallsCount_free = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_functionCallsCount_new = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_functionCallsCount_delete = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_functionCallsCount_newSP = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_functionCallsCount_deleteSP = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));

		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_cumulativeMB_malloc = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_cumulativeMB_realloc = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_cumulativeMB_free = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_cumulativeMB_new = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_memInfoGeneral->appendTrackedDataValue(dde_memInfoGeneral_cumulativeMB_delete = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
	}
	
	{
		dde_physics3DDebugRendering = addDebugEntry(1/*linesCount*/, true/*hasToggleSwitch*/);
		dde_physics3DDebugRendering->setPlaceholderedText(">> Physics 3D debug");
		bool isDebugRender = app->getRuntimeSettings()->getSettingBool(rpg3D::IGamePartPhysics3D::RUNTIME_SETTING_physics3DDebugRender, false);
		dde_physics3DDebugRendering->setToggleSwitchIsOn(isDebugRender);
		dde_physics3DDebugRendering->setCb_onToggleSwitchStateChanged(std::bind(&DeV_PageMisc::onPhysics3DDebugRenderingToggleSwitchChange, this, std::placeholders::_1, std::placeholders::_2));
	}
	{
		dde_physics3DDebugClearDepthBuffer = addDebugEntry(1/*linesCount*/, true/*hasToggleSwitch*/);
		dde_physics3DDebugClearDepthBuffer->setPlaceholderedText(">> Physics 3D debug clear deth buffer");
		bool isDebugRender = app->getRuntimeSettings()->getSettingBool(rpg3D::IGamePartPhysics3D::RUNTIME_SETTING_physics3DDebugRender, false);
		dde_physics3DDebugClearDepthBuffer->setToggleSwitchIsOn(isDebugRender);
		dde_physics3DDebugClearDepthBuffer->setCb_onToggleSwitchStateChanged(std::bind(&DeV_PageMisc::onPhysics3DDebugRenderingToggleSwitchChange, this, std::placeholders::_1, std::placeholders::_2));
	}
	{
		dde_renderTriangles = addDebugEntry(4/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dde_renderTriangles->setPlaceholderedText(">> Triangles:\n2D:#0#, min:#1#, max:#2#\n3D:#3#, min:#4#, max:#5#\nSpecial:#6#, min:#7#, max:#8#");
		
		dde_renderTriangles->appendTrackedDataValue(dde_renderTriangles_2D_frame = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_renderTriangles->appendTrackedDataValue(dde_renderTriangles_2D_min = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_renderTriangles->appendTrackedDataValue(dde_renderTriangles_2D_max = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		
		dde_renderTriangles->appendTrackedDataValue(dde_renderTriangles_3D_frame = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_renderTriangles->appendTrackedDataValue(dde_renderTriangles_3D_min = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_renderTriangles->appendTrackedDataValue(dde_renderTriangles_3D_max = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		
		dde_renderTriangles->appendTrackedDataValue(dde_renderTriangles_Special_frame = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_renderTriangles->appendTrackedDataValue(dde_renderTriangles_Special_min = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_renderTriangles->appendTrackedDataValue(dde_renderTriangles_Special_max = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));

		isDirty_monitoredVal_triangles = true;
	}
	{
		dde_renderLines = addDebugEntry(2/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dde_renderLines->setPlaceholderedText(">> Lines:\nSpecial:#0#, min:#1#, max:#2#");
		
		dde_renderLines->appendTrackedDataValue(dde_renderLines_Special_frame = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_renderLines->appendTrackedDataValue(dde_renderLines_Special_min = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_renderLines->appendTrackedDataValue(dde_renderLines_Special_max = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));

		isDirty_monitoredVal_lines = true;
	}
	{
		dde_glUploadCount = addDebugEntry(3/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dde_glUploadCount->setPlaceholderedText(">> GL_upload count:\nBuffer:#0#, min:#1#, max:#2#\nTexture:#3#, min:#4#, max:#5#");

		dde_glUploadCount->appendTrackedDataValue(dde_glUploadCount_Buffer_frame = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_glUploadCount->appendTrackedDataValue(dde_glUploadCount_Buffer_min = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_glUploadCount->appendTrackedDataValue(dde_glUploadCount_Buffer_max = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));

		dde_glUploadCount->appendTrackedDataValue(dde_glUploadCount_Texture_frame = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_glUploadCount->appendTrackedDataValue(dde_glUploadCount_Texture_min = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_glUploadCount->appendTrackedDataValue(dde_glUploadCount_Texture_max = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));

		isDirty_monitoredVal_glUploadCount = true;
	}
	{
		dde_glUploadBytes = addDebugEntry(3/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dde_glUploadBytes->setPlaceholderedText(">> GL_upload bytes:\nBuffer:#0# KB, min:#1# KB, max:#2# KB\nTexture:#3# KB, min:#4# KB, max:#5# KB");

		dde_glUploadBytes->appendTrackedDataValue(dde_glUploadBytes_Buffer_frame = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_glUploadBytes->appendTrackedDataValue(dde_glUploadBytes_Buffer_min = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_glUploadBytes->appendTrackedDataValue(dde_glUploadBytes_Buffer_max = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));

		dde_glUploadBytes->appendTrackedDataValue(dde_glUploadBytes_Texture_frame = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_glUploadBytes->appendTrackedDataValue(dde_glUploadBytes_Texture_min = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_glUploadBytes->appendTrackedDataValue(dde_glUploadBytes_Texture_max = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));

		isDirty_monitoredVal_glUploadBytes = true;
	}
	{
		dde_shaderUniforms = addDebugEntry(3/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dde_shaderUniforms->setPlaceholderedText(">> Shader_Uniform:\nSet calls:#0#, min:#1#, max:#2#\nData sent:#3# KB, min:#4# KB, max:#5# KB");

		dde_shaderUniforms->appendTrackedDataValue(dde_shaderUniforms_SetCalls.value_frame = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_shaderUniforms->appendTrackedDataValue(dde_shaderUniforms_SetCalls.value_min = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_shaderUniforms->appendTrackedDataValue(dde_shaderUniforms_SetCalls.value_max = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_shaderUniforms_SetCalls.isDirty_monitoredVal = true;

		dde_shaderUniforms->appendTrackedDataValue(dde_shaderUniforms_BytesCount.value_frame = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_shaderUniforms->appendTrackedDataValue(dde_shaderUniforms_BytesCount.value_min = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_shaderUniforms->appendTrackedDataValue(dde_shaderUniforms_BytesCount.value_max = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_shaderUniforms_BytesCount.isDirty_monitoredVal = true;
	}
	{
		dde_GLResources = addDebugEntry(3/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dde_GLResources->setPlaceholderedText(">> GL resources\nVRAM:#0# MB, min:#1# MB, max:#2# MB\nShader programs:#3#, Buffers:#4#, Textures:#5#");

		dde_GLResources->appendTrackedDataValue(dde_GLResources_VRAM.value_frame = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_GLResources->appendTrackedDataValue(dde_GLResources_VRAM.value_min = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_GLResources->appendTrackedDataValue(dde_GLResources_VRAM.value_max = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_GLResources_VRAM.isDirty_monitoredVal = true;

		dde_GLResources->appendTrackedDataValue(dde_GLResources_shaderProgramssCount = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_GLResources->appendTrackedDataValue(dde_GLResources_buffersCount = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_GLResources->appendTrackedDataValue(dde_GLResources_texturesCount = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
	}
	{
		dde_glCreateCalls = addDebugEntry(2/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		dde_glCreateCalls->setPlaceholderedText(">> GL create calls:\nShader programs create:#0#, Buffers create:#1#, Textures create:#2#");

		dde_glCreateCalls->appendTrackedDataValue(dde_glCreateCalls_shaderProgramssCreateCalls = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_glCreateCalls->appendTrackedDataValue(dde_glCreateCalls_buffersCreateCalls = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		dde_glCreateCalls->appendTrackedDataValue(dde_glCreateCalls_texturesCreateCalls = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
	}
	{
		entryTextures.dde = addDebugEntry(2/*linesCount*/, false/*hasToggleSwitch*/, cPageIndex/*pageIndex*/);
		entryTextures.dde->setPlaceholderedText(">> Textures\n count:#0#, ramMB:#1#, vramMB:#2#");

		entryTextures.dde->appendTrackedDataValue(entryTextures.count = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		entryTextures.dde->appendTrackedDataValue(entryTextures.ramMB = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
		entryTextures.dde->appendTrackedDataValue(entryTextures.vramMB = WrappedValue<int>::NewSPDirect(0, base::IWrappedValue::Type::T_int));
	}
}

void DeV_PageMisc::onPhysics3DDebugRenderingToggleSwitchChange(IDebugEntryUser* debugEntryUser, bool newState) {
	if (debugEntryUser == dde_physics3DDebugRendering.get()) {
		app->getRuntimeSettings()->setSetting(rpg3D::IGamePartPhysics3D::RUNTIME_SETTING_physics3DDebugRender, newState);
	} else if (debugEntryUser == dde_physics3DDebugClearDepthBuffer.get()) {
		app->getRuntimeSettings()->setSetting(rpg3D::IGamePartPhysics3D::RUNTIME_SETTING_physics3DDebugClearDepthBuffer, newState);
	} else {
		throw LogicException(LOC);
	}
}

void DeV_PageMisc::visualLoop(float deltaS, int deltaMS) {
	super::visualLoop(deltaS, deltaMS);
	
	if (isActive() && getActivePageIndex() == cPageIndex) {
		if (dynamicDebugEntry_threads_managedThreadsCount->getReference() != threadsManager->getStatistics_managedThreadsCount()) {
			dynamicDebugEntry_threads_managedThreadsCount->getReference() = threadsManager->getStatistics_managedThreadsCount();
			dynamicDebugEntry_threads->invalidateData();
		}
		if (dynamicDebugEntry_threads_nativeThreadsCount->getReference() != threadsManager->getStatistics_nativeThreadsCount()) {
			dynamicDebugEntry_threads_nativeThreadsCount->getReference() = threadsManager->getStatistics_nativeThreadsCount();
			dynamicDebugEntry_threads->invalidateData();
		}
		
		{
			mem::MemoryStats* memStats = mem::MM::mm->getStats();

			int ram_mBytes = Math::floor(memStats->ram_bytes_count / (1024 * 1024));
			if (dde_memInfoGeneral_ramMB->getReference() != ram_mBytes) {
				dde_memInfoGeneral_ramMB->getReference() = ram_mBytes;
				dde_memInfoGeneral->invalidateData();
			}
			int activeAllocations_mBytes = Math::floor(memStats->activeAllocations_bytes_count / (1024 * 1024));
			if (dde_memInfoGeneral_activeAllocationsMB->getReference() != activeAllocations_mBytes) {
				dde_memInfoGeneral_activeAllocationsMB->getReference() = activeAllocations_mBytes;
				dde_memInfoGeneral->invalidateData();
			}
			//
			if (dde_memInfoGeneral_activesCount_voidPointers->getReference() != memStats->activesCount_voidPointers) {
				dde_memInfoGeneral_activesCount_voidPointers->getReference() = memStats->activesCount_voidPointers;
				dde_memInfoGeneral->invalidateData();
			}
			if (dde_memInfoGeneral_activesCount_objects->getReference() != memStats->activesCount_objects) {
				dde_memInfoGeneral_activesCount_objects->getReference() = memStats->activesCount_objects;
				dde_memInfoGeneral->invalidateData();
			}
			if (dde_memInfoGeneral_activesCount_spObjects->getReference() != mem::SharedPointer_Stats::activesCount_spObjects) {
				dde_memInfoGeneral_activesCount_spObjects->getReference() = mem::SharedPointer_Stats::activesCount_spObjects;
				dde_memInfoGeneral->invalidateData();
			}
			//
			if (dde_memInfoGeneral_functionCallsCount_malloc->getReference() != memStats->functionCallsCount_malloc) {
				dde_memInfoGeneral_functionCallsCount_malloc->getReference() = memStats->functionCallsCount_malloc;
				dde_memInfoGeneral->invalidateData();
			}
			if (dde_memInfoGeneral_functionCallsCount_realloc->getReference() != memStats->functionCallsCount_realloc) {
				dde_memInfoGeneral_functionCallsCount_realloc->getReference() = memStats->functionCallsCount_realloc;
				dde_memInfoGeneral->invalidateData();
			}
			if (dde_memInfoGeneral_functionCallsCount_free->getReference() != memStats->functionCallsCount_free) {
				dde_memInfoGeneral_functionCallsCount_free->getReference() = memStats->functionCallsCount_free;
				dde_memInfoGeneral->invalidateData();
			}
			if (dde_memInfoGeneral_functionCallsCount_new->getReference() != memStats->functionCallsCount_new) {
				dde_memInfoGeneral_functionCallsCount_new->getReference() = memStats->functionCallsCount_new;
				dde_memInfoGeneral->invalidateData();
			}
			if (dde_memInfoGeneral_functionCallsCount_delete->getReference() != memStats->functionCallsCount_delete) {
				dde_memInfoGeneral_functionCallsCount_delete->getReference() = memStats->functionCallsCount_delete;
				dde_memInfoGeneral->invalidateData();
			}
			if (dde_memInfoGeneral_functionCallsCount_newSP->getReference() != mem::SharedPointer_Stats::functionCallsCount_newSP) {
				dde_memInfoGeneral_functionCallsCount_newSP->getReference() = mem::SharedPointer_Stats::functionCallsCount_newSP;
				dde_memInfoGeneral->invalidateData();
			}
			if (dde_memInfoGeneral_functionCallsCount_deleteSP->getReference() != mem::SharedPointer_Stats::functionCallsCount_deleteSP) {
				dde_memInfoGeneral_functionCallsCount_deleteSP->getReference() = mem::SharedPointer_Stats::functionCallsCount_deleteSP;
				dde_memInfoGeneral->invalidateData();
			}
			//
			int cumulativeByteChangesMB_malloc = Math::floor(memStats->cumulativeByteChanges_malloc / (1024 * 1024));
			if (dde_memInfoGeneral_cumulativeMB_malloc->getReference() != cumulativeByteChangesMB_malloc) {
				dde_memInfoGeneral_cumulativeMB_malloc->getReference() = cumulativeByteChangesMB_malloc;
				dde_memInfoGeneral->invalidateData();
			}
			// asd_x;// use -1 for the value currently because value is not accessible until the custom memory manager is implemented.
			int cumulativeByteChangesMB_realloc = -1;// asd_x; Math::floor(memStats->cumulativeByteChanges_realoc / (1024 * 1024));
			if (dde_memInfoGeneral_cumulativeMB_realloc->getReference() != cumulativeByteChangesMB_realloc) {
				dde_memInfoGeneral_cumulativeMB_realloc->getReference() = cumulativeByteChangesMB_realloc;
				dde_memInfoGeneral->invalidateData();
			}
			// asd_x;// use -1 for the value currently because value is not accessible until the custom memory manager is implemented.
			if (dde_memInfoGeneral_cumulativeMB_free->getReference() != -1) {
				dde_memInfoGeneral_cumulativeMB_free->getReference() = -1;
				dde_memInfoGeneral->invalidateData();
			}
			int cumulativeByteChangesMB_new = Math::floor(memStats->cumulativeByteChanges_new / (1024 * 1024));
			if (dde_memInfoGeneral_cumulativeMB_new->getReference() != cumulativeByteChangesMB_new) {
				dde_memInfoGeneral_cumulativeMB_new->getReference() = cumulativeByteChangesMB_new;
				dde_memInfoGeneral->invalidateData();
			}
			// asd_x;// use -1 for the value currently because value is not accessible until the custom memory manager is implemented.
			if (dde_memInfoGeneral_cumulativeMB_delete->getReference() != -1) {
				dde_memInfoGeneral_cumulativeMB_delete->getReference() = -1;
				dde_memInfoGeneral->invalidateData();
			}
		}

		{
			if (isDirty_monitoredVal_triangles || isMinMaxCumulativeClearRequested) {
				isDirty_monitoredVal_triangles = false;

				monitoredVal_triangles_2D_min = GLUtil::gl->getProfiler2D()->drawTrianglesCount;
				monitoredVal_triangles_2D_max = GLUtil::gl->getProfiler2D()->drawTrianglesCount;
				monitoredVal_triangles_3D_min = GLUtil::gl->getProfiler3D()->drawTrianglesCount;
				monitoredVal_triangles_3D_max = GLUtil::gl->getProfiler3D()->drawTrianglesCount;
				monitoredVal_triangles_Special_min = GLUtil::gl->getProfilerSpecial()->drawTrianglesCount;
				monitoredVal_triangles_Special_max = GLUtil::gl->getProfilerSpecial()->drawTrianglesCount;
			} else {
				monitoredVal_triangles_2D_min = Math::min(monitoredVal_triangles_2D_min, GLUtil::gl->getProfiler2D()->drawTrianglesCount);
				monitoredVal_triangles_2D_max = Math::max(monitoredVal_triangles_2D_max, GLUtil::gl->getProfiler2D()->drawTrianglesCount);
				monitoredVal_triangles_3D_min = Math::min(monitoredVal_triangles_3D_min, GLUtil::gl->getProfiler3D()->drawTrianglesCount);
				monitoredVal_triangles_3D_max = Math::max(monitoredVal_triangles_3D_max, GLUtil::gl->getProfiler3D()->drawTrianglesCount);
				monitoredVal_triangles_Special_min = Math::min(monitoredVal_triangles_Special_min, GLUtil::gl->getProfilerSpecial()->drawTrianglesCount);
				monitoredVal_triangles_Special_max = Math::max(monitoredVal_triangles_Special_max, GLUtil::gl->getProfilerSpecial()->drawTrianglesCount);
			}

			bool isChange = false;

			CheckedSetDebugEntry(dde_renderTriangles_2D_frame, GLUtil::gl->getProfiler2D()->drawTrianglesCount, isChange);
			CheckedSetDebugEntry(dde_renderTriangles_2D_min, monitoredVal_triangles_2D_min, isChange);
			CheckedSetDebugEntry(dde_renderTriangles_2D_max, monitoredVal_triangles_2D_max, isChange);

			CheckedSetDebugEntry(dde_renderTriangles_3D_frame, GLUtil::gl->getProfiler3D()->drawTrianglesCount, isChange);
			CheckedSetDebugEntry(dde_renderTriangles_3D_min, monitoredVal_triangles_3D_min, isChange);
			CheckedSetDebugEntry(dde_renderTriangles_3D_max, monitoredVal_triangles_3D_max, isChange);

			CheckedSetDebugEntry(dde_renderTriangles_Special_frame, GLUtil::gl->getProfilerSpecial()->drawTrianglesCount, isChange);
			CheckedSetDebugEntry(dde_renderTriangles_Special_min, monitoredVal_triangles_Special_min, isChange);
			CheckedSetDebugEntry(dde_renderTriangles_Special_max, monitoredVal_triangles_Special_max, isChange);

			if (isChange) {
				dde_renderTriangles->invalidateData();
			}
		}
		{
			if (isDirty_monitoredVal_lines || isMinMaxCumulativeClearRequested) {
				isDirty_monitoredVal_lines = false;

				monitoredVal_lines_Special_min = GLUtil::gl->getProfilerSpecial()->drawLinesCount;
				monitoredVal_lines_Special_max = GLUtil::gl->getProfilerSpecial()->drawLinesCount;
			} else {
				monitoredVal_lines_Special_min = Math::min(monitoredVal_lines_Special_min, GLUtil::gl->getProfilerSpecial()->drawLinesCount);
				monitoredVal_lines_Special_max = Math::max(monitoredVal_lines_Special_max, GLUtil::gl->getProfilerSpecial()->drawLinesCount);
			}

			bool isChange = false;

			CheckedSetDebugEntry(dde_renderLines_Special_frame, GLUtil::gl->getProfilerSpecial()->drawLinesCount, isChange);
			CheckedSetDebugEntry(dde_renderLines_Special_min, monitoredVal_lines_Special_min, isChange);
			CheckedSetDebugEntry(dde_renderLines_Special_max, monitoredVal_lines_Special_max, isChange);

			if (isChange) {
				dde_renderLines->invalidateData();
			}
		}
		{
			int bufferUploadCount = GLUtil::gl->getGlobalsProfiler()->bufferUploadCount;
			int textureUploadCount = GLUtil::gl->getGlobalsProfiler()->textureUploadCount;

			if (isDirty_monitoredVal_glUploadCount || isMinMaxCumulativeClearRequested) {
				isDirty_monitoredVal_glUploadCount = false;

				monitoredVal_glUploadCount_Buffer_min = bufferUploadCount;
				monitoredVal_glUploadCount_Buffer_max = bufferUploadCount;
				monitoredVal_glUploadCount_Texture_min = textureUploadCount;
				monitoredVal_glUploadCount_Texture_max = textureUploadCount;
			} else {
				monitoredVal_glUploadCount_Buffer_min = Math::min(monitoredVal_glUploadCount_Buffer_min, bufferUploadCount);
				monitoredVal_glUploadCount_Buffer_max = Math::max(monitoredVal_glUploadCount_Buffer_max, bufferUploadCount);
				monitoredVal_glUploadCount_Texture_min = Math::min(monitoredVal_glUploadCount_Texture_min, textureUploadCount);
				monitoredVal_glUploadCount_Texture_max = Math::max(monitoredVal_glUploadCount_Texture_max, textureUploadCount);
			}

			bool isChange = false;

			CheckedSetDebugEntry(dde_glUploadCount_Buffer_frame, bufferUploadCount, isChange);
			CheckedSetDebugEntry(dde_glUploadCount_Buffer_min, monitoredVal_glUploadCount_Buffer_min, isChange);
			CheckedSetDebugEntry(dde_glUploadCount_Buffer_max, monitoredVal_glUploadCount_Buffer_max, isChange);

			CheckedSetDebugEntry(dde_glUploadCount_Texture_frame, textureUploadCount, isChange);
			CheckedSetDebugEntry(dde_glUploadCount_Texture_min, monitoredVal_glUploadCount_Texture_min, isChange);
			CheckedSetDebugEntry(dde_glUploadCount_Texture_max, monitoredVal_glUploadCount_Texture_max, isChange);

			if (isChange) {
				dde_glUploadCount->invalidateData();
			}
		}
		{
			int bufferUploadKB = Math::floor(GLUtil::gl->getGlobalsProfiler()->bufferUploadBytes / 1000);
			int textureUploadKB = Math::floor(GLUtil::gl->getGlobalsProfiler()->textureUploadBytes / 1000);

			if (isDirty_monitoredVal_glUploadBytes || isMinMaxCumulativeClearRequested) {
				isDirty_monitoredVal_glUploadBytes = false;

				monitoredVal_glUploadBytes_Buffer_min = bufferUploadKB;
				monitoredVal_glUploadBytes_Buffer_max = bufferUploadKB;
				monitoredVal_glUploadBytes_Texture_min = textureUploadKB;
				monitoredVal_glUploadBytes_Texture_max = textureUploadKB;
			} else {
				monitoredVal_glUploadBytes_Buffer_min = Math::min(monitoredVal_glUploadBytes_Buffer_min, bufferUploadKB);
				monitoredVal_glUploadBytes_Buffer_max = Math::max(monitoredVal_glUploadBytes_Buffer_max, bufferUploadKB);
				monitoredVal_glUploadBytes_Texture_min = Math::min(monitoredVal_glUploadBytes_Texture_min, textureUploadKB);
				monitoredVal_glUploadBytes_Texture_max = Math::max(monitoredVal_glUploadBytes_Texture_max, textureUploadKB);
			}

			bool isChange = false;

			CheckedSetDebugEntry(dde_glUploadBytes_Buffer_frame, bufferUploadKB, isChange);
			CheckedSetDebugEntry(dde_glUploadBytes_Buffer_min, monitoredVal_glUploadBytes_Buffer_min, isChange);
			CheckedSetDebugEntry(dde_glUploadBytes_Buffer_max, monitoredVal_glUploadBytes_Buffer_max, isChange);

			CheckedSetDebugEntry(dde_glUploadBytes_Texture_frame, textureUploadKB, isChange);
			CheckedSetDebugEntry(dde_glUploadBytes_Texture_min, monitoredVal_glUploadBytes_Texture_min, isChange);
			CheckedSetDebugEntry(dde_glUploadBytes_Texture_max, monitoredVal_glUploadBytes_Texture_max, isChange);

			if (isChange) {
				dde_glUploadBytes->invalidateData();
			}
		}
		{
			bool isChange = false;

			if (dde_shaderUniforms_SetCalls.isDirty_monitoredVal || isMinMaxCumulativeClearRequested) {
				dde_shaderUniforms_SetCalls.isDirty_monitoredVal = false;

				dde_shaderUniforms_SetCalls.monitoredVal_min = GLUtil::gl->getGlobalsProfiler()->shaderUniformSetCallsCount;
				dde_shaderUniforms_SetCalls.monitoredVal_max = GLUtil::gl->getGlobalsProfiler()->shaderUniformSetCallsCount;
			} else {
				dde_shaderUniforms_SetCalls.monitoredVal_min = Math::min(dde_shaderUniforms_SetCalls.monitoredVal_min, GLUtil::gl->getGlobalsProfiler()->shaderUniformSetCallsCount);
				dde_shaderUniforms_SetCalls.monitoredVal_max = Math::max(dde_shaderUniforms_SetCalls.monitoredVal_max, GLUtil::gl->getGlobalsProfiler()->shaderUniformSetCallsCount);
			}
			CheckedSetDebugEntry(dde_shaderUniforms_SetCalls.value_frame, GLUtil::gl->getGlobalsProfiler()->shaderUniformSetCallsCount, isChange);
			CheckedSetDebugEntry(dde_shaderUniforms_SetCalls.value_min, dde_shaderUniforms_SetCalls.monitoredVal_min, isChange);
			CheckedSetDebugEntry(dde_shaderUniforms_SetCalls.value_max, dde_shaderUniforms_SetCalls.monitoredVal_max, isChange);

			int shaderUniformSetKB = Math::floor(GLUtil::gl->getGlobalsProfiler()->shaderUniformSetBytesCount / 1000);
			if (dde_shaderUniforms_BytesCount.isDirty_monitoredVal || isMinMaxCumulativeClearRequested) {
				dde_shaderUniforms_BytesCount.isDirty_monitoredVal = false;

				dde_shaderUniforms_BytesCount.monitoredVal_min = shaderUniformSetKB;
				dde_shaderUniforms_BytesCount.monitoredVal_max = shaderUniformSetKB;
			} else {
				dde_shaderUniforms_BytesCount.monitoredVal_min = Math::min(dde_shaderUniforms_BytesCount.monitoredVal_min, shaderUniformSetKB);
				dde_shaderUniforms_BytesCount.monitoredVal_max = Math::max(dde_shaderUniforms_BytesCount.monitoredVal_max, shaderUniformSetKB);
			}
			CheckedSetDebugEntry(dde_shaderUniforms_BytesCount.value_frame, shaderUniformSetKB, isChange);
			CheckedSetDebugEntry(dde_shaderUniforms_BytesCount.value_min, dde_shaderUniforms_BytesCount.monitoredVal_min, isChange);
			CheckedSetDebugEntry(dde_shaderUniforms_BytesCount.value_max, dde_shaderUniforms_BytesCount.monitoredVal_max, isChange);

			if (isChange) {
				dde_shaderUniforms->invalidateData();
			}
		}
		{
			bool isChange = false;

			int vram_MB = Math::floor(GLUtil::gl->getGlobalsCumulativeProfiler()->vram_bytes_cumulative / (1024 * 1024));
			if (dde_GLResources_VRAM.isDirty_monitoredVal || isMinMaxCumulativeClearRequested) {
				dde_GLResources_VRAM.isDirty_monitoredVal = false;

				dde_GLResources_VRAM.monitoredVal_min = vram_MB;
				dde_GLResources_VRAM.monitoredVal_max = vram_MB;
			} else {
				dde_GLResources_VRAM.monitoredVal_min = Math::min(dde_GLResources_VRAM.monitoredVal_min, vram_MB);
				dde_GLResources_VRAM.monitoredVal_max = Math::max(dde_GLResources_VRAM.monitoredVal_max, vram_MB);
			}
			CheckedSetDebugEntry(dde_GLResources_VRAM.value_frame, vram_MB, isChange);
			CheckedSetDebugEntry(dde_GLResources_VRAM.value_min, dde_GLResources_VRAM.monitoredVal_min, isChange);
			CheckedSetDebugEntry(dde_GLResources_VRAM.value_max, dde_GLResources_VRAM.monitoredVal_max, isChange);

			CheckedSetDebugEntry(dde_GLResources_shaderProgramssCount, GLUtil::gl->getGlobalsCumulativeProfiler()->shaderProgramsCount_cumulative, isChange);
			CheckedSetDebugEntry(dde_GLResources_buffersCount, GLUtil::gl->getGlobalsCumulativeProfiler()->buffersCount_cumulative, isChange);
			CheckedSetDebugEntry(dde_GLResources_texturesCount, GLUtil::gl->getGlobalsCumulativeProfiler()->texturesCount_cumulative, isChange);

			if (isChange) {
				dde_GLResources->invalidateData();
			}
		}
		{
			bool isChange = false;

			CheckedSetDebugEntry(dde_glCreateCalls_shaderProgramssCreateCalls, GLUtil::gl->getGlobalsCumulativeProfiler()->createCalls_shaderPrograms, isChange);
			CheckedSetDebugEntry(dde_glCreateCalls_buffersCreateCalls, GLUtil::gl->getGlobalsCumulativeProfiler()->createCalls_buffers, isChange);
			CheckedSetDebugEntry(dde_glCreateCalls_texturesCreateCalls, GLUtil::gl->getGlobalsCumulativeProfiler()->createCalls_textures, isChange);

			if (isChange) {
				dde_glCreateCalls->invalidateData();
			}
		}
		{
			bool isChange = false;

			int texturesRam_MB = Math::floor(Texture::Stats::ramBytes / (1024 * 1024));
			// Texture array vram value is also used because texture_array textures don't allocate vram.
			int texturesVram_MB = Math::floor((Texture::Stats::vramBytes + graphics::TextureArray::Stats::vramBytes) / (1024 * 1024));

			CheckedSetDebugEntry(entryTextures.count, Texture::Stats::texturesCount, isChange);
			CheckedSetDebugEntry(entryTextures.ramMB, texturesRam_MB, isChange);
			CheckedSetDebugEntry(entryTextures.vramMB, texturesVram_MB, isChange);

			if (isChange) {
				entryTextures.dde->invalidateData();
			}
		}

		if (isMinMaxCumulativeClearRequested) {
			isMinMaxCumulativeClearRequested = false;
		}
	}
}

void DeV_PageMisc::clearMinMaxCumulative() {
	super::clearMinMaxCumulative();
	
	isMinMaxCumulativeClearRequested = true;
}

void DeV_PageMisc::disposeMainPre() {
	if (dynamicDebugEntry_threads != nullptr) {
		dynamicDebugEntry_threads->remove();
		dynamicDebugEntry_threads = nullptr;
	}
	if (dde_memInfoGeneral != nullptr) {
		dde_memInfoGeneral->remove();
		dde_memInfoGeneral = nullptr;
	}
	if (dde_physics3DDebugRendering != nullptr) {
		dde_physics3DDebugRendering->remove();
		dde_physics3DDebugRendering = nullptr;
	}
	if (dde_physics3DDebugClearDepthBuffer != nullptr) {
		dde_physics3DDebugClearDepthBuffer->remove();
		dde_physics3DDebugClearDepthBuffer = nullptr;
	}
	if (dde_renderTriangles != nullptr) {
		dde_renderTriangles->remove();
		dde_renderTriangles = nullptr;
	}
	if (dde_renderLines != nullptr) {
		dde_renderLines->remove();
		dde_renderLines = nullptr;
	}
	if (dde_glUploadCount != nullptr) {
		dde_glUploadCount->remove();
		dde_glUploadCount = nullptr;
	}
	if (dde_glUploadBytes != nullptr) {
		dde_glUploadBytes->remove();
		dde_glUploadBytes = nullptr;
	}
	if (dde_shaderUniforms != nullptr) {
		dde_shaderUniforms->remove();
		dde_shaderUniforms = nullptr;
	}
	if (dde_GLResources != nullptr) {
		dde_GLResources->remove();
		dde_GLResources = nullptr;
	}
	if (dde_glCreateCalls != nullptr) {
		dde_glCreateCalls->remove();
		dde_glCreateCalls = nullptr;
	}
	if (entryTextures.dde != nullptr) {
		entryTextures.dde->remove();
		entryTextures.dde = nullptr;
	}

	super::disposeMainPre();
}

DeV_PageMisc::~DeV_PageMisc() {
	//void
}
