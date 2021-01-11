#include <sh/app/IAppSquareHeads.h>
#include "SH_Base.h"

using namespace sh;

SH_Base::SH_Base(
    IRenderingHandler* renderingHandler,
    IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
    IContainer2D* layer2DGameWorld,
    IAppSquareHeads* appLocal)
    : super(renderingHandler, appAssets, appMetrics, appUtils, layer2DGameWorld, appLocal),
      appLocal(appLocal)
{
    //void
}

SH_Base::~SH_Base() {
    //void
}
