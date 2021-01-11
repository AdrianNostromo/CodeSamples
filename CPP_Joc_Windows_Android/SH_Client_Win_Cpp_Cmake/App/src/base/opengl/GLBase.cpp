#include "GLBase.h"
#include <base/opengl/util/GLUtil.h>
#include <base/exceptions/LogicException.h>
#include <base/math/Matrix4.h>
#include <base/opengl/renderContext/RenderContext.h>
#include <base/app/config/AppConfig.h>
#include <base/MM.h>

GLBase::GLBase()
{
    renderContext = new opengl::RenderContext();

    GLUtil::gl = this;
    GLUtil::renderContext = renderContext;
}

void GLBase::reservedCreate() {
    createMain();
}

bool GLBase::isProfilerEnabled() {
    if(isProfilerActiveCounter >= 1) {
        return true;
    }

    return false;
}

void GLBase::enableProfiler() {
    isProfilerActiveCounter++;
    if(isProfilerActiveCounter == 1) {
        if(activeGlobalsProfiler != nullptr || profiler2D != nullptr || profiler3D != nullptr || profilerSpecial != nullptr) {
            throw LogicException(LOC);
        }

        activeGlobalsProfiler = new GlobalsProfiler();
        activeGlobalsProfiler->clear();
        
        profiler2D = new LocalsProfiler();
        profiler2D->clear();

        profiler3D = new LocalsProfiler();
        profiler3D->clear();

        profilerSpecial = new LocalsProfiler();
        profilerSpecial->clear();
    }
}

void GLBase::disableProfiler() {
    if(isProfilerActiveCounter <= 0) {
        throw LogicException(LOC);
    }

    isProfilerActiveCounter--;
    if(isProfilerActiveCounter == 0) {
        if(activeGlobalsProfiler == nullptr || profiler2D == nullptr || profiler3D == nullptr || profilerSpecial == nullptr) {
            throw LogicException(LOC);
        }

        activeLocalsProfiler = nullptr;

        delete profiler2D;
        profiler2D = nullptr;

        delete profiler3D;
        profiler3D = nullptr;

        delete profilerSpecial;
        profilerSpecial = nullptr;

        delete activeGlobalsProfiler;
        activeGlobalsProfiler = nullptr;
    }
}

IGL::LocalsProfiler* GLBase::getProfiler2D() {
    return profiler2D;
}

IGL::GlobalsCumulativeProfiler* GLBase::getGlobalsCumulativeProfiler() {
    return activeGlobalsCumulativeProfiler;
}

IGL::GlobalsProfiler* GLBase::getGlobalsProfiler() {
    return activeGlobalsProfiler;
}

IGL::LocalsProfiler* GLBase::getProfiler3D() {
    return profiler3D;
}

IGL::LocalsProfiler* GLBase::getProfilerSpecial() {
    return profilerSpecial;
}

void GLBase::setActiveProfiler2D() {
    if(!isProfilerEnabled()) {
        throw LogicException(LOC);
    }

    activeLocalsProfiler = profiler2D;
}

void GLBase::setActiveProfilerSpecial() {
    if(!isProfilerEnabled()) {
        throw LogicException(LOC);
    }

    activeLocalsProfiler = profilerSpecial;
}

void GLBase::setActiveProfiler3D() {
    if(!isProfilerEnabled()) {
        throw LogicException(LOC);
    }

    activeLocalsProfiler = profiler3D;
}

GLBase::Capabilities& GLBase::getCapabilities() {
    return capabilities;
}

void GLBase::createMain() {
    // Check types.

    // Matrix is a grid of 4x4 floats.
    unsigned int targetSize = 4 * 4 * 4;
    unsigned int actualSize = sizeof(Matrix4);
    if(actualSize != targetSize) {
        throw LogicException(LOC);
    }

    if (AppConfig::IS_DEBUG) {
        activeGlobalsCumulativeProfiler = new GlobalsCumulativeProfiler();
    }
}

int GLBase::getGlContextIndex() {
    return glContextIndex;
}

bool GLBase::getIsGlContext() {
    return isGlContext;
}

void GLBase::disableIsGlContext() {
    if(!isGlContext) {
        throw LogicException(LOC);
    }

    dispatchEvent_onGlContextDestroyPre();

    isGlContext = false;
}

void GLBase::incrementGlContextAndActivate() {
    glContextIndex++;
    isGlContext = true;

    dispatchEvent_onGlContextCreated();
}

void GLBase::addGlContextListener(IGL::IListener* listener) {
    if(listener->getIsListening()) {
        throw LogicException(LOC);
    }
    listener->setIsListening(true);

    if(glContextListenersList.contains(listener)) {
        throw LogicException(LOC);
    }

    glContextListenersList.appendDirect(listener);
}

void GLBase::removeGlContextListener(IGL::IListener* listener) {
    if(!listener->getIsListening()) {
        throw LogicException(LOC);
    }
    listener->setIsListening(false);

    if(!glContextListenersList.remove(listener)) {
        throw LogicException(LOC);
    }
}

void GLBase::addManagedGlResource(IManagedGlResource* managedGlResource) {
    if(managedGlResourcesList.contains(managedGlResource)) {
        throw LogicException(LOC);
    }

    managedGlResourcesList.appendDirect(managedGlResource);
}

void GLBase::removeManagedGlResource(IManagedGlResource* managedGlResource) {
    if(!managedGlResourcesList.remove(managedGlResource)) {
        throw LogicException(LOC);
    }
}

Color& GLBase::getCachedClearColor() {
    return cachedClearColor;
}

void GLBase::setCachedClearColor(Color& cachedClearColor) {
    this->cachedClearColor.set(cachedClearColor);

    clearColor(
        this->cachedClearColor.r,
        this->cachedClearColor.g,
        this->cachedClearColor.b,
        this->cachedClearColor.a
    );
}

void GLBase::setCachedClearColorDirect(Color cachedClearColor) {
    setCachedClearColor(cachedClearColor);
}

void GLBase::dispatchEvent_onGlContextCreated() {
    renderContext->onGlContextCreated();
    
    for(int i=0;i<glContextListenersList.size();i++) {
        IListener* entry = glContextListenersList.getDirect(i);

        entry->onGlContextCreated();
    }

    for(int i=0;i<managedGlResourcesList.size();i++) {
        IManagedGlResource* entry = managedGlResourcesList.getDirect(i);

        entry->onGlContextCreated();
    }
}

void GLBase::dispatchEvent_onGlContextDestroyPre() {
    // This must be called before the managed_gl_resources because it does the resources unbinding.
    renderContext->onGlContextDestroyedPre();

    for(int i=0;i<glContextListenersList.size();i++) {
        IListener* entry = glContextListenersList.getDirect(i);

        entry->onGlContextDestroyedPre();
    }

    for(int i=0;i<managedGlResourcesList.size();i++) {
        IManagedGlResource* entry = managedGlResourcesList.getDirect(i);

        entry->onGlContextDestroyedPre();
    }
}

GLBase::~GLBase() {
    if(profiler2D != nullptr) {
        delete profiler2D;
        profiler2D = nullptr;
    }
    if(profiler3D != nullptr) {
        delete profiler3D;
        profiler3D = nullptr;
    }
    if(profilerSpecial != nullptr) {
        delete profilerSpecial;
        profilerSpecial = nullptr;
    }
    if(activeGlobalsCumulativeProfiler != nullptr) {
        delete activeGlobalsCumulativeProfiler;
        activeGlobalsCumulativeProfiler = nullptr;
    }
    if (renderContext != nullptr) {
        delete renderContext;
        renderContext = nullptr;
    }
}
