#pragma once

#include <base/gh.h>
#include "IGL.h"
#include <base/list/ArrayList.h>

namespace opengl {
    class RenderContext;
};

class GLBase : virtual public IGL {pub dCtor(GLBase);
    // This is not the context but a index to detect when the context changes.
    prot int glContextIndex = -1;
    prot bool isGlContext = false;

    pub opengl::RenderContext* renderContext;

	// >= 1: active.
	// == 0: disabled.
	// else: err.
	prot int isProfilerActiveCounter = 0;
    
    // This is always active when the app has the is_debug flag.
    prot GlobalsCumulativeProfiler* activeGlobalsCumulativeProfiler = nullptr;
    prot GlobalsProfiler* activeGlobalsProfiler = nullptr;
    prot LocalsProfiler* activeLocalsProfiler = nullptr;

    prot LocalsProfiler* profiler2D = nullptr;
    prot LocalsProfiler* profiler3D = nullptr;
    // This is used for aux stuff (eg. physics 3d debug).
    prot LocalsProfiler* profilerSpecial = nullptr;

    priv ArrayList<IListener*> glContextListenersList{};
    //asd_r_1;// need an order to make the texture_array load before texture.
    priv ArrayList<IManagedGlResource*> managedGlResourcesList{};

    priv Color cachedClearColor{};

    priv Capabilities capabilities{};

    pub explicit GLBase();
    pub void reservedCreate();
    prot virtual void createMain();

    pub bool isProfilerEnabled() override;
    pub void enableProfiler() override;
    pub void disableProfiler() override;
    pub GlobalsCumulativeProfiler* getGlobalsCumulativeProfiler() override;
    pub GlobalsProfiler* getGlobalsProfiler() override;
    pub LocalsProfiler* getProfiler2D() override;
    pub LocalsProfiler* getProfiler3D() override;
    pub LocalsProfiler* getProfilerSpecial() override;
    pub void setActiveProfilerSpecial() override;
    pub void setActiveProfiler2D() override;
    pub void setActiveProfiler3D() override;

    pub Capabilities& getCapabilities() final;

    pub int getGlContextIndex() override;
    pub bool getIsGlContext() override;
    pub void disableIsGlContext();
    pub void incrementGlContextAndActivate();

    pub void addGlContextListener(IListener* listener) override;
    pub void removeGlContextListener(IListener* listener) override;

    pub void addManagedGlResource(IManagedGlResource* managedGlResource) override;
    pub void removeManagedGlResource(IManagedGlResource* managedGlResource) override;

    pub Color& getCachedClearColor() override;
    pub void setCachedClearColor(Color& cachedClearColor) override;
    pub void setCachedClearColorDirect(Color cachedClearColor) override;

    priv void dispatchEvent_onGlContextCreated();
    priv void dispatchEvent_onGlContextDestroyPre();

    pub ~GLBase() override;
};
