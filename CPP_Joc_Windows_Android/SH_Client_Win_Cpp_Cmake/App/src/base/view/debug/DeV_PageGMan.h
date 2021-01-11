#pragma once

#include <base/gh.h>
#include "DeV_PageMisc.h"
#include <base/list/ArrayList.h>

namespace base {
class DeV_PageGMan : public DeV_PageMisc {priv typedef DeV_PageMisc super;pub dCtor(DeV_PageGMan);
    priv class SingleCRendData{
        pub class DT_vmT {
            pub int vertices;
            
            pub int modelTransforms;

            pub void set_v(int vertices) { this->vertices = vertices; };
            pub void set_mT(int modelTransforms) { this->modelTransforms = modelTransforms; };
        };
        pub class DT_vimTrMT {
            pub int vertices;
            pub int indices;
            
            pub int modelTransforms;
            
            pub int remapingModelTransforms;

            pub void set_vi(int vertices, int indices) { this->vertices = vertices; this->indices = indices; };
            pub void set_mTrMT(int modelTransforms, int remapingModelTransforms) { this->modelTransforms = modelTransforms; this->remapingModelTransforms = remapingModelTransforms; };
        };
        pub class DT_vimTmNTrMT {
            pub int vertices;
            pub int indices;
            
            pub int modelTransforms;
            pub int modelNormalTransforms;
            
            pub int remapingModelTransforms;

            pub void set_vi(int vertices, int indices) { this->vertices = vertices; this->indices = indices; };
            pub void set_mTmNTrMT(int modelTransforms, int modelNormalTransforms, int remapingModelTransforms) { this->modelTransforms = modelTransforms; this->modelNormalTransforms = modelNormalTransforms; this->remapingModelTransforms = remapingModelTransforms; };
        };

        pub DT_vimTrMT entriesCount;

        pub DT_vimTrMT fillCount;
        pub DT_vimTrMT fillPercent;
        pub DT_vmT fragmentationCount;
        pub DT_vmT fragmentationPercent;

        pub DT_vimTmNTrMT bufferSubDataCalls;
        pub DT_vimTmNTrMT uploadEntries;
        pub DT_vimTmNTrMT uploadBytes;

        pub DT_vmT defragmentationEntriesChanged;

    };

    priv class LogData_CombinedRenderables {
        pub std::shared_ptr<base::IDebugEntryUser> dde_entriesCount = nullptr;

        pub std::shared_ptr<base::IDebugEntryUser> dde_fill = nullptr;
        pub std::shared_ptr<base::IDebugEntryUser> dde_fragmentation = nullptr;

        pub std::shared_ptr<base::IDebugEntryUser> dde_bufferSubDataCalls = nullptr;
        pub std::shared_ptr<base::IDebugEntryUser> dde_uploadEntries = nullptr;
        pub std::shared_ptr<base::IDebugEntryUser> dde_uploadBytes = nullptr;

        pub std::shared_ptr<base::IDebugEntryUser> dde_defragmentationEntriesChanged = nullptr;

        pub ArrayList<SingleCRendData> datasList{};

    };

    priv int cPageIndex = -1;

    priv bool isMinMaxCumulativeClearRequested = true;

    priv LogData_CombinedRenderables dtCRends{};

    priv int oldCRendsCount = 0;

    pub explicit DeV_PageGMan(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList);
    prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

    prot void visualLoop(float deltaS, int deltaMS) override;

    prot void clearMinMaxCumulative() override;

    prot void disposeMainPre() override;
    pub ~DeV_PageGMan() override;
};
};
