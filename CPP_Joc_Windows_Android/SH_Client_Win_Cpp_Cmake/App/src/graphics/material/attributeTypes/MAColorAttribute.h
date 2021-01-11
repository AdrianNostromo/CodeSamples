#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"
#include <graphics/visual/Color.h>

namespace graphics {
class MAColorAttribute : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(MAColorAttribute);
    pub static std::string Alias_Diffuse;
    pub static long TypeBitGroup_Diffuse;

    //asd_r;// implement when needed.
    /*public final static String SpecularAlias = "specularColor";
    public final static long Specular = register(SpecularAlias);
    public final static String AmbientAlias = "ambientColor";
    public static final long Ambient = register(AmbientAlias);
    public final static String EmissiveAlias = "emissiveColor";
    public static final long Emissive = register(EmissiveAlias);
    public final static String ReflectionAlias = "reflectionColor";
    public static final long Reflection = register(ReflectionAlias);
    public final static String AmbientLightAlias = "ambientLightColor";
    public static final long AmbientLight = register(AmbientLightAlias);
    public final static String FogAlias = "fogColor";
    public static final long Fog = register(FogAlias);*/

    pub Color col;

    pub explicit MAColorAttribute(long typeBitGroup, Color& col);

    pub static MAColorAttribute* NewDiffuse(Color col);

    pub MAColorAttribute* clone(Attribute* o = nullptr) override;

    pub ~MAColorAttribute() override;
};
};
