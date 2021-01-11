#pragma once

#include <base/list/Array3D.h>
#include <base/visual3d/material/Visual3DModelMaterial.h>
#include "GPManActiveZone.h"

namespace battlezones_a_v1 {
class GPManStarter : public GPManActiveZone {
private: typedef GPManActiveZone super;
public:
    GPManStarter(GPManStarter const&) = delete;
    GPManStarter(GPManStarter &&) = default;
    GPManStarter& operator=(GPManStarter const&) = delete;
    GPManStarter& operator=(GPManStarter &&) = default;

    explicit GPManStarter(base::IGameWorld* gw);

    void gwInitFinished() override;

    ~GPManStarter() override;
private:
	void generateNexusZoneConfig();

};
};
