#pragma once

#include "MEMMove.h"

class MovingEntityModule : public rpg3D::MEMMove {
private: typedef rpg3D::MEMMove super;
public:
    MovingEntityModule(MovingEntityModule const&) = delete;
    MovingEntityModule(MovingEntityModule &&) = default;
    MovingEntityModule& operator=(MovingEntityModule const&) = delete;
    MovingEntityModule& operator=(MovingEntityModule &&) = default;

    explicit MovingEntityModule(IWorldEntity* entity, rpg3D::MovingEntityTemplate* t);

    ~MovingEntityModule() override;
};
