#include "IManagedGlResource.h"

IManagedGlResource::BindLocations::Config IManagedGlResource::BindLocations::ARRAY_BUFFER{0/*target*/, false/*isIndexedBindTarget*/};
IManagedGlResource::BindLocations::Config IManagedGlResource::BindLocations::ELEMENT_ARRAY_BUFFER{1/*target*/, false/*isIndexedBindTarget*/ };
IManagedGlResource::BindLocations::Config IManagedGlResource::BindLocations::UNIFORM_BUFFER{2/*target*/, true/*isIndexedBindTarget*/ };
IManagedGlResource::BindLocations::Config IManagedGlResource::BindLocations::FRAMEBUFFER{3/*target*/, false/*isIndexedBindTarget*/ };
IManagedGlResource::BindLocations::Config IManagedGlResource::BindLocations::SHADER{4/*target*/, false/*isIndexedBindTarget*/ };
IManagedGlResource::BindLocations::Config IManagedGlResource::BindLocations::TEXTURE{5/*target*/, true/*isIndexedBindTarget*/ };

const int IManagedGlResource::BindLocations::LocationsCount = 6;
