#pragma once

#include <base/gh.h>
#include <base/math/Bounds2DInt.h>

namespace voxel3D {
class VoxelsGrid3DSparse {pub dCtor(VoxelsGrid3DSparse);
    pub enum Sides { xNeg = 0, xPos = 1, yNeg = 2, yPos = 3, zNeg = 4, zPos = 5 };
    
    pub class Quad {
        //asd_2;// convert to a short bounds after working example.
        Bounds2DInt bounds{};
    };

    pub class Voxel {
        pub Voxel* neighbours[6]{nullptr};

        pub Quad* quad = nullptr;
    };

    //asd_2;// seems to require a planeHead nodes.
    // Order is [x][y][z]
    pub Voxel* root = nullptr;

    pub explicit VoxelsGrid3DSparse();

    pub void set(short gX, short gY, short gZ);

    pub virtual ~VoxelsGrid3DSparse();
};
};
