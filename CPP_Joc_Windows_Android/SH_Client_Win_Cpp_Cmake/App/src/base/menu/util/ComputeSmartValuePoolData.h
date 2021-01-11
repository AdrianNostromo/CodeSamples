#pragma once

class ComputeSmartValuePoolData {
private:
    // -1 : not set.
    // 0 : false.
    // 1 : true : value locks the variable and is not overridden.
    int isPropDependingOnAnimating = -1;

public:
    ComputeSmartValuePoolData();

    bool isInit_isPropDependingOnAnimating();
    bool isTrue_isPropDependingOnAnimating();
    void set_isPropDependsOnAnimating(bool val);

    ~ComputeSmartValuePoolData();
};
