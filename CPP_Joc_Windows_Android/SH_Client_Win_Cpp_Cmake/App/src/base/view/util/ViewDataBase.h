#pragma once

class IUIView;

class ViewDataBase {
public:
    IUIView* callerView;
    ViewDataBase* callerViewReshowData;

public:
    ViewDataBase(ViewDataBase const&) = delete;
    ViewDataBase(ViewDataBase &&) = default;
    ViewDataBase& operator=(ViewDataBase const&) = delete;
    ViewDataBase& operator=(ViewDataBase &&) = default;

    explicit ViewDataBase(IUIView* callerView, ViewDataBase* callerViewReshowData);

    virtual ~ViewDataBase();
};
