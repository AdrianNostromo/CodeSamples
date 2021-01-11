#include <graphics/visual2d/drawable/IInteractiveDrawable2D.h>
#include "CursorHandlingData.h"
#include <base/exceptions/LogicException.h>

using namespace base;

CursorHandlingData::CursorHandlingData(int cursorIndex)
    : cursorIndex(cursorIndex)
{
    //void
}

CursorHandlingData::ButtonHD* CursorHandlingData::getButtonHandlingData(base::Touch::ButtonCode* buttonId) {
    while(buttonsHandlingDataList.size() <= buttonId->index) {
        buttonsHandlingDataList.appendDirect(nullptr);
    }

    std::shared_ptr<ButtonHD> cursorHD = buttonsHandlingDataList.getDirect(buttonId->index, nullptr);
    if (cursorHD == nullptr) {
        cursorHD = *buttonsHandlingDataList.setDirect(buttonId->index, std::make_shared<ButtonHD>(this, buttonId));
    }

    return cursorHD.get();
}

void CursorHandlingData::addCursorOverTarget(IInteractiveDrawable2D* entry) {
    if(cursorOverTargetsList.contains(entry)) {
        throw LogicException(LOC);
    }

    entry->setIsCursorOver(cursorIndex, true);
	cursorOverTargetsList.appendReference(entry);
}

void CursorHandlingData::removeCursorOverTarget(IInteractiveDrawable2D* entry) {
    int i = cursorOverTargetsList.indexOf(entry);
    if(i < 0) {
        throw LogicException(LOC);
    }

    entry->setIsCursorOver(cursorIndex, false);
	cursorOverTargetsList.remove(i);
}

void CursorHandlingData::removeAllCursorOverTargets() {
    while(cursorOverTargetsList.size() > 0) {
        IInteractiveDrawable2D* target = cursorOverTargetsList.getDirect(cursorOverTargetsList.size()-1);

        removeCursorOverTarget(target);
    }
}

CursorHandlingData::~CursorHandlingData() {
    //void
}

CursorHandlingData::ButtonHD::ButtonHD(CursorHandlingData* cursorHD, base::Touch::ButtonCode* buttonId)
    : cursorHD(cursorHD), buttonId(buttonId)
{
    //void
}

void CursorHandlingData::ButtonHD::addCursorDownTarget(IInteractiveDrawable2D* entry) {
    if(cursorDownTargetsList.contains(entry)) {
        throw LogicException(LOC);
    }

    entry->setIsCursorButtonPressed(cursorHD->cursorIndex, buttonId, true);
    cursorDownTargetsList.appendReference(entry);
}

void CursorHandlingData::ButtonHD::removeCursorDownTarget(IInteractiveDrawable2D* entry) {
    int i = cursorDownTargetsList.indexOf(entry);
    if(i < 0) {
        throw LogicException(LOC);
    }

    entry->setIsCursorButtonPressed(cursorHD->cursorIndex, buttonId, false);
    cursorDownTargetsList.remove(i);
}

void CursorHandlingData::ButtonHD::addCursorDownHookingTarget(IInteractiveDrawable2D* entry) {
    if(cursorDownHookingTargetsList.contains(entry)) {
        throw LogicException(LOC);
    }

    cursorDownHookingTargetsList.appendReference(entry);
}

CursorHandlingData::ButtonHD::~ButtonHD() {
    //void
}
