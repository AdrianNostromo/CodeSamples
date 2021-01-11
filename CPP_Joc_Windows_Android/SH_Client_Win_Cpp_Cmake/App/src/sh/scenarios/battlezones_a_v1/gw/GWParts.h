#pragma once

#include "GWLBase.h"

namespace battlezones_a_v1 {
class GWParts : public GWLBase {
private: typedef GWLBase super;
private:
	GPOnline* online = nullptr;

public:
    GWParts(GWParts const&) = delete;
    GWParts(GWParts &&) = default;
    GWParts& operator=(GWParts const&) = delete;
    GWParts& operator=(GWParts &&) = default;

    explicit GWParts(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);

    ~GWParts() override;
protected:
	base::GamePartMain* newGPMain() override;
	base::GamePartContent* newGPContent() override;
	base::GPOnline* newGPOnline() override;
	base::GPManager* newGPManager() override;
	base::GamePartSimExtendable* newGPSim() override;
};
};
