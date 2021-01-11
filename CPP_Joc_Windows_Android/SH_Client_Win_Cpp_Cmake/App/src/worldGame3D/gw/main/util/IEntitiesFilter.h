#pragma once

class IWorldEntity;

namespace base {
class IEntitiesFilter {
public:
	virtual bool test(IWorldEntity* entity) = 0;

	virtual ~IEntitiesFilter() = default;
};
};
