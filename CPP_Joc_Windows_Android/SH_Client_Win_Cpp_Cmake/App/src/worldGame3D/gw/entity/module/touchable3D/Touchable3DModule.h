#pragma once

#include "Touchable3DModule_AABBBounds.h"

class Touchable3DModule : public Touchable3DModule_AABBBounds {
private: typedef Touchable3DModule_AABBBounds super;
public:
    Touchable3DModule(
        IWorldEntity* entity,
        Touchable3DTemplate* t
    );
    ~Touchable3DModule();
};
