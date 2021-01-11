#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"

namespace graphics {
    class DirectionalLight;
};

namespace graphics {
class MADirectionalLightsList : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(MADirectionalLightsList);
    pub static std::string Alias;
    pub static long TypeBitGroup;

    priv ArrayList<DirectionalLight*> lightsListManaged{};

    pub explicit MADirectionalLightsList();

    pub ArrayList<DirectionalLight*>& peekLightsListManaged();

    pub void addLightManaged(DirectionalLight* light);

    pub void bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) final;

    pub MADirectionalLightsList* clone(Attribute* o = nullptr) override;

    pub ~MADirectionalLightsList() override;
};
};
