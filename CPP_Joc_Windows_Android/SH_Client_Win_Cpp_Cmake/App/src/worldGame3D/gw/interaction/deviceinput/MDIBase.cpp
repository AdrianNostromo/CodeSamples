#include "MDIBase.h"

using namespace base;

MDIBase::MDIBase(IGameWorld* gw)
    : gw(gw), appUtils(gw->getHandler()->getAppUtils())
{
    //void
}

void MDIBase::reservedCreate() {
    createMain();
}

void MDIBase::createMain() {
    //void
}

void MDIBase::reservedCreateB() {
    createBMain();
}

void MDIBase::createBMain() {
    //void
}

void MDIBase::reservedDisposePre() {
    disposePre();
}

void MDIBase::reservedDispose() {
    disposeMain();
}

void MDIBase::disposePre() {
    //void
}

void MDIBase::disposeMain() {
    //void
}

MDIBase::~MDIBase() {
    //void
}
