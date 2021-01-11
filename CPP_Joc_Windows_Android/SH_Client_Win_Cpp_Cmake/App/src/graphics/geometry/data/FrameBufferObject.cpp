#include "FrameBufferObject.h"
#include <assert.h>
#include <base/opengl/util/GLUtil.h>
#include <iostream>
#include <graphics/visual/Texture.h>

using namespace graphics;

FrameBufferObject* FrameBufferObject::DefaultFBO = nullptr;

FrameBufferObject::FrameBufferObject(
	bool isDefaultFBO)
    : super(&BindLocations::FRAMEBUFFER/*bindLocationIndex*/),
    drawBuffer(0), readBuffer(0), depthMapTexture(nullptr), depthMapTextureAutoBindSlot(0),
	isDefaultFBO(isDefaultFBO)
{
	if (!isDefaultFBO) {
		// This constructor can be used only for the default frambuffer.
		throw LogicException(LOC);
	}

	resourceHasZeroGHandle = true;
}

FrameBufferObject::FrameBufferObject(
    GLenum drawBuffer, GLenum readBuffer, Texture* depthMapTexture, int depthMapTextureAutoBindSlot)
    : super(&BindLocations::FRAMEBUFFER/*bindLocationIndex*/),
    drawBuffer(drawBuffer), readBuffer(readBuffer), depthMapTexture(depthMapTexture), depthMapTextureAutoBindSlot(depthMapTextureAutoBindSlot)
{
    //void
}

void FrameBufferObject::onLoadToGl() {
	super::onLoadToGl();

	if (isDefaultFBO) {
		gHandle = 0;

		return;
	}

	// Create framebuffer.
	CallGLChecked(GLUtil::gl->genFramebuffers(1, &gHandle));

	bind(false/*isStorageAllocated*/);

	// Atach the texture.
	// Bind to the slot that will be used in the color rendering to avoid extra texture binds.
	// This bind is required to make sure the texture is uploaded to gl.
	depthMapTexture->bind(depthMapTextureAutoBindSlot);
	// Attach the texture to the framebuffer.
	GLUtil::gl->framebufferTexture2D(IGL::FRAMEBUFFER, IGL::DEPTH_ATTACHMENT, IGL::TEXTURE_2D, depthMapTexture->getGHandle(), 0);

	// Configure framebuffer to have no color.
	// Specify which color buffers are to be drawn into.
	GLUtil::gl->drawBuffer(drawBuffer);
	// Select a color buffer source for pixels.
	GLUtil::gl->readBuffer(readBuffer);
}

void FrameBufferObject::onUnloadFromGl() {
	if (isDefaultFBO) {
		return;
	}

	if (gHandle != 0) {
		CallGLChecked(GLUtil::gl->deleteFramebuffers(1, &gHandle));

		gHandle = 0;
	}

	super::onUnloadFromGl();
}

void FrameBufferObject::onBind(int slot, graphics::ShaderProgram* shader) {
	super::onBind(slot, shader);

	if (!GLUtil::gl->getIsGlContext()) {
		// No gl context or textureArray mode.
		throw LogicException(LOC);
	}

	CallGLChecked(GLUtil::gl->bindFramebuffer(IGL::FRAMEBUFFER, gHandle));
}

void FrameBufferObject::onUnbind(int slot, graphics::ShaderProgram* shader) {
	super::onUnbind(slot, shader);

	if (isDefaultFBO) {
		// Don't throw an error because this occurs on Android when app is minimised because the default FBO is in gl resources management system.
		//throw LogicException(LOC);
	}

	if (!GLUtil::gl->getIsGlContext()) {
		// No gl context or textureArray mode.
		throw LogicException(LOC);
	}

	// This works because this unbind function is called only if the buffer was bound.
	CallGLChecked(GLUtil::gl->bindFramebuffer(IGL::FRAMEBUFFER, 0));
}

FrameBufferObject::~FrameBufferObject() {
    //void
}
