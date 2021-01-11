#pragma once

#include "GWParts.h"

namespace battlezones_a_v1 {
class GWLocal : public GWParts {
private: typedef GWParts super;
public:
	GWLocal(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);
	void startLevelPlay() override;
    void createPost() override;

    ~GWLocal() override;
protected:
    int createComponents() override;

};
};
