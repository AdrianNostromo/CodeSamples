#pragma once

#include <base/gh.h>
#include "../../czCommon/config/CZ_Common.h"
#include <base/list/Array3D.h>
#include <base/list/ArrayList.h>
#include <memory>
#include <base/list/Array2D.h>
#include <base/list/ListDL.h>
#include <base/memory/SharedPointer.h>

class Visual3DModelMaterial;
class EntityTemplate;
class IWorldEntity;
class IEventListener;
namespace base {
	class GWEvent;
	namespace snackbar {
		class WrappedSnackbarView;
	};
};
namespace rpg3D {
	class ZoneContentData;
};
namespace graphics {
	class TilingInformation;
};

namespace customRoomgeons {
namespace czDefenseTownA {
class CZ_DefenseTownA : public czCommon::CZ_Common {priv typedef czCommon::CZ_Common super;pub dCtor(CZ_DefenseTownA);
	pub static std::string NAME;
	pub static std::shared_ptr<worldGame3D::ContentZone> FactoryFunction();
	
	priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

	priv sp<base::snackbar::WrappedSnackbarView> snackbarWView_partyIsNotValid = nullptr;

    pub explicit CZ_DefenseTownA();

	priv void onEvent(IEventListener& eventListener, base::GWEvent& event);

	priv static sp<rpg3D::ZoneContentData> CreateDefaultZoneContentData();

	prot void onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;
	prot void onZoneDeactivate_pre(base::IGameWorld* gw) override;

    pub ~CZ_DefenseTownA() override;
};
};
};