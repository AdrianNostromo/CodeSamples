#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include <base/input/util/IBlockingInputLayer.h>

class IInputHandler : virtual public base::IInputLayer {
    pub virtual bool hasBlockingInputLayer(base::IBlockingInputLayer* blockingInputLayer) = 0;
    pub virtual void addBlockingInputLayer(base::IBlockingInputLayer* blockingInputLayer) = 0;
    pub virtual void removeBlockingInputLayer(base::IBlockingInputLayer* blockingInputLayer) = 0;

    pub virtual ~IInputHandler() = default;
};
