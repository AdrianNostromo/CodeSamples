#include "RS_Base.h"

using namespace base;

RS_Base::RS_Base()
	: super()
{
	IRuntimeSettings::runtimeSettings = this;
}

RS_Base::~RS_Base() {
	//void
}
