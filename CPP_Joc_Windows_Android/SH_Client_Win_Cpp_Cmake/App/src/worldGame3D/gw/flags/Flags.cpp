#include "Flags.h"
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/util/WorldFlag.h>
#include <worldGame3D/gw/util/WrappedWorldFlag.h>

using namespace worldGame3D;
using namespace worldGame3D::gw;

Flags::Flags() {
	//void
}

sp<WrappedWorldFlag> Flags::get(std::string& id, bool mustExist) {
    WorldFlag* flag = flagsMap.getDirect(id, nullptr);
    if (flag == nullptr) {
        if (mustExist) {
            throw LogicException(LOC);
        }

        return nullptr;
    }

    return flag->wrapper;
}

sp<WrappedWorldFlag> Flags::set(std::string& id, std::shared_ptr<base::IWrappedValue> wValue, bool isZoneOwned) {
    WorldFlag* flag = flagsMap.getDirect(id, nullptr);
    if (flag == nullptr) {
        flag = &(flagsList.appendEmplace(id, wValue, isZoneOwned)->data);

        flagsMap.putDirect(flag->id, flag);
    } else {
        if (flag->isZoneOwned != isZoneOwned) {
            // Bug occured probably. Flag updates must have the same isZoneOwned value.
            throw LogicException(LOC);
        }

        flag->wValue = wValue;
    }

    return flag->wrapper;
}

void Flags::removeAllZoneOwnedFlags() {
    flagsList.enumerateDirect([this](ListDL<WorldFlag>::Entry* listEntry, WorldFlag& flag, bool& stopEnumeration) ->void {
        if (flag.isZoneOwned) {
            flagsMap.removeMustExist(flag.id);
            listEntry->remove();
        }
    });
}

Flags::~Flags() {
	//void
}
