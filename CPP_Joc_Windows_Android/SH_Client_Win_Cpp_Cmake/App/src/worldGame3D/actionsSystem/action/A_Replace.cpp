#include "A_Replace.h"
#include <base/log/GlobalAccessLogHandler.h>
#include <base/app/config/AppConfig.h>

using namespace worldGame3D;

A_Replace::A_Replace(
	int type, std::string name,
	bool isSyncedToServer,
	int persistenceDurationMS,
	int groupsBitMap,
	int preRequirementsGroupsBitMask,
	int replaceGroupsBitMask)
	: super(
		type, name,
		isSyncedToServer,
		persistenceDurationMS,
		groupsBitMap,
		preRequirementsGroupsBitMask),
	replaceGroupsBitMask(replaceGroupsBitMask)
{
	//void
}

int A_Replace::getReplaceGroupsBitMask() {
	return replaceGroupsBitMask;
}

bool A_Replace::getIsReplaced() {
	return isReplaced;
}

void A_Replace::setIsReplaced(bool isReplaced) {
	this->isReplaced = isReplaced;
}

A_Replace::~A_Replace() {
	//void
}
