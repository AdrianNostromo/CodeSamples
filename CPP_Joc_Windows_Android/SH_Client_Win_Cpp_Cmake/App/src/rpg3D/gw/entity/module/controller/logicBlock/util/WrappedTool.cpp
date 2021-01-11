#include "WrappedTool.h"

using namespace rpg3D::util;

WrappedTool::WrappedTool(sp<rpg3D::ITool> tool)
	: tool(tool),
	iFilter_innactiveForActivation(std::bind(&WrappedTool::onEntityInteraction_innactiveForActivation, this, std::placeholders::_1, std::placeholders::_2)),
	iFilter_activeContinuousForUpdate(std::bind(&WrappedTool::onEntityInteraction_activeContinuousForUpdate, this, std::placeholders::_1, std::placeholders::_2)),
	iFilter_activeContinuousForDeactivation(std::bind(&WrappedTool::onEntityInteraction_activeContinuousForDeactivation, this, std::placeholders::_1, std::placeholders::_2)),
	iFilter_alwaysUpdate(std::bind(&WrappedTool::onEntityInteraction_alwaysUpdate, this, std::placeholders::_1, std::placeholders::_2))
{
	iFilter_innactiveForActivation.setFiltersListBorrowed(this->tool->getActivationFiltersList_innactiveForActivation());
	iFilter_activeContinuousForUpdate.setFiltersListBorrowed(this->tool->getActivationFiltersList_activeContinuousForUpdate());
	iFilter_activeContinuousForDeactivation.setFiltersListBorrowed(this->tool->getActivationFiltersList_activeContinuousForDeactivation());
	iFilter_alwaysUpdate.setFiltersListBorrowed(this->tool->getActivationFiltersList_alwaysUpdate());
}

void WrappedTool::onEntityInteraction_innactiveForActivation(IEventListener& eventListener, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
	std::shared_ptr< InteractionCacheEntry> cacheEntry = std::make_shared< InteractionCacheEntry>(rpg3D::ITool::FilterSource::innactiveForActivation, interaction);
	cachedInteractionsList.append_emplace(cacheEntry);
}

void WrappedTool::onEntityInteraction_activeContinuousForUpdate(IEventListener& eventListener, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
	std::shared_ptr< InteractionCacheEntry> cacheEntry = std::make_shared< InteractionCacheEntry>(rpg3D::ITool::FilterSource::activeContinuousForUpdate, interaction);
	cachedInteractionsList.append_emplace(cacheEntry); 
}

void WrappedTool::onEntityInteraction_activeContinuousForDeactivation(IEventListener& eventListener, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
	std::shared_ptr< InteractionCacheEntry> cacheEntry = std::make_shared< InteractionCacheEntry>(rpg3D::ITool::FilterSource::activeContinuousForDeactivation, interaction);
	cachedInteractionsList.append_emplace(cacheEntry); 
}

void WrappedTool::onEntityInteraction_alwaysUpdate(IEventListener& eventListener, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
	std::shared_ptr< InteractionCacheEntry> cacheEntry = std::make_shared< InteractionCacheEntry>(rpg3D::ITool::FilterSource::alwaysUpdate, interaction);
	cachedInteractionsList.append_emplace(cacheEntry); 
}

WrappedTool::~WrappedTool() {
	//void
}
