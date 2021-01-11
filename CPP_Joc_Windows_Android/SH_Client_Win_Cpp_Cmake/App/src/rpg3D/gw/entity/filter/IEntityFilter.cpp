#include "IEntityFilter.h"

using namespace rpg3D;

const int IEntityFilter::Flags::NotSelf = 1;
const int IEntityFilter::Flags::HasInstanceId = 2;
const int IEntityFilter::Flags::Team_Same = 3;
const int IEntityFilter::Flags::Team_Different = 4;
const int IEntityFilter::Flags::Team_Same_Or_NoTeamModule = 5;
const int IEntityFilter::Flags::Alive = 6;
const int IEntityFilter::Flags::LAST_FLAG_ID = Alive;
