
#include <worldGame3D/gw/sim/GamePartSimExtendable.h>
#include "GWPartInteraction.h"

class GWPartSim : public base::GWPartInteraction {
private: typedef base::GWPartInteraction super;
private:
    base::GamePartSimExtendable* gpSim = nullptr;

public:
    GWPartSim(GWPartSim const&) = delete;
    GWPartSim(GWPartSim &&) = default;
    GWPartSim& operator=(GWPartSim const&) = delete;
    GWPartSim& operator=(GWPartSim &&) = default;

    explicit GWPartSim(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);

	base::IGPSim* getSim() override;
	base::IGPSim* getGPSim() override;

    ~GWPartSim() override;
protected:
    int createComponents() override;
    void createGParts() override;

    void isPlayEnabled_changedEvent() override;

    virtual base::GamePartSimExtendable* newGPSim();

    void disposeMain() override;
};
