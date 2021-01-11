#include <base/visual2D/VisualUtil2D.h>
#include "GPMLayers2D.h"

using namespace base;

GPMLayers2D::GPMLayers2D(base::IGameWorld* gw)
    : super(gw)
{
    //void
}

void GPMLayers2D::createLayers() {
    super::createLayers();

    getVisualsMap()->putDirectB("layerTouchAreaUnder", layerTouchAreaUnder = VisualUtil2D::newGroup(getGameWorldContainer2D()));
    getVisualsMap()->putDirectB("layerSubUserInterface", layerSubUserInterface = VisualUtil2D::newGroup(getGameWorldContainer2D()));
    getVisualsMap()->putDirectB("layerMainUserInterface", layerMainUserInterface = VisualUtil2D::newGroup(getGameWorldContainer2D()));
    getVisualsMap()->putDirectB("layerTouchAreaOver", layerTouchAreaOver = VisualUtil2D::newGroup(getGameWorldContainer2D()));
}

GPMLayers2D::~GPMLayers2D() {
    //void
}
