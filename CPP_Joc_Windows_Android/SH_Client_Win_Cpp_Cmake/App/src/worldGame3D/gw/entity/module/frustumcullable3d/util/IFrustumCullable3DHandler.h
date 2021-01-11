#pragma once

class IFrustumCullable3DModule;

class IFrustumCullable3DHandler {
public:
    virtual void onFrustumCullable3DInvalid(IFrustumCullable3DModule* target) = 0;

	virtual ~IFrustumCullable3DHandler() = default;
};
