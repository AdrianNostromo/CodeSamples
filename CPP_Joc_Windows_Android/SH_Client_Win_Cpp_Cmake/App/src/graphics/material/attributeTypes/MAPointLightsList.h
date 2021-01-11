#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"

namespace graphics {
    class PointLight;
};

namespace graphics {
class MAPointLightsList : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(MAPointLightsList);
    pub static std::string Alias;
    pub static long TypeBitGroup;

    priv ArrayList<PointLight*> lightsListManaged{};

    pub explicit MAPointLightsList();

    pub ArrayList<PointLight*>& peekLightsListManaged();

    pub void addLightManaged(PointLight* light);

    pub void bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) final;

    pub MAPointLightsList* clone(Attribute* o = nullptr) override;

    pub ~MAPointLightsList() override;
};
};
