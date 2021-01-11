#include "EntityTemplate.h"
#include <assert.h>
#include <base/exceptions/LogicException.h>

std::string EntityTemplate::ExtraDataID_tilingInformation = "graphics::TilingInformation";

Map2D<std::string, std::string, EntityTemplate*> EntityTemplate::entriesMap{};
Map2D<std::string, std::string, std::function<EntityTemplate*(sp<Array1D<std::shared_ptr<base::IWrappedValue>>>& configValues)>> EntityTemplate::customTemplatesFactoriesMap{};

EntityTemplate::EntityTemplate(std::string& nSpace, std::string& id)
    : nSpace(nSpace), id(id), baseId(id), configParams(nullptr)
{
    //void
}

EntityTemplate::EntityTemplate(std::string& nSpace, std::string& id, std::string& baseId, sp<Array1D<std::shared_ptr<base::IWrappedValue>>> configParams)
    : nSpace(nSpace), id(id), baseId(baseId), configParams(configParams)
{
    //void
}

EntityTemplate* EntityTemplate::newEntry(std::string& nSpace, std::string& id) {
    return newEntry(nSpace, id, id/*baseId*/, nullptr/*configParams*/);
}

EntityTemplate* EntityTemplate::newEntry(std::string& nSpace, std::string& id, std::string& baseId, sp<Array1D<std::shared_ptr<base::IWrappedValue>>> configParams) {
    // Check if the scenario already exists.
    if (entriesMap.containsKey(nSpace, id)) {
        throw LogicException(LOC);
    }

    EntityTemplate* newTemplate = *entriesMap.putDirect(
        nSpace, id,
        new EntityTemplate(nSpace, id, baseId, configParams)
    );

    return newTemplate;
}

// Optional
EntityTemplate* EntityTemplate::fromId(std::string& nSpace, std::string id) {
    return entriesMap.getDirect(nSpace, id, nullptr);
}

EntityTemplate* EntityTemplate::setModuleTemplate(BasicModuleTemplate* moduleTemplate) {
	if (moduleTemplate->moduleName.empty() || moduleTemplate->moduleId < 0) {
		// Templates must have a moduleName or moduleId.
		throw LogicException(LOC);
	}

	if (moduleTemplatesIdMap.containsKey(moduleTemplate->moduleId)) {
		throw LogicException(LOC);
	}
	moduleTemplatesIdMap.putDirect(moduleTemplate->moduleId, moduleTemplate);
    
    moduleTemplatesList.appendReference(moduleTemplate);

    return this;
}

bool EntityTemplate::hasModuleTemplate(int moduleId) {
    if(moduleTemplatesIdMap.getPointer(moduleId) != nullptr) {
        return true;
    }

    return false;
}

bool EntityTemplate::hasTagsList(ArrayList<std::string> tagsList) {
    for(int i=0;i<tagsList.size();i++) {
        std::string& tagEntry = *tagsList.getPointer(i);
        if(!hasTag(tagEntry)) {
            return false;
        }
    }

    return true;
}

bool EntityTemplate::hasTag(std::string& tag) {
    return tagsList.contains(tag);
}

void EntityTemplate::toPartsString(std::stringstream& ss) {
    ss << nSpace << " " << baseId;

    if (configParams != nullptr) {
        int count = configParams->count();
        for (int i = 0; i < count; i++) {
            std::shared_ptr<base::IWrappedValue>& wVal = configParams->getReference(i);
            ss << " ";

            wVal->toString(ss, true/*includeTypePrefix*/);
        }
    }
}

EntityTemplate* EntityTemplate::FromPartsString(std::shared_ptr<ArrayList<std::string>> partsList, int startPartIndex) {
    int cPartIndex = startPartIndex;

    std::string& nSpace = partsList->getReference(cPartIndex++);
    std::string& baseId = partsList->getReference(cPartIndex++);

    EntityTemplate* ret = nullptr;

    sp<Array1D<std::shared_ptr<base::IWrappedValue>>> configParams = nullptr;
    int configPartsCount = partsList->count() - cPartIndex;
    if (configPartsCount > 0) {
        configParams = new Array1D<std::shared_ptr<base::IWrappedValue>>(configPartsCount);
        for (int i = 0; i < configPartsCount;i++) {
            std::string& partString = partsList->getReference(cPartIndex++);
            
            configParams->setDirect(i, base::IWrappedValue::NewFromString(partString));
        }

        std::function<EntityTemplate* (sp<Array1D<std::shared_ptr<base::IWrappedValue>>>& configValues)>& foo = customTemplatesFactoriesMap.getReference(nSpace, baseId);
        ret = foo(configParams);
    } else {
        ret = fromId(nSpace, baseId);
    }

    if (ret == nullptr) {
        throw LogicException(LOC);
    }
    
    return ret;
}

EntityTemplate::~EntityTemplate() {
    //void
}
