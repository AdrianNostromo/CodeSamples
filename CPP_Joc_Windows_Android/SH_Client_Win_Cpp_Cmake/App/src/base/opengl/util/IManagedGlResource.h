#pragma once

#include <base/gh.h>
#include <base/object/IManagedObject.h>

class IManagedGlResource : virtual public base::IManagedObject {
    pub class BindLocations {
        pub class Config {pub dCtor(Config);
            pub const int target;

            pub const bool isIndexedBindTarget;

            pub explicit Config(int target, bool isIndexedBindTarget)
                : target(target), isIndexedBindTarget(isIndexedBindTarget)
            {
                // void
            }
        };

        pub static Config ARRAY_BUFFER;
        pub static Config ELEMENT_ARRAY_BUFFER;
        pub static Config UNIFORM_BUFFER;
        pub static Config FRAMEBUFFER;
        pub static Config SHADER;
        pub static Config TEXTURE;

        pub static const int LocationsCount;
    };

    pub virtual void onGlContextCreated() = 0;
    pub virtual void onGlContextDestroyedPre() = 0;

    pub ~IManagedGlResource() override = default;
};
