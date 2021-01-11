#pragma once

#include <base/gh.h>
#include "IEntityModule.h"
#include <string>
#include <base/exceptions/LogicException.h>
#include <base/object/ManagedObject.h>
#include <worldGame3D/gw/entity/event/EntityEvent.h>
#include <base/event/listener/IEventListener.h>
#include <base/list/ArrayList.h>
#include <memory>

class IWorldEntity;
namespace base {
	// Define here because subclasses use this often.
	class GWEvent;
};

class EntityModule : public base::ManagedObject, public virtual IEntityModule {priv typedef base::ManagedObject super;pub dCtor(EntityModule);
	// Not used with the new int module_id system.
    pub std::string moduleId;

	pub IWorldEntity* entity;
	pub IWorldEntity* e;

	// This contains event listeners from this module and are auto disposed on disposePre;
	prot ArrayList<std::shared_ptr<IEventListener>> autoListenersList{};

	prot worldGame3D::IGameWorldGameParts* gwGamePartsOptional = nullptr;

	pub explicit EntityModule(IWorldEntity* entity);
	pub explicit EntityModule(std::string& moduleId, IWorldEntity* entity);
	pub void reservedCreateB() final;

	// This is made final so it is not used by mistake. Modules should use createBMain instead (probabbly).
	prot void createPost() final;
	prot virtual void createBMain();

	prot void addAutoListener(std::shared_ptr<IEventListener> listener);

	prot virtual void disposePre();

	pub virtual std::string getModuleId() final;
	pub IWorldEntity* getEntity() override;

	pub void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	pub void disconnectFromGamePartsPre() override;
	prot virtual void disconnectedFromGameParts();

	pub void reservedDisposePre() final;
	pub ~EntityModule() override;
};
