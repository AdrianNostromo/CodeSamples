#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"

namespace graphics {
    class SpotLight;
};

namespace graphics {
class MASpotLightsList : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(MASpotLightsList);
    pub static std::string Alias;
    pub static long TypeBitGroup;

    priv ArrayList<SpotLight*> lightsListManaged{};

    pub explicit MASpotLightsList();

    pub ArrayList<SpotLight*>& peekLightsListManaged();

    pub void addLightManaged(SpotLight* light);

    pub void bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) final;

    pub MASpotLightsList* clone(Attribute* o = nullptr) override;

    pub ~MASpotLightsList() override;
};
};
