#include "CH_Base.h"

using namespace base;
using namespace base::crashHandler;

CH_Base::CH_Base() {
	ICrashHandler::crashHandler = this;
}

void CH_Base::reservedCreate() {
	create();
}

void CH_Base::create() {
	//void
}

CH_Base::~CH_Base() {
	//void
}
