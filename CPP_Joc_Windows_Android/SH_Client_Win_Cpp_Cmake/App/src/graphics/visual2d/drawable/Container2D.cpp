#include "Container2D.h"
#include <assert.h>
#include <base/math/util/BoolUtil.h>
#include <base/geometry/Shape2D.h>
#include <base/input/util/TouchEventType.h>
#include <base/exceptions/LogicException.h>
#include <graphics/visual2d/util/WrappedTouchListener2D.h>
#include <base/exceptions/LogicException.h>
#include <base/input/util/Touch.h>
#include <graphics/visual2d/drawable/IRoot2D.h>

Container2D::Container2D()
	:super()
{
	//void
}

void Container2D::childSyncDrawSpecsRequested() {
	isAnyChildSyncDrawSpecsRequired = true;

	if (getParent() != nullptr) {
		getParent()->childSyncDrawSpecsRequested();
	}
}

void Container2D::addChild(IDrawableNode2D* drawable) {
	if (drawable == this || drawable->getInParent_drawablesListEntryRef() != nullptr
		|| drawable->getInParent_drawablesWithInputInterractionListEntryRef() != nullptr)
	{
		throw LogicException(LOC);
	}

	if (drawable->getParent()) {
		drawable->getParent()->removeChild(drawable);
	}

	drawable->getInParent_drawablesListEntryRef() = drawablesList.appendDirect(drawable);

	drawable->setParent(this);
	drawable->setRoot(getRoot());
}

void Container2D::removeChild(IDrawableNode2D* drawable) {
	if (drawable->getParent() != this || drawable->getInParent_drawablesListEntryRef() == nullptr) {
		throw LogicException(LOC);
	}

	// Root must be removed before parent to handle the case where the drawable is in a root_stage list so it has to be removed from that global system before it can be removed from it's local parent.
	drawable->setRoot(nullptr);
	drawable->setParent(nullptr);

	if (drawable->getInParent_drawablesWithInputInterractionListEntryRef() != nullptr) {
		throw LogicException(LOC);
	}

	drawable->getInParent_drawablesListEntryRef()->remove();
	drawable->getInParent_drawablesListEntryRef() = nullptr;
}

IDrawableNode2D* Container2D::onChildAddingToRendererAndGetPrevDrawableWithRenderData(IDrawableNode2D* leafDrawableOrFirstLevelLocalChild) {
	if (leafDrawableOrFirstLevelLocalChild == nullptr || leafDrawableOrFirstLevelLocalChild->getParent() != this) {
		throw LogicException(LOC);
	}
	
	IDrawableNode2D* prevDrawableWithRenderData = nullptr;
	if (drawablesWithRenderDataList.count() > 0) {
		// This container has a drawable with render data in a leaf (guaranteed), before the drawable or after or both. Find the correct insertion location.
		// Need to enumerate all children before and after the drawable to find a insertion location. It is guaranteed that at least 1 location is found.
		// Enumerate to the left.
		for (ListDL<IDrawableNode2D*>::Entry* entry = leafDrawableOrFirstLevelLocalChild->getInParent_drawablesListEntryRef()->prev; entry != nullptr; entry = entry->prev) {
			if (entry->data->getInParent_drawablesWithRenderDataListEntryRef() != nullptr) {
				// If the entry is a drawable, grab it directly or if it is a container, grab the max drawable leaf that has rd.
				prevDrawableWithRenderData = entry->data->getDisplayTreeMaxLeafDrawableWithRenderData();

				leafDrawableOrFirstLevelLocalChild->getInParent_drawablesWithRenderDataListEntryRef() = drawablesWithRenderDataList.insertAfterEmplace(
					entry->data->getInParent_drawablesWithRenderDataListEntryRef(),
					leafDrawableOrFirstLevelLocalChild
				);

				break;
			}
		}
		if (prevDrawableWithRenderData == nullptr) {
			// Enumerate to the right/
			for (ListDL<IDrawableNode2D*>::Entry* entry = leafDrawableOrFirstLevelLocalChild->getInParent_drawablesListEntryRef()->next; entry != nullptr; entry = entry->next) {
				if (entry->data->getInParent_drawablesWithRenderDataListEntryRef() != nullptr) {
					// If the entry is a drawable, grab it directly or if it is a container, grab the min drawable leaf that has rd.
					/// Also do the ->prev->data on the entry.
					IDrawableNode2D* nextDrawableWithRenderData = entry->data->getDisplayTreeMinLeafDrawableWithRenderData();
					ListDL<IDrawableNode2D*>::Entry*& nextDrawableWithRenderDataListEntry = nextDrawableWithRenderData->getInParent_drawablesWithRenderDataListEntryRef();
					if (nextDrawableWithRenderDataListEntry->prev != nullptr) {
						prevDrawableWithRenderData = nextDrawableWithRenderDataListEntry->prev->data;
					}

					leafDrawableOrFirstLevelLocalChild->getInParent_drawablesWithRenderDataListEntryRef() = drawablesWithRenderDataList.insertBeforeEmplace(
						entry->data->getInParent_drawablesWithRenderDataListEntryRef(),
						leafDrawableOrFirstLevelLocalChild
					);

					break;
				}
			}
		}
	} else {
		// This container has no children with rd so it is required to go up (if this is not the stage) to the parent and try to insert this in it as a entry with rd (do this recursively until there is a containerst connection path to the stage).
		if (getInParent_drawablesWithRenderDataListEntryRef() != nullptr) {
			// This must be nullptr because there are not children with rd.
			throw LogicException(LOC);
		}

		// This container has no drawable with render data. Need to go up (until the stage) until a parent with a drawable_with_rd occurs and grab a insertion location.
		/// This container gets inserted in the parent drawablesWithRenderDataList.
		if (getParent() == nullptr && static_cast<IContainer2D*>(this) != getRoot()) {
			// This container must have a parent at this location unless it is the root stage.
			throw LogicException(LOC);
		}
		if (getParent() != nullptr) {
			prevDrawableWithRenderData = getParent()->onChildAddingToRendererAndGetPrevDrawableWithRenderData(this);

			if (getInParent_drawablesWithRenderDataListEntryRef() == nullptr) {
				// This container is now in the parent drawablesWithRDList because of the above call.
				throw LogicException(LOC);
			}
		} else {
			// This is the stage and currently are adding the first drawable_with_rd leaf entry.
			prevDrawableWithRenderData = nullptr;
		}

		/// This container has no drawable_with_rd children and will now insert teh first one.
		leafDrawableOrFirstLevelLocalChild->getInParent_drawablesWithRenderDataListEntryRef() = drawablesWithRenderDataList.insertBeforeEmplace(
			nullptr/*existingEntry*/,
			leafDrawableOrFirstLevelLocalChild
		);
	}

	return prevDrawableWithRenderData;
}

void Container2D::onChildRemovedFromRenderer(IDrawableNode2D* drawable) {
	if (drawable == nullptr || drawable->getParent() != this) {
		throw LogicException(LOC);
	}
	
	if (drawable->getInParent_drawablesWithRenderDataListEntryRef() == nullptr) {
		throw LogicException(LOC);
	}

	drawable->getInParent_drawablesWithRenderDataListEntryRef()->remove();
	drawable->getInParent_drawablesWithRenderDataListEntryRef() = nullptr;

	if (drawablesWithRenderDataList.count() == 0) {
		// This container no longer is part of the drawable_with_renderData so update that info on the parent.
		// Note. On normal drawables, the root is nullptr at this location but this getRoot() check works for the stage check because the stage.getRoot() is never made nullptr;
		if (getParent() == nullptr && static_cast<IContainer2D*>(this) != getRoot()) {
			// This function must always be called when a parent and root are available.
			throw LogicException(LOC);
		}

		if (getParent() != nullptr) {
			// Add a extra parent check for the case when this is the stage and has no parent.
			getParent()->onChildRemovedFromRenderer(this);
		}
	}
}

void Container2D::onGlobal_isVisibleChanged() {
    bool global_isVisible = getGlobal_isVisible();
	for (ListDL<IDrawableNode2D*>::Entry* entry = drawablesList.getFirst(); entry != nullptr; entry = entry->next) {
		IDrawableNode2D* childEntry = entry->data;

		childEntry->setParentGlobal_isVisible(global_isVisible);
    }

    super::onGlobal_isVisibleChanged();
}

void Container2D::onRootChanged() {
	super::onRootChanged();

    IRoot2D* root = getRoot();
	for (ListDL<IDrawableNode2D*>::Entry* entry = drawablesList.getFirst(); entry != nullptr; entry = entry->next) {
		IDrawableNode2D* childEntry = entry->data;

		childEntry->setRoot(root);
	}
}

void Container2D::onIsVisibleChanged() {
	super::onIsVisibleChanged();

	dispatchTouchBoxChangedIsPossible(true);
}

bool Container2D::getIsInputInteractive() {
	return (drawablesWithInputInterractionList.count() > 0) ? true : false;
};

void Container2D::onChildIsInputInteractiveChanged(IDrawableNode2D* drawable, bool childIsInputInteractive) {
	if(!childIsInputInteractive) {
		if (drawable->getInParent_drawablesWithInputInterractionListEntryRef() == nullptr) {
			throw LogicException(LOC);
		}

		drawable->getInParent_drawablesWithInputInterractionListEntryRef()->remove();
		drawable->getInParent_drawablesWithInputInterractionListEntryRef() = nullptr;

		if(drawablesWithInputInterractionList.count() == 0) {
			onIsInputInteractiveChanged();
		}
	}else {
		if (drawable->getInParent_drawablesWithInputInterractionListEntryRef() != nullptr) {
			throw LogicException(LOC);
		}

		{
			// Do the entry inserting in the special list drawablesWithInputInterractionList.
			/// Do insert in that list, need to enumerate all left and/or right entries from drawablesList until a drawable with a drawablesWithInputInterractionList entry is found.
			/// This insertion is slow that desired but avoids enumerating all drawables when a touch occurs.

			// Enumerate left entries.
			for (ListDL<IDrawableNode2D*>::Entry* entry = drawable->getInParent_drawablesListEntryRef()->prev; entry != nullptr; entry = entry->prev) {
				if (entry->data->getInParent_drawablesWithInputInterractionListEntryRef() != nullptr) {
					// Insert to the right of the entry.
					drawable->getInParent_drawablesWithInputInterractionListEntryRef() = drawablesWithInputInterractionList.insertAfterEmplace(entry->data->getInParent_drawablesWithInputInterractionListEntryRef(), drawable);

					break;
				}
			}

			if (drawable->getInParent_drawablesWithInputInterractionListEntryRef() == nullptr) {
				// Enumerate right entries.
				for (ListDL<IDrawableNode2D*>::Entry* entry = drawable->getInParent_drawablesListEntryRef()->next; entry != nullptr; entry = entry->next) {
					if (entry->data->getInParent_drawablesWithInputInterractionListEntryRef() != nullptr) {
						// Insert to the left of the entry.
						drawable->getInParent_drawablesWithInputInterractionListEntryRef() = drawablesWithInputInterractionList.insertBeforeEmplace(entry->data->getInParent_drawablesWithInputInterractionListEntryRef(), drawable);

						break;
					}
				}
			}

			if (drawable->getInParent_drawablesWithInputInterractionListEntryRef() == nullptr) {
				// This is the only drawable with input enabled, prepend it.
				drawable->getInParent_drawablesWithInputInterractionListEntryRef() = drawablesWithInputInterractionList.prependEmplace(drawable);
			}
		}

		if(drawablesWithInputInterractionList.count() == 1) {
			onIsInputInteractiveChanged();
		}
	}
};

void Container2D::onTouchBoxChanged() {
	// Set a flag.
	isTouchBoxChanged = true;

	dispatchTouchBoxChangedIsPossible(false);
}

bool Container2D::getIsTouchBoxChanged() {
	return isTouchBoxChanged;
}

void Container2D::clearIsTouchBoxChanged() {
	isTouchBoxChanged = false;
}

IDrawableNode2D* Container2D::getDisplayTreeMinLeafDrawableWithRenderData() {
	if (getInParent_drawablesWithRenderDataListEntryRef() == nullptr || drawablesWithRenderDataList.count() == 0) {
		// This function is never called unless the drawable is in the list_with_render_data.
		throw LogicException(LOC);
	}

	return drawablesWithRenderDataList.getFirst()->data->getDisplayTreeMinLeafDrawableWithRenderData();
}

IDrawableNode2D* Container2D::getDisplayTreeMaxLeafDrawableWithRenderData() {
	if (getInParent_drawablesWithRenderDataListEntryRef() == nullptr || drawablesWithRenderDataList.count() == 0) {
		// This function is never called unless the drawable is in the list_with_render_data.
		throw LogicException(LOC);
	}

	return drawablesWithRenderDataList.getLast()->data->getDisplayTreeMaxLeafDrawableWithRenderData();
}

void Container2D::dispatchTouchBoxChangedIsPossible(bool skipVisibleCheck) {
	if(isTouchBoxChanged && getIsInputInteractive() && (skipVisibleCheck || getIsVisible()) && getParent() != nullptr) {
		getParent()->onTouchBoxChanged();
	}
}

void Container2D::onParentChangePre() {
	super::onParentChangePre();

	if(getParent() != nullptr && getIsInputInteractive()) {
		getParent()->onChildIsInputInteractiveChanged(this, false);
	} else if (getInParent_drawablesWithInputInterractionListEntryRef() != nullptr) {
		throw LogicException(LOC);
	}

	dispatchTouchBoxChangedIsPossible(false);
}

void Container2D::onParentChanged() {
	super::onParentChanged();

	if(getParent() != nullptr && getIsInputInteractive()) {
		getParent()->onChildIsInputInteractiveChanged(this, true);
	} else if (getInParent_drawablesWithInputInterractionListEntryRef() != nullptr) {
		throw LogicException(LOC);
	}

	dispatchTouchBoxChangedIsPossible(false);
}

void Container2D::onIsInputInteractiveChanged() {
	if(!getIsInputInteractive()) {
		if(getParent() != nullptr) {
			getParent()->onChildIsInputInteractiveChanged(this, false);
		} else if (getInParent_drawablesWithInputInterractionListEntryRef() != nullptr) {
			throw LogicException(LOC);
		}
	}else {
		if(getParent() != nullptr) {
			getParent()->onChildIsInputInteractiveChanged(this, true);
		} else if (getInParent_drawablesWithInputInterractionListEntryRef() != nullptr) {
			throw LogicException(LOC);
		}
	}

	dispatchTouchBoxChangedIsPossible(false);
}

int Container2D::syncDrawSpecsWhereNeeded(const int parentInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D** preRenderData) {
	// Compute here because invalidationBitMap becomes 0 in the super call.
	int combinedInvalidationBitMap = BoolUtil::combine(
		invalidationBitMap,
		parentInvalidationBitMap
	);

	int r = super::syncDrawSpecsWhereNeeded(parentInvalidationBitMap, renderer, preRenderData);
	if (r != 0) {
		return r;
	}

	if (combinedInvalidationBitMap != 0 || isAnyChildSyncDrawSpecsRequired) {
		if (getIsVisible() && (getParent() || static_cast<IContainer2D*>(this) == getRoot())) {
			for (ListDL<IDrawableNode2D*>::Entry* entry = drawablesList.getFirst(); entry != nullptr; entry = entry->next) {
				IDrawableNode2D* childEntry = entry->data;

				if (combinedInvalidationBitMap != 0 || childEntry->isSyncDrawSpecsRequired()) {
					if (childEntry->syncDrawSpecsWhereNeeded(
						combinedInvalidationBitMap,
						renderer,
						preRenderData
					) != 0) {
						return -1;
					}
					// "preRenderData" is set by the "syncDrawSpecsWhereNeeded" call always. Use this update method so the stage can update "preRenderData" also.
				}
			}
		}

		isAnyChildSyncDrawSpecsRequired = false;
	}

	return 0;
}

Affine2& Container2D::getGlobalTransform(bool updateTransformStackIfDirty) {
	return super::getGlobalTransform(updateTransformStackIfDirty);
}

float Container2D::getGlobalAlpha() {
	return super::getGlobalAlpha();
}

Color& Container2D::getGlobalTintColor() {
	return super::getGlobalTintColor();
}

bool Container2D::isSyncDrawSpecsRequired() {
	bool b = super::isSyncDrawSpecsRequired();
	if (!b && isAnyChildSyncDrawSpecsRequired) {
		return true;
	}

	return b;
}

bool Container2D::worldTouchEvent(
	TouchEventType* eventType,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2* deltaPosW,
	base::CursorHandlingData* cursorHD, base::CursorHandlingData::ButtonHD* cursorButtonHD,
	int eventIndex)
{
	if(cursorHD == nullptr) {
		// The new system requires this value to be valid always.
		throw LogicException(LOC);
	}

	int index = drawablesWithInputInterractionList.count();
	for (ListDL<IDrawableNode2D*>::Entry* listEntry = drawablesWithInputInterractionList.getLast(); listEntry != nullptr; listEntry = listEntry->prev) {
		index--;

		IDrawableNode2D* childEntry = listEntry->data;

        if(childEntry != nullptr) {
            IContainer2D* entry_container2D;
            if((entry_container2D = dynamic_cast<IContainer2D*>(childEntry))) {
                IContainer2D* entry = entry_container2D;

                entry->clearIsTouchBoxChanged();

                if(entry->getIsVisible() && entry->getIsInputInteractive()) {
					bool b = entry->worldTouchEvent(
						eventType,
						cursorIndex, buttonId,
						screenPos, deltaPosW,
						cursorHD, cursorButtonHD,
						eventIndex
					);

                    if(b) {
                        return true;
                    }
                }
            }

            IInteractiveDrawable2D* entry_interactiveDrawable;
            if((entry_interactiveDrawable = dynamic_cast<IInteractiveDrawable2D*>(childEntry))) {
                IInteractiveDrawable2D* entry = entry_interactiveDrawable;

	            entry->clearIsTouchBoxChanged();

                if(entry->getIsVisible() && entry->getIsInputInteractive() && entry->getIsGlobalTransformInit()) {
					Vector2 localPos{};
					entry->worldPosToLocalPos(screenPos, localPos);
	                std::shared_ptr<Shape2D> touchShape = entry->getTouchShape();

                    if(touchShape != nullptr && touchShape->contains(localPos)) {
						// The drawable is input interractive with at least 1 mouse button (guaranteed because it is in the drawablesWithInterractions list).
                        if (!entry->getIsInputFrozen() && (buttonId->index < 0 || BoolUtil::isBitAtIndex(entry->getInputInterractiveButtonsBitMap(), buttonId->index))) {
                        	callTouchListener(
		                        cursorHD, cursorButtonHD,
                                eventType, entry->getWrappedTouchListenersList(), entry,
	                            cursorIndex, buttonId,
                                screenPos, localPos,
								deltaPosW,
                                eventIndex
                            );
                        }

	                    if(!entry->getIsTouchFallthroughEnabled()) {
		                    return true;
	                    }
                    }
                }
            }
        }
    }

    return false;
}

void Container2D::callTouchListener(
	base::CursorHandlingData* cursorHD, base::CursorHandlingData::ButtonHD* cursorButtonHD,
	TouchEventType* eventType, ArrayList<WrappedTouchListener2D*>* wrappedTouchListenersList, IInteractiveDrawable2D* entry,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos,
	Vector2* deltaPosW,
    int eventIndex)
{
    if(eventType == TouchEventType::CURSOR_DOWN) {
        bool isTouchDownHookingRequired = false;

		bool b = entry->onCursorDown(
			entry,
			cursorIndex, buttonId,
			screenPos, localPos
		);
		if (b) {
			isTouchDownHookingRequired = true;
		}
	    if(entry->getIsEventsEnabled()) {
		    for (int i = 0; i < wrappedTouchListenersList->size(); i++) {
			    WrappedTouchListener2D *wrappedTouchListener = *wrappedTouchListenersList->getPointer(i);

			    bool b = wrappedTouchListener->listener->onCursorDown(
				    entry,
				    cursorIndex, buttonId,
					screenPos, localPos
			    );

			    if (b) {
				    isTouchDownHookingRequired = true;
				    wrappedTouchListener->putIsListenerHookingTouchDown(cursorIndex, true);
			    } else {
				    wrappedTouchListener->putIsListenerHookingTouchDown(cursorIndex, false);
			    }
		    }
	    }
	    
        if(isTouchDownHookingRequired) {
			cursorButtonHD->addCursorDownHookingTarget(entry);
        }

	    cursorButtonHD->addCursorDownTarget(entry);
    }else if(eventType == TouchEventType::CURSOR_UP) {
		entry->onCursorUp(
			entry,
			cursorIndex, buttonId,
			screenPos, localPos
		);
		if(entry->getIsEventsEnabled()) {
		    for (int i = 0; i < wrappedTouchListenersList->size(); i++) {
			    WrappedTouchListener2D *wrappedTouchListener = *wrappedTouchListenersList->getPointer(i);

			    wrappedTouchListener->listener->onCursorUp(
				    entry,
				    cursorIndex, buttonId,
					screenPos, localPos
			    );
		    }
	    }
	    
        bool isInTouchDownTargetsList = cursorButtonHD->cursorDownTargetsList.contains(entry);
        if (isInTouchDownTargetsList) {
			entry->onCursorTap(
				entry,
				cursorIndex, buttonId,
				screenPos, localPos
			);
	        if(entry->getIsEventsEnabled()) {
		        for (int i = 0; i < wrappedTouchListenersList->size(); i++) {
			        WrappedTouchListener2D *wrappedTouchListener = *wrappedTouchListenersList->getPointer(i);

			        wrappedTouchListener->listener->onCursorTap(
				        entry,
				        cursorIndex, buttonId,
						screenPos, localPos
			        );
		        }
	        }
        }

        if (isInTouchDownTargetsList) {
            cursorButtonHD->removeCursorDownTarget(entry);
        }
    }else if(eventType == TouchEventType::CURSOR_MOVE) {
		if (deltaPosW == nullptr) {
			throw LogicException(LOC);
		}

        // All targets that are under cursor receive move events. Even ones that have drag in progress and are under cursor.
		entry->onCursorMove(
			entry,
			cursorIndex,
			screenPos, localPos,
			*deltaPosW
		);
	    if(entry->getIsEventsEnabled()) {
		    for (int i = 0; i < wrappedTouchListenersList->size(); i++) {
			    WrappedTouchListener2D *wrappedTouchListener = *wrappedTouchListenersList->getPointer(i);
			    wrappedTouchListener->listener->onCursorMove(
				    entry,
				    cursorIndex,
					screenPos, localPos,
					*deltaPosW
			    );
		    }
	    }
	    
        // Set a flag on the target with the current move event index. That will be used to detect cursor_out.
        entry->setCursorMoveEventIndex(eventIndex);

        // Handle cursor over.
        if(!entry->getIsCursorOver(cursorIndex)) {
            callTouchListener(
	            cursorHD, nullptr,
	            TouchEventType::CURSOR_OVER, wrappedTouchListenersList, entry,
	            cursorIndex, base::Touch::ButtonCode::None,
				screenPos, localPos,
				deltaPosW,
	            eventIndex
            );
        }
    }else if(eventType == TouchEventType::CURSOR_OVER) {
	    // Add entry to cursor over list and set the entry over state.
	    cursorHD->addCursorOverTarget(entry);

	    // Dispatch a cursor_over event.
		entry->onCursorOver(
			entry,
			cursorIndex,
			screenPos, localPos
		);
	    if(entry->getIsEventsEnabled()) {
		    for (int i = 0; i < wrappedTouchListenersList->size(); i++) {
			    WrappedTouchListener2D *wrappedTouchListener = *wrappedTouchListenersList->getPointer(i);
			    wrappedTouchListener->listener->onCursorOver(
				    entry,
				    cursorIndex,
					screenPos, localPos
			    );
		    }
	    }
    }else if(eventType == TouchEventType::CURSOR_OUT) {
	    cursorHD->removeCursorOverTarget(entry);

	    // Dispatch a cursor_out event.
		entry->onCursorOut(
			entry,
			cursorIndex,
			screenPos, localPos
		);
	    if(entry->getIsEventsEnabled()) {
		    for (int i = 0; i < wrappedTouchListenersList->size(); i++) {
			    WrappedTouchListener2D *wrappedTouchListener = *wrappedTouchListenersList->getPointer(i);
			    wrappedTouchListener->listener->onCursorOut(
				    entry,
				    cursorIndex,
					screenPos, localPos
			    );
		    }
	    }
    }else {
        throw LogicException(LOC);
    }
}

void Container2D::disposeMain() {
	// Remove all children.
	ListDL<IDrawableNode2D*>::Entry* entry = nullptr;
	while((entry = drawablesList.getFirst()) != nullptr) {
		IDrawableNode2D *childEntry = entry->data;

		removeChild(childEntry);
	}

	super::disposeMain();
}

Container2D::~Container2D() {
    //void
}
