#pragma once

#include <base/gh.h>
#include "V3DM_Base.h"
#include <worldGame3D/gw/main/util/IUpdatableEntityVisual.h>
#include <base/visual3d/IManagedDynamicVisualManager.h>
#include <graphics/visual3d/drawable/IDrawableNode3D.h>
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>
#include <base/map/Map1D.h>

namespace worldGame3D {
class V3DM_Drawables : public V3DM_Base, virtual public IUpdatableEntityVisual, virtual public IManagedDynamicVisualManager {priv typedef V3DM_Base super;pub dCtor(V3DM_Drawables);
    pub static base::EntityEvent* EVENT_visual3D_visibleChanged;
	pub static base::EntityEvent* EVENT_visual3D_updateVisual;

	// This is used with the subclass positional handling logic.
	priv bool isPositionalSyncListenerEnabled = false;
	priv bool isPositionalSyncListenerInitialised = false;
	priv ArrayList<std::shared_ptr<IEventListener>> syncAutoListenersList{};

	pub bool isVisualsDirty = false;

	// This is used to update the visual only once per frame (maximum times).
	priv IListEntry* inHandler_VisualUpdateListEntryRef = nullptr;

    prot ArrayList<IDrawableNode3D*>* drawablesList;

    priv Map1D<std::string, IDrawableNode3D*>* drawablesMap;
	prot IDrawableNode3D* rootDrawable;

	priv IContainer3D* visualsParent = nullptr;

	pub explicit V3DM_Drawables(
		IWorldEntity* entity,
		Visual3DTemplate* t,
		ArrayList<IDrawableNode3D*>* drawablesList, Map1D<std::string, IDrawableNode3D*>* drawablesMap, IDrawableNode3D* rootDrawable);
	prot void createMain() override;
	prot void createBMain() override;

	prot void setIsPositionalSyncListenerEnabled(bool isPositionalSyncListenerEnabled);

	pub IWorldEntity* getEntity() override;
	pub ArrayList<IDrawableNode3D*>* peekDrawablesList();
	pub void requireVisualsUpdate();
	pub void requireManagedVisualUpdate(IManagedDynamicVisual* targetManagedVisual) override;
	
	pub IContainer3D* getVisualsParent() final;
	pub void setVisualsParent(IContainer3D* visualsParent) final;

	pub IContainer3D* peekRootDrawableAsContainer() final;
	pub Quaternion* peekRootDrawable_rotation() final;
	pub Matrix4* peekRootDrawable_globalTransform(bool updateTransformStackIfDirty) final;
	pub bool peekRootDrawable_isGlobalTransformInit() final;

	pub bool getIsVisible() final;
	pub void setIsVisibleIncremental(bool visible) final;

	pub IDrawableNode3D* getDrawableOptional(std::string id) override;
	IDrawableNode3D* getDrawableMustExist(std::string id) override;

	prot void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	prot void disconnectFromGamePartsPre() override;

	pub IListEntry*& getInHandler_VisualUpdateListEntryRef() final;

	pub void updateVisual() override;

	priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

	prot void syncPos(Vector3* pos);
	prot void syncPos(float x, float y, float z);
	prot void syncPosX(float x);
	prot void syncPosY(float y);
	prot void syncPosZ(float z);
	prot void syncRot(Quaternion& rot);
	prot void syncRot(Vector3& axis, float degrees);
	prot void syncRotEulerZYX(Vector3& axis, float degrees);
	prot void syncCardinalRotation(float degrees);
	prot void syncScale(Vector3* scale);

	prot void disposeMain() override;
    pub ~V3DM_Drawables() override;
};
};
