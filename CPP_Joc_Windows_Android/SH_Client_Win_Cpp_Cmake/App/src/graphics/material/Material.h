#pragma once

#include <base/gh.h>
#include "../attribute/AttributesList.h"

namespace graphics {
class Material final/*Note1001. calls_the_reserved_dispose*/ : public AttributesList {priv typedef AttributesList super;pub dCtor(Material);
    pub explicit Material();

    pub Material* clone(AttributesList* o=nullptr) override;

    pub ~Material() override;
};
};
