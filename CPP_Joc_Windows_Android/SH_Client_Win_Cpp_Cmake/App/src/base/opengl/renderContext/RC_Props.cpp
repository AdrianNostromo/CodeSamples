#include "RC_Props.h"
#include <base/opengl/util/GLUtil.h>
#include <base/opengl/IGL.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace opengl;

RC_Props::RC_Props()
	: super()
{
	//void
}

void RC_Props::onGlContextCreated() {
	super::onGlContextCreated();

	{
		depthMask = true;
		if (depthMask != depthMask_target) {
			isFlushed = false;
		}
	}

	{
		isDepthTestEnabled = false;
		if (isDepthTestEnabled != isDepthTestEnabled_target) {
			isFlushed = false;
		}

		depthFunction = 0;
		if (depthFunction != depthFunction_target) {
			if (isDepthTestEnabled_target) {
				isFlushed = false;
			}
		}

		depthRangeNear = 0.0f;
		if (depthRangeNear != depthRangeNear_target) {
			if (isDepthTestEnabled_target) {
				isFlushed = false;
			}
		}

		depthRangeFar = 1.0f;
		if (depthRangeFar != depthRangeFar_target) {
			if (isDepthTestEnabled_target) {
				isFlushed = false;
			}
		}
	}

	{
		isBlendingEnabled = false;
		if (isBlendingEnabled != isBlendingEnabled_target) {
			isFlushed = false;
		}

		blendingSFactor = IGL::ONE;
		if (blendingSFactor != blendingSFactor_target) {
			if (isBlendingEnabled_target) {
				isFlushed = false;
			}
		}

		blendingDFactor = IGL::ZERO;
		if (blendingDFactor != blendingDFactor_target) {
			if (isBlendingEnabled_target) {
				isFlushed = false;
			}
		}
	}

	{
		isCullFaceEnabled = false;
		if (isCullFaceEnabled != isCullFaceEnabled_target) {
			isFlushed = false;
		}

		cullFaceMode = IGL::BACK;
		if (cullFaceMode != cullFaceMode_target) {
			if (isCullFaceEnabled) {
				isFlushed = false;
			}
		}

		frontFaceMode = IGL::CCW;
		if (frontFaceMode != frontFaceMode_target) {
			if (isCullFaceEnabled) {
				isFlushed = false;
			}
		}
	}
}

bool RC_Props::getIsFlushed() {
	return isFlushed;
}

void RC_Props::flush() {
	if (isFlushed) {
		return;
	}

	{
		// Depth mask
		if (depthMask != depthMask_target) {
			depthMask = depthMask_target;

			GLUtil::gl->fromRenderContext_depthMask(depthMask);
		}
	}

	{
		// Depth test
		bool isDepthTestEnabled_old = isDepthTestEnabled;

		if (isDepthTestEnabled != isDepthTestEnabled_target) {
			isDepthTestEnabled = isDepthTestEnabled_target;

			if (isDepthTestEnabled) {
				//GlobalAccessLogHandler::devLogHandler->post_debug("gl Enable DEPTH_TEST");
				GLUtil::gl->fromRenderContext_enable(IGL::DEPTH_TEST);
			} else {
				//GlobalAccessLogHandler::devLogHandler->post_debug("gl Disable DEPTH_TEST");
				GLUtil::gl->fromRenderContext_disable(IGL::DEPTH_TEST);
			}
		}

		if (isDepthTestEnabled) {
			if (!isDepthTestEnabled_old/*just_got_enabled*/
				|| depthFunction != depthFunction_target/*change*/)
			{
				depthFunction = depthFunction_target;

				GLUtil::gl->fromRenderContext_depthFunc(depthFunction);
			}

			if (!isDepthTestEnabled_old/*just_got_enabled*/
				|| depthRangeNear != depthRangeNear_target/*change*/
				|| depthRangeFar != depthRangeFar_target/*change*/)
			{
				depthRangeNear = depthRangeNear_target;
				depthRangeFar = depthRangeFar_target;

				GLUtil::gl->fromRenderContext_depthRange(depthRangeNear, depthRangeFar);
			}
		}
	}
	
	{
		// Blending
		bool isBlendingEnabled_old = isBlendingEnabled;

		if (isBlendingEnabled != isBlendingEnabled_target) {
			isBlendingEnabled = isBlendingEnabled_target;

			if (isBlendingEnabled) {
				GLUtil::gl->fromRenderContext_enable(IGL::BLEND);
			} else {
				GLUtil::gl->fromRenderContext_disable(IGL::BLEND);
			}
		}

		if (isBlendingEnabled) {
			if (!isBlendingEnabled_old/*just_got_enabled*/
				|| blendingSFactor != blendingSFactor_target/*change*/
				|| blendingDFactor != blendingDFactor_target/*change*/)
			{
				blendingSFactor = blendingSFactor_target;
				blendingDFactor = blendingDFactor_target;

				GLUtil::gl->fromRenderContext_blendFunc(blendingSFactor, blendingDFactor);
			}
		}
	}

	{
		// Cull face.
		bool isCullFaceEnabled_old = isCullFaceEnabled;

		if (isCullFaceEnabled != isCullFaceEnabled_target) {
			isCullFaceEnabled = isCullFaceEnabled_target;

			if (isCullFaceEnabled) {
				//if (!GLUtil::gl->isEnabled(IGL::CULL_FACE)) {
					GLUtil::gl->fromRenderContext_enable(IGL::CULL_FACE);
					//GlobalAccessLogHandler::devLogHandler->post_debug("gl Enable CULL_FACE");

				//}
			} else {
				//if (GLUtil::gl->isEnabled(IGL::CULL_FACE)) {
					GLUtil::gl->fromRenderContext_disable(IGL::CULL_FACE);
					//GlobalAccessLogHandler::devLogHandler->post_debug("gl Disable CULL_FACE");
				//}
			}
		}

		if (isCullFaceEnabled) {
			if (!isCullFaceEnabled_old/*just_got_enabled*/
				|| cullFaceMode != cullFaceMode_target/*change*/)
			{
				cullFaceMode = cullFaceMode_target;

				GLUtil::gl->fromRenderContext_cullFace(cullFaceMode);
				//GlobalAccessLogHandler::devLogHandler->post_debug("gl cull_face mode %i", cullFaceMode);
			}
		}
		
		if (isCullFaceEnabled) {
			// Front_face is used only when cull enabled is used.
			if (!isCullFaceEnabled_old/*just_got_enabled*/
				|| frontFaceMode != frontFaceMode_target/*change*/)
			{
				frontFaceMode = frontFaceMode_target;

				GLUtil::gl->fromRenderContext_frontFace(frontFaceMode);
				//GlobalAccessLogHandler::devLogHandler->post_debug("gl front_face mode %i", frontFaceMode);
			}
		}
	}

	isFlushed = true;
}

void RC_Props::setDepthMask(GLboolean depthMask) {
	if (depthMask != depthMask_target) {
		depthMask_target = depthMask;

		isFlushed = false;
	}
}

void RC_Props::setDepthTest(bool isDepthTestEnabled, GLenum depthFunction, GLclampd depthRangeNear, GLclampd depthRangeFar) {
	if (isDepthTestEnabled != isDepthTestEnabled_target) {
		isDepthTestEnabled_target = isDepthTestEnabled;

		isFlushed = false;
	}

	if (depthFunction != depthFunction_target) {
		depthFunction_target = depthFunction;

		if (isDepthTestEnabled_target) {
			isFlushed = false;
		}
	}

	if (depthRangeNear != depthRangeNear_target) {
		depthRangeNear_target = depthRangeNear;

		if (isDepthTestEnabled_target) {
			isFlushed = false;
		}
	}

	if (depthRangeFar != depthRangeFar_target) {
		depthRangeFar_target = depthRangeFar;

		if (isDepthTestEnabled_target) {
			isFlushed = false;
		}
	}
}

void RC_Props::setBlending(bool isBlendingEnabled, GLenum blendingSFactor, GLenum blendingDFactor) {
	if (isBlendingEnabled != isBlendingEnabled_target) {
		isBlendingEnabled_target = isBlendingEnabled;

		isFlushed = false;
	}

	if (blendingSFactor != blendingSFactor_target) {
		blendingSFactor_target = blendingSFactor;

		if (isBlendingEnabled_target) {
			isFlushed = false;
		}
	}

	if (blendingDFactor != blendingDFactor_target) {
		blendingDFactor_target = blendingDFactor;

		if (isBlendingEnabled_target) {
			isFlushed = false;
		}
	}
}

void RC_Props::setCullFace(bool isCullFaceEnabled, GLenum cullFaceMode) {
	if (isCullFaceEnabled != this->isCullFaceEnabled_target) {
		this->isCullFaceEnabled_target = isCullFaceEnabled;

		isFlushed = false;
	}

	if (cullFaceMode != this->cullFaceMode_target) {
		this->cullFaceMode_target = cullFaceMode;

		if (this->cullFaceMode_target) {
			isFlushed = false;
		}
	}

	
}

void RC_Props::setFrontFace(GLenum frontFaceMode) {
	if (frontFaceMode != this->frontFaceMode_target) {
		this->frontFaceMode_target = frontFaceMode;

		if (this->isCullFaceEnabled_target) {
			isFlushed = false;
		}
	}
}

RC_Props::~RC_Props() {
	//void
}
