#pragma once

#include <string>
#include <memory>
#include <base/object/IWrappedValue.h>

class IWorldEntity;

namespace base {
class EntityEvent {
private:
	static int typeCounter;

public:
	bool isBubbleAsGWEntityEvent;

	int type;

	std::string name;

	// This is optional and used to detect the origin of some actions (eg. stackable_pickup causes a move to occur and need that move action to not cancel the stackable_pickup action but other move action to cancel the stackable_pickup action).
	std::string initiatorIdentifier;

	void* voidData = nullptr;
	IWorldEntity* entityData = nullptr;
	IWorldEntity* entityDataOther = nullptr;
	std::shared_ptr<IWrappedValue> wrappedValue = nullptr;

public:
    EntityEvent(EntityEvent const&) = delete;
    EntityEvent(EntityEvent &&) = default;
    EntityEvent& operator=(EntityEvent const&) = delete;
    EntityEvent& operator=(EntityEvent &&) = default;

    explicit EntityEvent(bool isBubbleAsGWEntityEvent, std::string name);

	void clearData();

	void* getVoidData();
	EntityEvent* setVoidData(void* voidData);
	EntityEvent* setWrappedValue(std::shared_ptr<IWrappedValue> wrappedValue);

	IWorldEntity* getEntityData();
	IWorldEntity* getEntityDataOther();
	EntityEvent* setEntityData(IWorldEntity* entityData, IWorldEntity* entityDataOther=nullptr);
	EntityEvent* setData_initiatorIdentifier(std::string& initiatorIdentifier);

	virtual ~EntityEvent();
};
};
