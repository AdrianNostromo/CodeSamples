#pragma once

#include <base/gh.h>
#include <graphics/attribute/Attribute.h>

namespace graphics {
class RendererAttribute : public Attribute {priv typedef Attribute super;pub dCtor(RendererAttribute);
	prot static long registerAttribute(std::string& alias);

	pub explicit RendererAttribute(long typeBitGroup);

	pub Attribute* clone(Attribute* o = nullptr) override;

    pub ~RendererAttribute() override;
};
};
