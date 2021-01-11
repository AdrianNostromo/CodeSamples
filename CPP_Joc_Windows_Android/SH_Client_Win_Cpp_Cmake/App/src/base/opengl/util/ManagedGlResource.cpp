#include <base/opengl/util/GLUtil.h>
#include "ManagedGlResource.h"
#include <base/exceptions/LogicException.h>

ManagedGlResource::ManagedGlResource(BindLocations::Config* bindLocation)
    : super(),
    bindLocation(bindLocation)
{
    //void
}

void ManagedGlResource::createPost() {
    super::createPost();

    // Add to managed gl resource manager.
    GLUtil::gl->addManagedGlResource(this);

    if (GLUtil::gl->getIsGlContext()) {
        loadToGl();
    }
}

void ManagedGlResource::onGlContextCreated() {
    loadToGl();
}

void ManagedGlResource::onGlContextDestroyedPre() {
    // The unbind is done by the RenderState before this.

    unloadFromGl();
}

unsigned int ManagedGlResource::getGHandle() {
    return gHandle;
}

void ManagedGlResource::loadToGl() {
    if (loadedOnGlContextIndex != -1) {
        return;
    }

    loadedOnGlContextIndex = GLUtil::gl->getGlContextIndex();

    onLoadToGl();
}

void ManagedGlResource::unloadFromGl() {
    if (loadedOnGlContextIndex == -1) {
        return;
    }

    unbind();
    loadedOnGlContextIndex = -1;

    onUnloadFromGl();
}

void ManagedGlResource::onLoadToGl() {
    if (gHandle != 0) {
        // Load to gl called but the asset already has a gl resource handle.
        /// This seems like an android thing but should never occur because android gl unload is done correctly.
        throw LogicException(LOC);
    }
}

void ManagedGlResource::onUnloadFromGl() {
    gHandle = 0;
}

void ManagedGlResource::bind(bool isStorageAllocated) {
    if (bindLocation->isIndexedBindTarget) {
        throw LogicException(LOC);
    }

    GLUtil::renderContext->bindRequest(this, isStorageAllocated, 0/*slot*/, nullptr/*shader*/);
}

void ManagedGlResource::bind(int slot, bool isStorageAllocated) {
    if (slot < 0 && bindLocation->isIndexedBindTarget) {
        throw LogicException(LOC);
    }

    GLUtil::renderContext->bindRequest(this, isStorageAllocated, slot, nullptr/*shader*/);
}

void ManagedGlResource::bind(graphics::ShaderProgram* shader, bool isStorageAllocated) {
    if (bindLocation->isIndexedBindTarget) {
        throw LogicException(LOC);
    }

    GLUtil::renderContext->bindRequest(this, isStorageAllocated, 0/*slot*/, shader);
}

void ManagedGlResource::unbind() {
    GLUtil::renderContext->unbindRequest(this);
}

void ManagedGlResource::onBind(int slot, graphics::ShaderProgram* shader) {
    // Note. The resource may have a gHandle of 0 here (tex_array with no nextures), it is fine.
    if (gHandle == 0 && !resourceHasZeroGHandle && !resourceHasOptionalGHandle) {
        // Resource is not uploaded to gl. Make sure reservedCreate is called on this.
        if (!getIsCreated()) {
            // Bug detected. Forgot to call managedCreate on the resource.
            throw LogicException(LOC);
        }
    }
}

void ManagedGlResource::onUnbind(int slot, graphics::ShaderProgram* shader) {
    //void
}

void ManagedGlResource::updateGLBufferIfNeeded(graphics::ShaderProgram* shader) {
    //void
}

int ManagedGlResource::getBoundSlotOptional() {
    if (!_isBound) {
        return 0;
    }

    return _bound_slot;
}

int ManagedGlResource::getBoundSlotMustExist() {
    if (!_isBound) {
        throw LogicException(LOC);
    }

    return _bound_slot;
}

void ManagedGlResource::disposeMainPre() {
    unbind();

    unloadFromGl();

    super::disposeMainPre();
}

void ManagedGlResource::disposeMain() {
    // Remove from managed gl resource manager.
    GLUtil::gl->removeManagedGlResource(this);

    super::disposeMain();
}

ManagedGlResource::~ManagedGlResource() {
    //void
}
