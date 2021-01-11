#include "E_Base.h"

using namespace graphics;

E_Base::E_Base()
	: super()
{
	//void
}

void E_Base::reservedCreate() {
    if(isCreated) {
        throw LogicException(LOC);
    }
    isCreated = true;

    createMain();
    create();
    createPost();
}

bool E_Base::getIsCreated() {
    return isCreated;
}

void E_Base::createMain() {
    //void
}

void E_Base::create() {
    //void
}

void E_Base::createPost() {
    //void
}

void E_Base::syncIfNeeded() {
	//void
}

E_Base::~E_Base() {
	//void
}
