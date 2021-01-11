#pragma once

#include <worldGame3D/gw/main/util/IUpdatableEntityVisual.h>
#include <base/visual3d/IManagedDynamicVisualManager.h>
#include <graphics/visual3d/drawable/IDrawable3D.h>
#include "TRIP_Base.h"
#include <base/list/ArrayList.h>
#include <base/exceptions/LogicException.h>
#include <graphics/visual3d/drawable/IContainer3D.h>

namespace rpg3D {
class TRIP_Visual : public TRIP_Base, virtual public IUpdatableEntityVisual, virtual public IManagedDynamicVisualManager {priv typedef TRIP_Base super;pub dCtor(TRIP_Visual);
	priv IDrawableNode3D* rootDrawable = nullptr;
	priv ArrayList<IDrawableNode3D*>* drawablesList = nullptr;
	priv Map1D<std::string, IDrawableNode3D*>* drawablesMap = nullptr;

	priv bool isVisualsDirty = false;

	// This is used to update the visual only once per frame (maximum times).
	priv IListEntry* inHandler_VisualUpdateListEntryRef = nullptr;

	pub explicit TRIP_Visual(ToolConfigRangeInertialProjectile* config, std::shared_ptr<ExtraData> extraData);

	pub IWorldEntity* getEntity() override;

	pub IAppAssets* getAppAssets();

	pub void requireManagedVisualUpdate(IManagedDynamicVisual* targetManagedVisual) override;

	pub IListEntry*& getInHandler_VisualUpdateListEntryRef() final;

	pub void updateVisual() override;

	prot void onEntityChangePre() override;
	prot void onEntityChanged() override;

	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;

	priv void requireVisualsUpdate();

	prot template <typename T>
	T getToolDrawable(std::string& id, bool mustExist);

	priv void disposeVisualsIfAny();

	prot void disposeMain() override;
	~TRIP_Visual() override;
};

template <typename T>
T TRIP_Visual::getToolDrawable(std::string& id, bool mustExist) {
	IDrawableNode3D* visual = drawablesMap->getDirect(id);

	T cVisual = visual->castAs<T>();
	if(cVisual == nullptr && mustExist) {
		throw LogicException(LOC);
	}

	return cVisual;
}

};
