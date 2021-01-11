#pragma once

#include <base/gh.h>
#include <rpg3D/gw/contentZones/zone/ContentZone.h>
#include <base/list/ArrayList.h>
#include <memory>
#include <base/sp.h>

class IEventListener;
namespace base {
	class AsyncFHEvent;
};
namespace rpg3D {
	class ZoneContentData;
};

namespace customRoomgeons {
namespace czCommon {
class CZ_Common : public rpg3D::ContentZone {priv typedef rpg3D::ContentZone super;pub dCtor(CZ_Common);
	priv enum AssetsLoadStatus { PreStart, LoadingData, ProcessingData, Finished};
	priv AssetsLoadStatus assetsLoadStatus = PreStart;
	priv std::shared_ptr<std::string> loadingDataToProcess = nullptr;
	priv ArrayList<std::shared_ptr<IEventListener>> fhListenersList{};

	prot sp<rpg3D::ZoneContentData> loadedZoneData = nullptr;

	pub explicit CZ_Common(
		std::string& id,
		bool useAutoPlayerPositioning, std::shared_ptr<std::string> defaultZoneEntrance);

	pub int getMainUIViewID() final;
	pub rpg3D::MainInGameUIView* getNewMainUIViewUninitialised(base::IGameWorld* gw) final;

	prot bool loadActivationAssets(base::IGameWorld* gw) override;
	prot void unloadActivationAssets() override;
	priv void onAsyncFHEvent(IEventListener& eventListener, base::AsyncFHEvent& event);

	prot void onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;
	prot void onZoneDeactivate_pre(base::IGameWorld* gw) override;

    pub ~CZ_Common() override;
};
};
};
