#include "AM_CachesHandler.h"
#include <base/assets/cachesHandler/CachesGroupsHandler.h>
#include <base/MM.h>

using namespace base;

AM_CachesHandler::AM_CachesHandler(IApp* app)
	: super(app)
{
	//void
}

void AM_CachesHandler::createMain() {
	super::createMain();

	cacheGroupsHandler = new CachesGroupsHandler();
	cacheGroupsHandler->reservedCreate();
}

ICachesGroupsHandler* AM_CachesHandler::getCacheGroupsHandler() {
	return cacheGroupsHandler;
}

void AM_CachesHandler::disposeMain() {
	if (cacheGroupsHandler != nullptr) {
		cacheGroupsHandler->reservedDisposeMain();
		delete cacheGroupsHandler;

		cacheGroupsHandler = nullptr;
	}

	super::createMain();
}

AM_CachesHandler::~AM_CachesHandler() {
	//void
}
