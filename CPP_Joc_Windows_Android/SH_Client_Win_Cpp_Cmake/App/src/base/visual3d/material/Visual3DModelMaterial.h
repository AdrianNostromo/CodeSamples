#pragma once

#include <base/gh.h>

class Visual3DModelMaterial {pub dCtor(Visual3DModelMaterial);
    pub Visual3DModelMaterial();
    
    pub virtual Visual3DModelMaterial* clone() = 0;

    pub virtual ~Visual3DModelMaterial();
};
