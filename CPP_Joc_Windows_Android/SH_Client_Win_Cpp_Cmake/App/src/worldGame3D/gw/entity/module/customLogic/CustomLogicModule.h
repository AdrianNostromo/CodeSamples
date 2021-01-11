#pragma once

#include <worldGame3D/gw/entity/module/EntityModule.h>

namespace base {
	class CustomLogicTemplate;
}

namespace base {
class CustomLogicModule : public EntityModule {
private: typedef EntityModule super;
public:
	CustomLogicTemplate* t;

public:
    CustomLogicModule(
        IWorldEntity* entity,
		CustomLogicTemplate* t
    );
    ~CustomLogicModule();
};
};
