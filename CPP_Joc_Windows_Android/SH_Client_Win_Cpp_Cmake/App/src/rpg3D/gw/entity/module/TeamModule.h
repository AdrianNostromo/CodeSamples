#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "ITeamModule.h"
#include <rpg3D/gw/entity/template/TeamTemplate.h>

namespace rpg3D {
class TeamModule : public EntityModule, public virtual ITeamModule {priv typedef EntityModule super;pub dCtor(TeamModule);
	pub TeamTemplate* t;

    // 0 : not used.
    // >0 : used team.
    priv int teamIndex = 0;

    //asd;// Currently aggro is against team with different index, implement a different logic when needed.

    pub explicit TeamModule(
	    IWorldEntity* entity,
	    TeamTemplate* t);

    pub int getTeamIndex() final;
    pub void setTeamIndex(int teamIndex) final;

    pub ~TeamModule() override;
};
};
