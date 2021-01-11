#include "PseudoVisualData.h"
#include <base/exceptions/LogicException.h>

PseudoVisualData::PseudoVisualData() {
    //void
}

std::shared_ptr<base::IWrappedValue> PseudoVisualData::getPropertyValue(std::string property) {
    return propertiesMap.getDirect(property);
}

void PseudoVisualData::setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) {
    propertiesMap.putReference(property, value);
}

bool PseudoVisualData::isInit_StaticAnimatingPropertyStatus(std::string& property) {
    if(staticAnimatingPropertiesMap.getDirect(property, -1) != -1) {
        return true;
    }

    return false;
}

bool PseudoVisualData::isTrue_StaticAnimatingPropertyStatus(std::string& property) {
    int v = staticAnimatingPropertiesMap.getDirect(property, -1);

    if(v == 1) {
        return true;
    }else if(v == 0) {
        return false;
    }

    throw LogicException(LOC);
}

void PseudoVisualData::putStaticAnimatingPropertyStatus(std::string& property, int status) {
    staticAnimatingPropertiesMap.putDirect(property, status);
}

PseudoVisualData::~PseudoVisualData() {
    //void
}
