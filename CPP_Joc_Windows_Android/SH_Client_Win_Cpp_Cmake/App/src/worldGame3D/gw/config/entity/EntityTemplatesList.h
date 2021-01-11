#pragma once

#include <worldGame3D/gw/entity/template/EntityTemplate.h>

namespace base {
class EntityTemplatesList {
private:
	static std::string NAMESPACE;

public:
	static bool IsStaticsInitHooked;

	//void

public:
	static EntityTemplate* newEntry(std::string id);

private:
	static void InitStatics();

};
};
