#pragma once

#include <base/gh.h>
#include "InputHandlerCursor.h"
#include <base/input/util/IBlockingInputLayer.h>
#include <base/list/ArrayList.h>

// This is used to disable stream_game_inputs when a view is opened that uses the stream_game_inputs.
class InputHBlockingStreamLayerListeners : public InputHandlerCursor, virtual public base::IBlockingInputLayer::IHandler {priv typedef InputHandlerCursor super;pub dCtor(InputHBlockingStreamLayerListeners);
    // Only the last one receives the events.
    // New entries are appended to the end.
    priv ArrayList<base::IBlockingInputLayer*> blockingListenersStack{};

    pub explicit InputHBlockingStreamLayerListeners(IApp* app, INativeInput* deviceInputConnection, IErrorHandler* errorHandler);

    pub bool getIsCursorModeSupported(base::Touch::CursorMode* cursorMode) override;
    pub void onCursorModeChanged(base::IBlockingInputLayer* target) override;

	pub bool getIsSensorSupported(base::Touch::SensorType* sensorId) override;
	pub void onIsSensorActiveChanged(base::Touch::SensorType* sensorId, bool isActive) override;

    pub bool hasBlockingInputLayer(base::IBlockingInputLayer* blockingInputLayer) override;
    pub void addBlockingInputLayer(base::IBlockingInputLayer* blockingInputLayer) override;
    pub void removeBlockingInputLayer(base::IBlockingInputLayer* blockingInputLayer) override;

    prot bool distributeInputCacheEntry(std::shared_ptr<base::SensorEvent>& inputCacheEntry) override;

    priv base::IBlockingInputLayer* getBlockerStreamListener();

    pub ~InputHBlockingStreamLayerListeners() override;
};
