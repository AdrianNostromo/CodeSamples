#include "ViewDataBase.h"

ViewDataBase::ViewDataBase(IUIView* callerView, ViewDataBase* callerViewReshowData)
    : callerView(callerView), callerViewReshowData(callerViewReshowData)
{
    //void
}

ViewDataBase::~ViewDataBase() {
    //void
}
