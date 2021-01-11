#include "TSH_Throw.h"

using namespace rpg3D;

TSH_Throw::TSH_Throw(ToolConfigStackablesHauler* config)
	: super(config)
{
	//void
}

void TSH_Throw::initParam_doThrowCarriedOnCarriedEntityTouchDown(bool doThrowCarriedOnCarriedEntityTouchDown) {
	this->doThrowCarriedOnCarriedEntityTouchDown = doThrowCarriedOnCarriedEntityTouchDown;
}


TSH_Throw::~TSH_Throw() {
	//void
}
