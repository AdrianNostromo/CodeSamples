#include "GPUEC_CustomFilters.h"
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <rpg3D/gw/entity/module/userControlled/filters/IWorldInteractionFilter.h>

using namespace rpg3D;

GPUEC_CustomFilters::GPUEC_CustomFilters(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void GPUEC_CustomFilters::onControlledEntityChange_pre() {
	super::onControlledEntityChange_pre();

	if (controledEntityTarget != nullptr && controledEntityTarget->userControlledModule != nullptr) {
		// Remove the current entity custom filters.
		controledEntityTarget->userControlledModule->setUserInputDispatcher(nullptr);
	}
}

void GPUEC_CustomFilters::onControlledEntityChanged() {
	super::onControlledEntityChanged();

	if (controledEntityTarget != nullptr && controledEntityTarget->userControlledModule != nullptr) {
		// Add the current entity custom filters.
		controledEntityTarget->userControlledModule->setUserInputDispatcher(this);
	}
}

void GPUEC_CustomFilters::hookCustomFilter(rpg3D::IWorldInteractionFilter* filter) {
	if (filter->getIsHookedRef()) {
		// Already hooked or has no inputs configured.
		throw LogicException(LOC);
	}
	filter->getIsHookedRef() = true;

	onHookCustomFilter(filter);
}

void GPUEC_CustomFilters::onHookCustomFilter(rpg3D::IWorldInteractionFilter* filter) {
	//void
}

void GPUEC_CustomFilters::releaseCustomFilter(rpg3D::IWorldInteractionFilter* filter) {
	if (!filter->getIsHookedRef()) {
		throw LogicException(LOC);
	}
	filter->getIsHookedRef() = false;

	if (filter->getHookedListEntries().count() > 0) {
		for (int i = 0; i < filter->getHookedListEntries().count(); i++) {
			IListEntry* listEntry = filter->getHookedListEntries().getDirect(i);

			listEntry->remove();
		}

		filter->getHookedListEntries().clear();
	}
}

IListEntry* GPUEC_CustomFilters::InsertFilterInList(rpg3D::IWorldInteractionFilter* filter, ListDL<IWorldInteractionFilter*>& customFiltersLists) {
	int inputPriority = filter->getInputPriority();

	// The sorting is: priority high->low.
	for(ListDL<IWorldInteractionFilter*>::Entry* entry = customFiltersLists.getFirst(); entry != nullptr; entry = entry->next) {
		if (inputPriority > entry->data->getInputPriority()) {
			return customFiltersLists.insertBeforeDirect(entry, filter);
		}
	}
	
	return customFiltersLists.appendDirect(filter);
}

GPUEC_CustomFilters::~GPUEC_CustomFilters() {
	//void
}
