#pragma once

#include <base/gh.h>
#include <graphics/attribute/Attribute.h>

namespace graphics {
class MaterialAttribute : public Attribute {priv typedef Attribute super;pub dCtor(MaterialAttribute);
	prot static long registerAttribute(std::string& alias);

	pub explicit MaterialAttribute(long typeBitGroup);

	pub Attribute* clone(Attribute* o = nullptr) override;

    pub ~MaterialAttribute() override;
};
};
