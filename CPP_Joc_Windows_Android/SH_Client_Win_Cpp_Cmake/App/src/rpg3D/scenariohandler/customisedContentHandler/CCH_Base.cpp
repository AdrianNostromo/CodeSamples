#include "CCH_Base.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/customisedContent/IGamePartCustomisedContent.h>

using namespace rpg3D;

std::string CCH_Base::CUSTOMISED_CONTENT_CACHE_GROUP = "rpg3D";

CCH_Base::CCH_Base(IApp* app)
	: super(),
	app(app)
{
	//void
}

base::IGameWorld* CCH_Base::getActiveGameWorld() {
	return activeGameWorld;
}

void CCH_Base::setActiveGameWorld(base::IGameWorld* activeGameWorld) {
	if (this->activeGameWorld != nullptr) {
		this->activeWorld_gpCustomisedContent->setCustomisedContentHandler(nullptr);
		this->activeWorld_gpCustomisedContent = nullptr;

		this->activeGameWorld = nullptr;
	}

	if (activeGameWorld != nullptr) {
		this->activeGameWorld = activeGameWorld;

		this->activeWorld_gpCustomisedContent = activeGameWorld->getComponentAs<rpg3D::IGamePartCustomisedContent*>(true/*mustExist*/);
		if (this->activeWorld_gpCustomisedContent == nullptr) {
			throw LogicException(LOC);
		}

		this->activeWorld_gpCustomisedContent->setCustomisedContentHandler(this);
	}
}

CCH_Base::~CCH_Base() {
	//void
}
