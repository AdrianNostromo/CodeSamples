#include "ComputeSmartValuePoolData.h"
#include <base/exceptions/LogicException.h>

ComputeSmartValuePoolData::ComputeSmartValuePoolData() {
    //void
}

bool ComputeSmartValuePoolData::isInit_isPropDependingOnAnimating() {
    if(isPropDependingOnAnimating != -1) {
        return true;
    }

    return false;
}

bool ComputeSmartValuePoolData::isTrue_isPropDependingOnAnimating() {
    if(isPropDependingOnAnimating == 1) {
        return true;
    }else if(isPropDependingOnAnimating == 0) {
        return false;
    }

    throw LogicException(LOC);
}

void ComputeSmartValuePoolData::set_isPropDependsOnAnimating(bool value) {
    this->isPropDependingOnAnimating = value ? 1 : 0;
}

ComputeSmartValuePoolData::~ComputeSmartValuePoolData() {
    //void
}
