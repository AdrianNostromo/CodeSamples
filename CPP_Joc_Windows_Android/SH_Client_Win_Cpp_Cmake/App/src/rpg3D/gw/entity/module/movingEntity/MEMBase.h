#pragma once

#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include <string>
#include "IMovingEntityModule.h"

class IWorldEntity;
namespace worldGame3D {class IVisual3DModule;}

class MEMBase : public EntityModule, virtual public IMovingEntityModule {
private: typedef EntityModule super;
public:
    rpg3D::MovingEntityTemplate* t;

    worldGame3D::IVisual3DModule* visual3D;

public:
    MEMBase(MEMBase const&) = delete;
    MEMBase(MEMBase &&) = default;
    MEMBase& operator=(MEMBase const&) = delete;
    MEMBase& operator=(MEMBase &&) = default;
    explicit MEMBase(IWorldEntity* entity, rpg3D::MovingEntityTemplate* t);

    ~MEMBase() override;
protected:
    void createBMain() override;

    void disposePre() override;
};
