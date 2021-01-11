
#include "GWContent.h"

namespace base {
class GWPartInteraction : public GWPartContent {
private: typedef GWPartContent super;
private:
    GPInteraction* gpInteraction = nullptr;
    GPInteraction* interaction = nullptr;

public:
    GWPartInteraction(GWPartInteraction const&) = delete;
    GWPartInteraction(GWPartInteraction &&) = default;
    GWPartInteraction& operator=(GWPartInteraction const&) = delete;
    GWPartInteraction& operator=(GWPartInteraction &&) = default;

    explicit GWPartInteraction(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);

    IGPInteraction* getGPInteraction() override;
    IGPInteraction* getInteraction() override;

    ~GWPartInteraction() override;
protected:
    void createGParts() override;

    virtual GPInteraction* newGPInteraction();

    void disposeMain() override;
};
};
