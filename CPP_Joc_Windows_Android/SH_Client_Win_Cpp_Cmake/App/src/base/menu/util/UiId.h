#pragma once

#include <string>
#include <memory>
#include <base/list/ArrayList.h>

// General User Interface element id.
class UiId {
private:
    std::shared_ptr<ArrayList<std::string>> splitId;

public:
    UiId(UiId const&) = delete;
    UiId(UiId &&) = default;
    UiId& operator=(UiId const&) = delete;
    UiId& operator=(UiId &&) = default;

    explicit UiId(std::shared_ptr<ArrayList<std::string>> splitId);

    explicit UiId(UiId& src, int srcOffset, int count);
    explicit UiId(ArrayList<std::string>& src, int srcOffset, int count);

    explicit UiId(std::string& composedId);
    explicit UiId(std::string& composedId, std::string prefixLayer);
    explicit UiId(std::string& composedId, std::string prefixLayer, bool containsAndIgnoreProp);

    static UiId* newIdFromComposedIdProp(std::string& composedIdProp, std::string prefixLayer);
    static UiId* newIdFromComposedId(std::string& composedId, std::string prefixLayer);
    static UiId* newIdFromComposedId(std::string& composedId);

    static std::string extractPropStringFromComposedIdProp(std::string &smartIdProp);
    static std::string* newPropStringFromComposedIdProp(std::string &smartIdProp);

    std::shared_ptr<ArrayList<std::string>> getSplitId();

    virtual ~UiId();

};
