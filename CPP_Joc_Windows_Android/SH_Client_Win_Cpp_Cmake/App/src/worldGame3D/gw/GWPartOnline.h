
#include <worldGame3D/gw/online/GPOnline.h>
#include "GWPartSim.h"

class GWPartOnline : public GWPartSim {
private: typedef GWPartSim super;
private:
    base::GPOnline* gpOnline = nullptr;

public:
    GWPartOnline(GWPartOnline const&) = delete;
    GWPartOnline(GWPartOnline &&) = default;
    GWPartOnline& operator=(GWPartOnline const&) = delete;
    GWPartOnline& operator=(GWPartOnline &&) = default;

    explicit GWPartOnline(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);

    base::IGPOnline* getOnline() override;
    base::IGPOnline* getGPOnline() override;

	~GWPartOnline() override;
protected:
    void createGParts() override;

    virtual base::GPOnline* newGPOnline();

    void disposeMain() override;
};
