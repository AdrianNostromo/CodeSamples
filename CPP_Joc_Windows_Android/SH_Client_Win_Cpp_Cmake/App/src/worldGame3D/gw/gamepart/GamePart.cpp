#include "GamePart.h"
#include <base/exceptions/LogicException.h>
#include <cassert>

GamePart::GamePart(base::IGameWorld* gw)
    : gw(gw)
{
    componentClassVoidP = this;
}

void GamePart::reservedCreateB() {
    if(isCreateB) {
        throw LogicException(LOC);
    }
    isCreateB = true;

    isCreatedBCallBitMap = 0;
    createBMain();
    if (isCreatedBCallBitMap != (1)) {
        throw LogicException(LOC);
    }
}

void GamePart::createBMain() {
    if ((isCreatedBCallBitMap & 1) != 0) {
        throw LogicException(LOC);
    }
    isCreatedBCallBitMap |= 1;
}

base::IGameWorld* GamePart::getGW() {
    return gw;
}

void GamePart::addAutoListener(std::shared_ptr<IEventListener> listener) {
    if(autoListenersList.containsDirect(listener)) {
        throw LogicException(LOC);
    }

    autoListenersList.appendReference(listener);
}

void GamePart::reservedDisposePrePreIfNeeded() {
    if (!isDisposePrePre) {
        reservedDisposePrePre();
    }
};

void GamePart::reservedDisposePrePre() {
    if(isDisposePrePre) {
        throw LogicException(LOC);
    }
    isDisposePrePre = true;

    isDisposedPrePreCallBitMap = 0;
    disposePrePre();
    if (isDisposedPrePreCallBitMap != (1)) {
        throw LogicException(LOC);
    }
}

void GamePart::disposePrePre() {
    if ((isDisposedPrePreCallBitMap & 1) != 0) {
        throw LogicException(LOC);
    }
    isDisposedPrePreCallBitMap |= 1;
}

void GamePart::reservedDisposePreIfNeeded() {
    if (!isDisposePre) {
        reservedDisposePre();
    }
}

void GamePart::reservedDisposePre() {
    if(isDisposePre || !isDisposePrePre) {
        throw LogicException(LOC);
    }
    isDisposePre = true;

    isDisposedPreCallBitMap = 0;
    disposePre();
    if (isDisposedPreCallBitMap != (1)) {
        throw LogicException(LOC);
    }
}

void GamePart::disposePre() {
    if ((isDisposedPreCallBitMap & 1) != 0) {
        throw LogicException(LOC);
    }
    isDisposedPreCallBitMap |= 1;

    for(int i=autoListenersList.size()-1;i>=0;i--) {
        std::shared_ptr<IEventListener> listener = autoListenersList.removeAndGetDirect(i);

        if(!listener->isConnected()) {
            listener->disconnect();
        }
    }
}

GamePart::~GamePart() {
    // Also add the isCreate check to catch some odd bugs.
    if(!isCreateB || !isDisposePre || !isDisposePrePre) {
        assert(false);
    }
}
