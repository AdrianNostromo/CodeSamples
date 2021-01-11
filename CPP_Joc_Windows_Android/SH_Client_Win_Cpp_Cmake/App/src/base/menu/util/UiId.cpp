#include "UiId.h"
#include <assert.h>
#include <base/util/StringUtil.h>
#include <base/MM.h>

UiId::UiId(std::shared_ptr<ArrayList<std::string>> splitId)
    : splitId(splitId)
{
    //void
}

UiId::UiId(UiId& src, int srcOffset, int count) {
    std::shared_ptr<ArrayList<std::string>> splitId = src.splitId->toNewArray(srcOffset, count);

    this->splitId = splitId;
}

UiId::UiId(ArrayList<std::string>& src, int srcOffset, int count) {
    std::shared_ptr<ArrayList<std::string>> splitId = src.toNewArray(srcOffset, count);

    this->splitId = splitId;
}

UiId::UiId(std::string& composedId) {
    std::shared_ptr<ArrayList<std::string>> idFragmentsList = StringUtil::split(composedId, ".", false);

    this->splitId = idFragmentsList;
}

UiId::UiId(std::string& composedId, std::string prefixLayer, bool containsAndIgnoreProp) {
    std::shared_ptr<ArrayList<std::string>> idParts = StringUtil::split(composedId, ".", false);

    ArrayList<std::string> expandedId{};
    for(int i=0;i<idParts->size();i++) {
        std::string entry = idParts->getDirect(i);

        if(i == 0) {
            if(entry == "root") {
                entry = "_root";
            }else if(entry == "local") {
                entry = "_local";
            }

            if(entry.find('_') != 0) {
                expandedId.appendDirect(prefixLayer);
            }
        }

        expandedId.appendDirect(entry);
    }

    // Remove the last gamepart because it is the parameter id.
    int finalPartsCount;
    if (containsAndIgnoreProp && expandedId.size() > 1) {
        finalPartsCount = expandedId.size() - 1;
    } else {
        finalPartsCount = expandedId.size();
    }

    std::shared_ptr<ArrayList<std::string>> splitId = expandedId.toNewArray(0, finalPartsCount);
    this->splitId = splitId;
}

UiId::UiId(std::string& composedId, std::string prefixLayer)
    : UiId(composedId, prefixLayer, false)
{
    //void
}

UiId* UiId::newIdFromComposedIdProp(std::string &composedIdProp, std::string prefixLayer) {
    UiId* ret = new UiId(composedIdProp, prefixLayer, true);

    return ret;
}

UiId* UiId::newIdFromComposedId(std::string& composedId, std::string prefixLayer) {
    UiId* ret = new UiId(composedId, prefixLayer);

    return ret;
}

UiId* UiId::newIdFromComposedId(std::string& composedId) {
    UiId* ret = new UiId(composedId);

    return ret;
}

std::string UiId::extractPropStringFromComposedIdProp(std::string& smartIdProp) {
    std::size_t i = smartIdProp.find_last_of('.');

    std::string s;
    if(i != std::string::npos) {
        s = smartIdProp.substr(i + 1, smartIdProp.length() - (i + 1));
    }

    return s;
}

std::string* UiId::newPropStringFromComposedIdProp(std::string& smartIdProp) {
    std::string s = extractPropStringFromComposedIdProp(smartIdProp);

    std::string* sp = new std::string(s);

    return sp;
}

std::shared_ptr<ArrayList<std::string>> UiId::getSplitId() {
    return splitId;
}

UiId::~UiId() {
    //void
}
