#include <base/input/util/Touch.h>
#include "InputHBlockingStreamLayerListeners.h"
#include <base/MM.h>

InputHBlockingStreamLayerListeners::InputHBlockingStreamLayerListeners(IApp* app, INativeInput* deviceInputConnection, IErrorHandler* errorHandler)
    : super(app, deviceInputConnection, errorHandler)
{
    //void
}

bool InputHBlockingStreamLayerListeners::getIsCursorModeSupported(base::Touch::CursorMode* cursorMode) {
    return super::getIsCursorModeSupported(cursorMode);
}

void InputHBlockingStreamLayerListeners::onCursorModeChanged(base::IBlockingInputLayer* target) {
    base::IBlockingInputLayer* lastLayer = getBlockerStreamListener();

    if(lastLayer == target) {
        setCursorMode(lastLayer->getCursorMode());
    }
}

bool InputHBlockingStreamLayerListeners::getIsSensorSupported(base::Touch::SensorType* sensorId) {
    return deviceInputConnection->getIsSensorSupported(sensorId);
}

void InputHBlockingStreamLayerListeners::onIsSensorActiveChanged(base::Touch::SensorType* sensorId, bool isActive) {
    deviceInputConnection->setIsSensorActiveIncremental(sensorId, isActive);
}

bool InputHBlockingStreamLayerListeners::hasBlockingInputLayer(base::IBlockingInputLayer* blockingInputLayer) {
    return blockingListenersStack.contains(blockingInputLayer);
}

void InputHBlockingStreamLayerListeners::addBlockingInputLayer(base::IBlockingInputLayer* blockingInputLayer) {
    if(hasBlockingInputLayer(blockingInputLayer) || blockingInputLayer->getHandler() != nullptr) {
        throw LogicException(LOC);
    }

    blockingListenersStack.appendDirect(blockingInputLayer);
    blockingInputLayer->setHandler(this);

    // Set the mouse mode to the blocking layer type.
    setCursorMode(blockingInputLayer->getCursorMode());
}

void InputHBlockingStreamLayerListeners::removeBlockingInputLayer(base::IBlockingInputLayer* blockingInputLayer) {
    if(!hasBlockingInputLayer(blockingInputLayer) || blockingInputLayer->getHandler() != static_cast<base::IBlockingInputLayer::IHandler*>(this)) {
        throw LogicException(LOC);
    }

    blockingInputLayer->setHandler(nullptr);
    if(!blockingListenersStack.remove(blockingInputLayer)) {
        throw LogicException(LOC);
    }

    // Set the mouse mode to the new latest blocking layer type of normal if none exist.
    base::IBlockingInputLayer* lastLayer = getBlockerStreamListener();
    if(lastLayer != nullptr) {
        setCursorMode(lastLayer->getCursorMode());
    }else {
        setCursorMode(base::Touch::CursorMode::Normal);
    }
}

base::IBlockingInputLayer* InputHBlockingStreamLayerListeners::getBlockerStreamListener() {
    if(blockingListenersStack.size() > 0) {
        base::IBlockingInputLayer *lastLayer = blockingListenersStack.getDirect(blockingListenersStack.size() - 1);

        return lastLayer;
    }

    return nullptr;
}

bool InputHBlockingStreamLayerListeners::distributeInputCacheEntry(std::shared_ptr<base::SensorEvent>& inputCacheEntry) {
    bool ret = super::distributeInputCacheEntry(inputCacheEntry);

    base::IBlockingInputLayer* lastLayer = getBlockerStreamListener();
    if(lastLayer != nullptr) {
        int ct = lastLayer->distributeInputCacheEntry(inputCacheEntry);
        if(ct > 0) {
            ret = true;
        }
    }

    return ret;
}

InputHBlockingStreamLayerListeners::~InputHBlockingStreamLayerListeners() {
    //void
}
