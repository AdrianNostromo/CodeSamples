#include "VCC_Group.h"

int VCC_Group::TYPE = super::NewType();

VCC_Group::VCC_Group()
    : super(TYPE)
{
    //void
}

VCC_Group* VCC_Group::New() {
    VCC_Group* ret = new VCC_Group();

    return ret;
}

VCC_Group::~VCC_Group() {
    //void
}
