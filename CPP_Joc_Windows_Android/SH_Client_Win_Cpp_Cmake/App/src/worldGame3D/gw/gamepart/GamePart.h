#pragma once

#include <base/gh.h>
#include "IGamePart.h"
#include <base/object/ManagedObject.h>
#include <base/list/ArrayList.h>
#include <memory>
#include <base/event/listener/IEventListener.h>

// Note. This class extends IComponent to make sure the interface exists somehere (in the case where no interfaces uses the component system).
/// In normal ussages, a interface will extend IComponent again and use 
class GamePart : public base::ManagedObject, virtual public IGamePart {priv typedef base::ManagedObject super;pub dCtor(GamePart);
    priv bool isCreateB = false;
    priv int isCreatedBCallBitMap = 0;

    priv bool isDisposePrePre = false;
    priv int isDisposedPrePreCallBitMap = 0;
    priv bool isDisposePre = false;
    priv int isDisposedPreCallBitMap = 0;

	prot base::IGameWorld* gw;

    // These callbacks will be auto disconnected
    prot ArrayList<std::shared_ptr<IEventListener>> autoListenersList{};

    pub explicit GamePart(base::IGameWorld* gw);
    pub void reservedCreateB();
    prot virtual void createBMain();

    base::IGameWorld* getGW() override;

    prot void addAutoListener(std::shared_ptr<IEventListener> listener);

    // This is used for some dispose that must occur before others (Content zone must be removed before the entities are removed by the gp_entities);
    pub void reservedDisposePrePreIfNeeded();
    pub void reservedDisposePrePre();
    prot virtual void disposePrePre();

    pub void reservedDisposePreIfNeeded();
    pub void reservedDisposePre();
    prot virtual void disposePre();

    pub ~GamePart() override;
};
