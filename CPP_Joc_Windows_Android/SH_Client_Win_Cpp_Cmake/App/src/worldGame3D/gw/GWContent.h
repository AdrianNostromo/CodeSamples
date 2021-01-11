
#include "GWPartMain.h"

namespace base {
class GWPartContent : public GWPartMain {
private: typedef GWPartMain super;
private:
    base::GamePartContent* content = nullptr;
	base::GamePartContent* gpContent = nullptr;

public:
    GWPartContent(GWPartContent const&) = delete;
    GWPartContent(GWPartContent &&) = default;
    GWPartContent& operator=(GWPartContent const&) = delete;
    GWPartContent& operator=(GWPartContent &&) = default;

    explicit GWPartContent(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);

	base::IGPContent* getContent() override;
	base::IGPContent* getGPContent() override;

    ~GWPartContent() override;
protected:
    void createGParts() override;

    virtual base::GamePartContent* newGPContent();

    void disposeMain() override;
};
};
