#include "GLBCTSingleRegion.h"
#include <base/opengl/util/GLUtil.h>
#include <base/list/CompositesBuffer.h>
#include <base/opengl/IGL.h>
#include <base/math/Math.h>

using namespace graphics;

GLBCTSingleRegion::GLBCTSingleRegion(CompositesBuffer* buffer)
	: super(buffer)
{
	//void
}

void GLBCTSingleRegion::validate(GLenum glBindTarget) {
	int invalidRangeLen = invalidEntriesRange.getLenX();
	if (invalidRangeLen > 0) {
		// Make sure to not update outside of the active buffer range if it shrunk.
		GLUtil::gl->bufferSubData(
			glBindTarget/*target*/,
			invalidEntriesRange.min.x * buffer->stride()/*offset_in_bytes*/, Math::min(invalidRangeLen, buffer->count()) * buffer->stride()/*size_in_bytes*/,
			buffer->getData()/*data*/
		);

		invalidEntriesRange.clear();
	}

	//asd_01;// a bug exists here, it seems like the remaping buffer is the proble.
	//asd_01;_t;
	GLUtil::gl->bufferSubData(
		glBindTarget/*target*/,
		0/*offset_in_bytes*/, buffer->count() * buffer->stride()/*size_in_bytes*/,
		buffer->getData()/*data*/
	);
}

void GLBCTSingleRegion::invalidateUsedRegion() {
	// This class doesn't ignore the optional buffer empty region because this does only 1 update block.
	/// The center_empty buffer region is not used with this update method.
	invalidEntriesRange.unionOther(0/*xMin*/, 0 + buffer->count()/*xMax*/);
}

void GLBCTSingleRegion::invalidateRegion(int blockEntriesOffset, int blockEntriesCount) {
	invalidEntriesRange.unionOther(blockEntriesOffset/*xMin*/, blockEntriesOffset + blockEntriesCount/*xMax*/);
}

GLBCTSingleRegion::~GLBCTSingleRegion() {
	//void
}
