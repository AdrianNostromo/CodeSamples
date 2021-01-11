#include "DataGLResource.h"

DataGLResource::DataGLResource(BindLocations::Config* bindLocation)
	: super(bindLocation)
{
	//void
}

void DataGLResource::onLoadToGl() {
    super::onLoadToGl();

    isInvalidAllocationBlock = false;
    // Don't cause invalidation on load to gl to avoid a initial update when the buffer is not initialised with valid data.
    //isInvalidData = true;
}

void DataGLResource::onUnloadFromGl() {
    isInvalidData = true;

    super::onUnloadFromGl();
}

void DataGLResource::updateGLBufferIfNeeded(graphics::ShaderProgram* shader) {
    super::updateGLBufferIfNeeded(shader);

    if (isInvalidAllocationBlock) {
        isInvalidAllocationBlock = false;

        unloadFromGl();
        loadToGl();
    }

    if (isInvalidData) {
        onValidateData();
    }
}

void DataGLResource::dataValidatedOnLoadToGl() {
    isInvalidData = false;
}

void DataGLResource::invalidAllocationBlock() {
    if (isInvalidAllocationBlock) {
        return;
    }

    isInvalidAllocationBlock = true;

    // Don't call invalidateData() because that will cause an error on BufferObjec-s. Just set the invalid flag.
    isInvalidData = true;
}

void DataGLResource::invalidateData() {
    if (isInvalidData) {
        return;
    }

    isInvalidData = true;
}

void DataGLResource::onValidateData() {
    isInvalidData = false;
}

DataGLResource::~DataGLResource() {
	//void
}
