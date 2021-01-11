#include "OGPConditionalEntities.h"
#include <worldGame3D/gw/entity/util/ConditionalWorldEntitySpawnConfig.h>
#include <worldGame3D/gw/entity/util/EntityRemoveReason.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/contentZones/zone/ContentZone.h>
#include <rpg3D/gw/zoneFlags/IOGPZoneFlags.h>
#include <base/container/flags/TestFlagEntry.h>
#include <base/container/flags/FlagEntry.h>
#include <base/container/flags/ManagedFlags.h>
#include <functional>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <worldGame3D/gw/main/GPMEntities.h>
#include <base/container/flags/ManagedTestFlags.h>

using namespace rpg3D;

EntityRemoveReason* const OGPConditionalEntities::ENT_REM_REASON_PURPOSE_FINISHED = new EntityRemoveReason("ENT_REM_REASON_PURPOSE_FINISHED", true, true);

OGPConditionalEntities::OGPConditionalEntities(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void OGPConditionalEntities::createBMain() {
	super::createBMain();

	gw->addEventListener(
		worldGame3D::IContentZone::GWEvent_onZoneDeactivatePre->type,
		std::bind(&OGPConditionalEntities::onGWEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

	gpZoneFlags = gw->getComponentAs<IOGPZoneFlags*>(true/*mustExist*/);
}

void OGPConditionalEntities::onGWEvent(IEventListener& eventListener, base::GWEvent& event) {
	if (event.type == worldGame3D::IContentZone::GWEvent_onZoneDeactivatePre->type) {
		conditionalEntitiesSpawnInfosList.clear();
	} else {
		throw LogicException(LOC);
	}
}

void OGPConditionalEntities::onZoneFlagEvent(IEventListener& eventListener, base::FlagsEvent& event) {
	if (event.type == base::FlagEntry::FlagsEvent_flagChanged->type) {
		if (event.flag == nullptr) {
			throw LogicException(LOC);
		}

		onZoneFlagsChange(event.flag->name);
	} else {
		throw LogicException(LOC);
	}
}

IWorldEntity* OGPConditionalEntities::addConditionalEntitySpawnInfo(sp<worldGame3D::ConditionalWorldEntitySpawnConfig> spawnInfo) {
	ConditionalEntitySpawnInfo* conditionalEntitySpawnInfo = conditionalEntitiesSpawnInfosList.append_emplace((spawnInfo));

	ArrayList<std::shared_ptr<base::TestFlagEntry>>& flagEntriesList = spawnInfo->spawnTestFlags.peekEntriesList();
	for (int i = 0; i < flagEntriesList.count(); i++) {
		std::shared_ptr<base::TestFlagEntry> testFlagEntry = flagEntriesList.getDirect(i);

		ListeningFlag* listeningFlagEntryValue = listeningFlagsTree.getValuePointer(testFlagEntry->name);
		if (listeningFlagEntryValue == nullptr) {
			// Not listening for this flag, add it to the listening flags.
			listeningFlagEntryValue = &(listeningFlagsTree.insertEmplace(testFlagEntry->name)->value);

			listeningFlagEntryValue->useCount++;

			base::FlagEntry* flagEntry = gpZoneFlags->getZoneFlags()->getOrCreateEntry(testFlagEntry->name);
			listeningFlagEntryValue->listenerEntry = flagEntry->getEventsDispatcher().addListener(std::bind(&OGPConditionalEntities::onZoneFlagEvent, this, std::placeholders::_1, std::placeholders::_2));
		} else {
			listeningFlagEntryValue->useCount++;
		}
	}

	// Do the flags check on the entity now. This is used to make spawn platforms available when spawning the player so he won't insta-activate the platform.
	return handleConditionalEntityChange(conditionalEntitySpawnInfo);
}

void OGPConditionalEntities::onZoneFlagsChange(std::string& flagName) {
	for (int i = 0; i < conditionalEntitiesSpawnInfosList.count(); i++) {
		ConditionalEntitySpawnInfo* conditionalEntitySpawnInfo = conditionalEntitiesSpawnInfosList.getPointer(i);

		// Only process the entries that use the flag.
		if (conditionalEntitySpawnInfo->spawnInfo->spawnTestFlags.usesFlag(flagName)) {
			handleConditionalEntityChange(conditionalEntitySpawnInfo);
		}
	}
}

IWorldEntity* OGPConditionalEntities::handleConditionalEntityChange(ConditionalEntitySpawnInfo* conditionalEntitySpawnInfo) {
	if (conditionalEntitySpawnInfo->activeEntity != nullptr && !gpZoneFlags->getZoneFlags()->test(conditionalEntitySpawnInfo->spawnInfo->spawnTestFlags)) {
		// Remove the entity.
		gw->getMainAs<base::GPMEntities*>()->markEntityForRemoval(conditionalEntitySpawnInfo->activeEntity, ENT_REM_REASON_PURPOSE_FINISHED, false);
		conditionalEntitySpawnInfo->activeEntity = nullptr;
	}

	if (conditionalEntitySpawnInfo->activeEntity == nullptr && gpZoneFlags->getZoneFlags()->test(conditionalEntitySpawnInfo->spawnInfo->spawnTestFlags)) {
		// Create the entity.
		EntityTemplate* entityTemplate = EntityTemplate::fromId(
			conditionalEntitySpawnInfo->spawnInfo->entityTemplateNSpace,
			conditionalEntitySpawnInfo->spawnInfo->entityTemplateId
		);

		IWorldEntity* entity = gw->getGPMain()->createEntity(
			entityTemplate,
			conditionalEntitySpawnInfo->spawnInfo->pos,
			&conditionalEntitySpawnInfo->spawnInfo->rot/*rot*/, nullptr/*cardinalRotation*/,
			conditionalEntitySpawnInfo->spawnInfo->instanceId,
			0,
			nullptr,
			true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
			nullptr
		);
		conditionalEntitySpawnInfo->activeEntity = entity;
	}

	return conditionalEntitySpawnInfo->activeEntity;
}

void OGPConditionalEntities::disposePre() {
	listeningFlagsTree.clear();

	super::disposePre();
}

OGPConditionalEntities::~OGPConditionalEntities() {
	//void
}
