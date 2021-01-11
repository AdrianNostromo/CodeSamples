
#include "GWPartsBase.h"

namespace base {
class GWPartMain : public GWPartsBase {
private: typedef GWPartsBase super;
private:
    GamePartMain * gpMain = nullptr;

public:
    GWPartMain(GWPartMain const&) = delete;
    GWPartMain(GWPartMain &&) = default;
    GWPartMain& operator=(GWPartMain const&) = delete;
    GWPartMain& operator=(GWPartMain &&) = default;

    explicit GWPartMain(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);

    IGamePartMain* getGPMain() override;
    IGamePartMain* getMain() override;

    IContainer2D* getGameWorldContainer2D() override { return gpMain->getGameWorldContainer2D(); }

    ~GWPartMain() override;
protected:
    void createGParts() override;
    int createComponents() override;
    int createLayers() override;
    int createContent() override;

    virtual GamePartMain* newGPMain();

    void isPlayEnabled_changedEvent() override;

    void disposeMain() override;
};
};
