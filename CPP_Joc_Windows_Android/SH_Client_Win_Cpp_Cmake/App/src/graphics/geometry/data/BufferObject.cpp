#include "BufferObject.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include "bufferChangesTracker/GLBCTSingleRegion.h"
#include "bufferChangesTracker/GLBCTIndividualRegions.h"

using namespace graphics;

BufferObject::BufferObject(
    BindLocations::Config* bindLocation, GLenum glBindTarget,
    unsigned int stride,
    UpdateMethod updateMethod,
    int initialCapacity, int minCapacity)
    : super(bindLocation),
    updateMethod(updateMethod),
    glBindTarget(glBindTarget),
    buffer(stride, minCapacity, initialCapacity)
{
    //void
}

void BufferObject::createMain() {
    super::createMain();

    if (updateMethod == SingleRegion) {
        bufferChangesTracker = new GLBCTSingleRegion(&buffer);
    } else if (updateMethod == IndividualRegions) {
        bufferChangesTracker = new GLBCTIndividualRegions(&buffer);
    } else {
        throw LogicException(LOC);
    }
}

CompositesBuffer& BufferObject::getBuffer() {
    return buffer;
}

void BufferObject::onLoadToGl() {
    super::onLoadToGl();

    if (!buffer.getIsLockedSize()) {
        // The data buffer size must be locked because the below code will alocate the vMemory block and the final size is required.
        throw LogicException(LOC);
    }

    GLUtil::gl->genBuffers(1, &gHandle);

    // Use a slot of 0 because this type of asset is indexed sometimes.
    bind(0/*slot*/, false/*isStorageAllocated*/);

    // This allocates the video_memory block.
    loggedTextureSizeBytes = GLUtil::gl->bufferData(
        glBindTarget/*target*/,
        buffer.byteCapacity()/*size_in_bytes*/, nullptr/*data*/,
        IGL::DYNAMIC_DRAW/*usage*/
    );
}

void BufferObject::onUnloadFromGl() {
    if(gHandle != 0) {
        GLUtil::gl->deleteBuffers(1, &gHandle, loggedTextureSizeBytes);

        gHandle = 0;

        if (bufferChangesTracker != nullptr) {
            // This is used to make sure a full update of the used region occurs when the buffer is loaded to gl.
            bufferChangesTracker->invalidateUsedRegion();
        }
    }

    super::onUnloadFromGl();
}

void BufferObject::invalidateData() {
    //super::invalidateData();

    throw LogicException(LOC);
}

void BufferObject::invalidateUsedRegion() {
    if (bufferChangesTracker != nullptr) {
        bufferChangesTracker->invalidateUsedRegion();
    }

    super::invalidateData();
}

void BufferObject::invalidateRegion(int entriesOffset, int entriesCount) {
    if (bufferChangesTracker != nullptr) {
        bufferChangesTracker->invalidateRegion(entriesOffset, entriesCount);
    }

    super::invalidateData();
}

void BufferObject::onValidateData() {
    super::onValidateData();

    if (bindLocation->isIndexedBindTarget) {
        // This is a UBO that uses indexed binding targets.
        // Need to call bindBuffer because the below bufferSubData works on the bound target.
        /// This won't affect the indexed binding from bindBufferBase (there are 2 separate binding types (target and array_slot)).
        GLUtil::gl->bindBuffer(glBindTarget, gHandle);
    }

    // This updates existing video_memory, it does not modify the block size.
    // This will not update outside of the block allocated with bufferData.
    if (bufferChangesTracker != nullptr) {
        bufferChangesTracker->validate(glBindTarget);
    } else {
        if (buffer.getUnusedCenterRegionRef().getLenX() > 0) {
            // The buffer contains a center region that is unused. Update only the sides.
            /// The empty center region is used by mem managers logic.
            int unusedCenterRegion_offsetBytes = buffer.getUnusedCenterRegionRef().min.x * buffer.stride();
            int unusedCenterRegion_countBytes = buffer.getUnusedCenterRegionRef().getLenX() * buffer.stride();

            if (unusedCenterRegion_offsetBytes > 0) {
                // Write the left region from start_of_the_buffer to the unused_region.
                GLUtil::gl->bufferSubData(
                    glBindTarget/*target*/,
                    0/*offset_in_bytes*/, unusedCenterRegion_offsetBytes/*size_in_bytes*/,
                    buffer.getData()/*data*/
                );
            }

            int unusedRegionRightBytes = unusedCenterRegion_offsetBytes + unusedCenterRegion_countBytes;
            if (buffer.byteCount() > unusedRegionRightBytes) {
                // Write the left region from unused_region to the buffer end.
                GLUtil::gl->bufferSubData(
                    glBindTarget/*target*/,
                    unusedRegionRightBytes/*offset_in_bytes*/, buffer.byteCount() - (unusedRegionRightBytes)/*size_in_bytes*/,
                    buffer.getDataBlockAsCharP() + (unusedRegionRightBytes)/*data*/
                );
            }
        } else {
            GLUtil::gl->bufferSubData(
                glBindTarget/*target*/,
                0/*offset_in_bytes*/, buffer.byteCount()/*size_in_bytes*/,
                buffer.getData()/*data*/
            );
        }
    }
}

void BufferObject::onBind(int slot, graphics::ShaderProgram* shader) {
    super::onBind(slot, shader);

    if(!GLUtil::gl->getIsGlContext()) {
        throw LogicException(LOC);
    }

    if (bindLocation->isIndexedBindTarget) {
        // Bind a buffer object to an indexed buffer target.
        // There is a more limited form of this function, glBindBufferBase, that binds the entire buffer to an index.It simply omits the offset?and size? fields, but it has one major difference from glBindBufferRange.If you respecify the data store for the buffer with glBufferData, then all indices where it is bound with glBindBufferBase will take the new size(for any subsequent rendering commands).With glBindBufferRange, the bound locations will not be updated if the buffer's data size is changed.
        /// Note: Respecifying a buffer's data store with a larger or smaller size is generally not a good idea to begin with. You are advised not to rely on the resizing feature of glBindBufferBase.
        /// Can also use glBindBufferRange but the bindBufferBase method is more safe in certain cases (maybe the cases are implemented but currently are not used).

        // NOTE. This call binds to the array in the atarget AND ALSO in the actual target.
        /// NOTE. to update the data, recall bindBufferBase or call bindBuffer before update because 
        /// the data update works on the bound target, not the bound array slot.
        /// Do note that this does not replace standard buffer binding with glBindBuffer.It does in fact bind the buffer to the target ? parameter, thus unbinding whatever was bound to that target.But usually when you use glBindBufferRange, you are serious about wanting to use the buffer rather than just modify it.
        /// Think of glBindBufferRange as binding the buffer to two places : the particular indexand the target ? .glBindBuffer only binds to the target ? , not the index.
        GLUtil::gl->bindBufferBase(glBindTarget, slot, gHandle);
    } else {
        GLUtil::gl->bindBuffer(glBindTarget, gHandle);
    }
}

void BufferObject::onUnbind(int slot, graphics::ShaderProgram* shader) {
    super::onUnbind(slot, shader);

    if(!GLUtil::gl->getIsGlContext()) {
        throw LogicException(LOC);
    }

    if (bindLocation->isIndexedBindTarget) {
        GLUtil::gl->bindBufferBase(glBindTarget, slot, 0/*gHandle*/);
    } else {
        GLUtil::gl->bindBuffer(glBindTarget, 0);
    }
}

void BufferObject::disposeMain() {
    if (bufferChangesTracker != nullptr) {
        delete bufferChangesTracker;
        bufferChangesTracker = nullptr;
    }

    super::disposeMain();
}

BufferObject::~BufferObject() {
    //void
}
