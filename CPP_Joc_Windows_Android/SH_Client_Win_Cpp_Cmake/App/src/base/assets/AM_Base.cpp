
#include "AM_Base.h"

std::string AM_Base::assetsFolderPath = "";
std::string AM_Base::cacheFolderPath = "cache/";

AM_Base::AM_Base(IApp* app)
    : app(app)
{
  //void
}

void AM_Base::reservedCreate() {
    createMain();
}

void AM_Base::createMain() {
    //void
}

void AM_Base::initGlobalTextures(ArrayList<graphics::TextureArray*>* globalTextures) {
    if (globalTextures == nullptr || this->globalTextures != nullptr) {
        throw LogicException(LOC);
    }

    this->globalTextures = globalTextures;
}

void AM_Base::reservedDisposeMain() {
    disposeMain();
}

void AM_Base::disposeMain() {
    //void
}
