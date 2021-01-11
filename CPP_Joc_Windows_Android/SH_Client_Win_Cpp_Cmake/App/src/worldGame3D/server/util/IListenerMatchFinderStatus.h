#pragma once

namespace worldGame3D {
class IListenerMatchFinderStatus {
public:
    virtual void statusChanged() = 0;
    virtual void failed() = 0;

    virtual ~IListenerMatchFinderStatus() = default;
};
};
