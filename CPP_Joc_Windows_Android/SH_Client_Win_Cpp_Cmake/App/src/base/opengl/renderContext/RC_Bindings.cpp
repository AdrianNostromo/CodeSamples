#include "RC_Bindings.h"
#include <base/opengl/util/ManagedGlResource.h>
#include <base/exceptions/LogicException.h>

using namespace opengl;

// A value of 10 should be enough for the low gl resources count target of the app.
const int RC_Bindings::MAX_BINDING_RESOURCES_COUNT = 10;

RC_Bindings::RC_Bindings()
	: super(),
	boundGLResourcesList(IManagedGlResource::BindLocations::LocationsCount, MAX_BINDING_RESOURCES_COUNT)
{
	//void
}

void RC_Bindings::bindRequest(ManagedGlResource* glResource, bool isStorageAllocated, int slot, graphics::ShaderProgram* shader) {
	if (!glResource->_isBound || glResource->_bound_slot != slot || glResource->_bound_shader != shader) {
		if (glResource->_isBound) {
			// Already bound in a different place, unbind from there.
			// This is a limitation of the render_context logic but resources should never be bound to multiple places with the current logic.
			// Maybe implement multiple bindings when needed if allowed.
			unbindRequest(glResource);
		}

		ManagedGlResource* boundGLResource = boundGLResourcesList.getDirect(glResource->bindLocation->target, slot);
		if (boundGLResource == glResource) {
			// This case has to be caught in the above check.
			throw LogicException(LOC);
		}

		if (boundGLResource != nullptr) {
			// Fake unbind the previous gl resource because the new resource will occupy the slot.
			boundGLResource->_bound_slot = -1;
			boundGLResource->_bound_shader = nullptr;
			boundGLResource->_isBound = false;

			boundGLResourcesList.setDirect(glResource->bindLocation->target, slot, nullptr);
		}

		glResource->_bound_slot = slot;
		glResource->_bound_shader = shader;
		glResource->_isBound = true;
		boundGLResourcesList.setDirect(glResource->bindLocation->target, slot, glResource);

		glResource->onBind(glResource->_bound_slot, glResource->_bound_shader);
	}

	if (isStorageAllocated) {
		glResource->updateGLBufferIfNeeded(shader);
	}
}

void RC_Bindings::unbindRequest(ManagedGlResource* glResource) {
	if (!glResource->_isBound) {
		// Already unbound.
		return;
	}

	ManagedGlResource* boundGLResource = boundGLResourcesList.getDirect(glResource->bindLocation->target, glResource->_bound_slot);

	if (boundGLResource != glResource) {
		// There is another gl resource bound at the target. This should never occur.
		throw LogicException(LOC);
	}

	glResource->onUnbind(glResource->_bound_slot, glResource->_bound_shader);

	boundGLResourcesList.setDirect(glResource->bindLocation->target, glResource->_bound_slot, nullptr);
	glResource->_bound_slot = -1;
	glResource->_bound_shader = nullptr;
	glResource->_isBound = false;
}

void RC_Bindings::unbindShaderDependants(graphics::ShaderProgram* shader) {
	for (int target = 0; target < boundGLResourcesList.getLengthA(); target++) {
		for (int slot = 0; slot < boundGLResourcesList.getLengthB(); slot++) {
			ManagedGlResource* boundGLResource = boundGLResourcesList.getDirect(target, slot);

			if (boundGLResource != nullptr && boundGLResource->_bound_shader == shader) {
				unbindRequest(boundGLResource);
			}
		}
	}
}

void RC_Bindings::onGlContextDestroyedPre() {
	for (int target = 0; target < boundGLResourcesList.getLengthA(); target++) {
		for (int slot = 0; slot < boundGLResourcesList.getLengthB(); slot++) {
			ManagedGlResource* boundGLResource = boundGLResourcesList.getDirect(target, slot);

			if (boundGLResource != nullptr) {
				unbindRequest(boundGLResource);
			}
		}
	}
}

RC_Bindings::~RC_Bindings() {
	//void
}
