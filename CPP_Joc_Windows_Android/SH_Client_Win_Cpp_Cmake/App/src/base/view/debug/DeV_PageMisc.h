#pragma once

#include <base/gh.h>
#include "DeV_PageHardcodedEntries.h"

namespace base {
    class IThreadsManager;
};

namespace base {
class DeV_PageMisc : public DeV_PageHardcodedEntries {priv typedef DeV_PageHardcodedEntries super;pub dCtor(DeV_PageMisc);
    priv class DT_FrameMinMax {
        pub std::shared_ptr<base::WrappedValue<int>> value_frame = nullptr;
        pub std::shared_ptr<base::WrappedValue<int>> value_min = nullptr;
        pub std::shared_ptr<base::WrappedValue<int>> value_max = nullptr;

        pub int monitoredVal_min = 0;
        pub int monitoredVal_max = 0;

        pub bool isDirty_monitoredVal = false;
    };

    priv int cPageIndex = -1;

    priv base::IThreadsManager* threadsManager = nullptr;

    priv bool isMinMaxCumulativeClearRequested = true;

    priv std::shared_ptr<base::IDebugEntryUser> dynamicDebugEntry_threads = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dynamicDebugEntry_threads_managedThreadsCount = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dynamicDebugEntry_threads_nativeThreadsCount = nullptr;

    priv std::shared_ptr<base::IDebugEntryUser> dde_memInfoGeneral = nullptr;
    //
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_ramMB = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_activeAllocationsMB = nullptr;
    //
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_activesCount_voidPointers = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_activesCount_objects = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_activesCount_spObjects = nullptr;
    //
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_functionCallsCount_malloc = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_functionCallsCount_realloc = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_functionCallsCount_free = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_functionCallsCount_new = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_functionCallsCount_delete = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_functionCallsCount_newSP = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_functionCallsCount_deleteSP = nullptr;
    //
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_cumulativeMB_malloc = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_cumulativeMB_realloc = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_cumulativeMB_free = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_cumulativeMB_new = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_memInfoGeneral_cumulativeMB_delete = nullptr;

    priv std::shared_ptr<base::IDebugEntryUser> dde_physics3DDebugRendering = nullptr;
    priv std::shared_ptr<base::IDebugEntryUser> dde_physics3DDebugClearDepthBuffer = nullptr;

    priv std::shared_ptr<base::IDebugEntryUser> dde_renderTriangles = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_renderTriangles_2D_frame = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_renderTriangles_2D_min = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_renderTriangles_2D_max = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_renderTriangles_3D_frame = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_renderTriangles_3D_min = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_renderTriangles_3D_max = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_renderTriangles_Special_frame = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_renderTriangles_Special_min = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_renderTriangles_Special_max = nullptr;

    bool isDirty_monitoredVal_triangles = false;
    priv int monitoredVal_triangles_2D_min = 0;
    priv int monitoredVal_triangles_2D_max = 0;
    priv int monitoredVal_triangles_3D_min = 0;
    priv int monitoredVal_triangles_3D_max = 0;
    priv int monitoredVal_triangles_Special_min = 0;
    priv int monitoredVal_triangles_Special_max = 0;

    priv std::shared_ptr<base::IDebugEntryUser> dde_renderLines = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_renderLines_Special_frame = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_renderLines_Special_min = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_renderLines_Special_max = nullptr;

    bool isDirty_monitoredVal_lines = false;
    priv int monitoredVal_lines_Special_min = 0;
    priv int monitoredVal_lines_Special_max = 0;

    priv std::shared_ptr<base::IDebugEntryUser> dde_glUploadCount = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_glUploadCount_Buffer_frame = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_glUploadCount_Buffer_min = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_glUploadCount_Buffer_max = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_glUploadCount_Texture_frame = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_glUploadCount_Texture_min = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_glUploadCount_Texture_max = nullptr;

    bool isDirty_monitoredVal_glUploadCount = false;
    priv int monitoredVal_glUploadCount_Buffer_min = 0;
    priv int monitoredVal_glUploadCount_Buffer_max = 0;
    priv int monitoredVal_glUploadCount_Texture_min = 0;
    priv int monitoredVal_glUploadCount_Texture_max = 0;

    priv std::shared_ptr<base::IDebugEntryUser> dde_glUploadBytes = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_glUploadBytes_Buffer_frame = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_glUploadBytes_Buffer_min = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_glUploadBytes_Buffer_max = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_glUploadBytes_Texture_frame = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_glUploadBytes_Texture_min = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_glUploadBytes_Texture_max = nullptr;

    bool isDirty_monitoredVal_glUploadBytes = false;
    priv int monitoredVal_glUploadBytes_Buffer_min = 0;
    priv int monitoredVal_glUploadBytes_Buffer_max = 0;
    priv int monitoredVal_glUploadBytes_Texture_min = 0;
    priv int monitoredVal_glUploadBytes_Texture_max = 0;

    priv std::shared_ptr<base::IDebugEntryUser> dde_shaderUniforms = nullptr;
    priv DT_FrameMinMax dde_shaderUniforms_SetCalls{};
    priv DT_FrameMinMax dde_shaderUniforms_BytesCount{};

    priv std::shared_ptr<base::IDebugEntryUser> dde_GLResources = nullptr;
    priv DT_FrameMinMax dde_GLResources_VRAM{};
    priv std::shared_ptr<base::WrappedValue<int>> dde_GLResources_shaderProgramssCount = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_GLResources_buffersCount = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_GLResources_texturesCount = nullptr;

    priv std::shared_ptr<base::IDebugEntryUser> dde_glCreateCalls = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_glCreateCalls_shaderProgramssCreateCalls = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_glCreateCalls_buffersCreateCalls = nullptr;
    priv std::shared_ptr<base::WrappedValue<int>> dde_glCreateCalls_texturesCreateCalls = nullptr;
    
    priv struct Entry_Textures {
        pub std::shared_ptr<base::IDebugEntryUser> dde = nullptr;
        pub std::shared_ptr<base::WrappedValue<int>> count = nullptr;
        pub std::shared_ptr<base::WrappedValue<int>> ramMB = nullptr;
        pub std::shared_ptr<base::WrappedValue<int>> vramMB = nullptr;
    } entryTextures;

    pub explicit DeV_PageMisc(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList);
    prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

    priv void onPhysics3DDebugRenderingToggleSwitchChange(IDebugEntryUser* debugEntryUser, bool newState);

    prot void visualLoop(float deltaS, int deltaMS) override;
    
    prot void clearMinMaxCumulative() override;

    prot void disposeMainPre() override;
    pub ~DeV_PageMisc() override;
};
};
