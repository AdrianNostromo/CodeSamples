#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include "MIGUIV_DirectionalPad.h"
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Touchable.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/math/Math.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace rpg3D;

MenuItemConfig* MIGUIV_DirectionalPad::new_viewItemConfig_dPad() {
	return new MenuItemConfig_ComposedVisual("dPad", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 100.0f, "_dpm.width")
			->put("height", 100.0f, "_dpm.height")

			// 56.0f Is the height for a toolbar and a good extra margin.
			->put("marginLeft", 12.0f + 56.0f + 30.0f, "_dpm.width")
			->put("marginBottom", 12.0f + 56.0f + 30.0f, "_dpm.height")

			->put("maxKnobRadiusHorizontal", 100.0f / 2.0f, "_dpm.width")
			->put("maxKnobRadiusVertical", 100.0f / 2.0f, "_dpm.height")
		))
		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x",
				0.5f, "props.marginLeft",
				VisualValueConfig::OPERATION_ADD,
				0.5f, "props.width"
			)
			->put("y",
				0.5f, "props.marginBottom",
				VisualValueConfig::OPERATION_ADD,
				0.5f, "props.height"
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("taBg", "layer", VCC_TouchArea::New(
			VCC_Touchable::TouchShape::Circle//radius = (width+height)/2f;
		), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("knobHolder", "layer", VCC_Group::New(), (new VisualPropertiesConfig())
			// manual x
			// manual y
		))
		->appendDirect_chain(new MenuItemConfig_Visual("knobBg", "knobHolder", VCC_Sprite::New(
            "visual2D/taGeneral.atlas", "rpg3D/gw/mainUi/dpad/knob"
        ), (new VisualPropertiesConfig())
			->put("width", 60.0f, "_dpm.width")
			->put("height", 60.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("tintColor", Color("#FFFFFFFF"))
			->put("alpha", 0.8f)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("knobOutline", "knobHolder", VCC_Sprite::New(
            "visual2D/taGeneral.atlas", "rpg3D/gw/mainUi/dpad/knobOutline"
        ), (new VisualPropertiesConfig())
			->put("width", 60.0f, "_dpm.width")
			->put("height", 60.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("tintColor", Color("#000000FF"))
			->put("alpha", 0.4f)
		))
	);
}

MIGUIV_DirectionalPad::MIGUIV_DirectionalPad(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw)
    : super(app, viewItemConfigs, viewEaseDurationsSList, gw)
{
    dPadTouchListener.cb_onCursorDown = std::bind(&MIGUIV_DirectionalPad::dPad_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
    dPadTouchListener.cb_onCursorDrag = std::bind(&MIGUIV_DirectionalPad::dPad_onCursorDrag, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6);
    dPadTouchListener.cb_onCursorUp = std::bind(&MIGUIV_DirectionalPad::dPad_onCursorUp, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
    dPadTouchListener.cb_onCursorUpOutside = std::bind(&MIGUIV_DirectionalPad::dPad_onCursorUpOutside, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

bool MIGUIV_DirectionalPad::dPad_onCursorDown(
    IInteractiveDrawable2D* target,
    int cursorIndex, base::Touch::ButtonCode* buttonId,
    Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
    if(!isActive()) {
        return false;
    }

    if(isDragActive) {
    	// A drag already exists (Hopefully with a different buttonId, should never be with the same buttonId as the active drag).
    	return false;
    }

    lastDragPos.set(screenPos);

    currentDragWorldPos.set(screenPos);
    currentDragLocalPos.set(localPos);

    isDragEndProcessed = false;
    isTouchDownPulse = true;
    isDragActive = true;
	currentDragCursorIndex = cursorIndex;
	currentDragButtonId = buttonId;

    return true;
}

void MIGUIV_DirectionalPad::dPad_onCursorDrag(
    IInteractiveDrawable2D* target,
    int cursorIndex, base::Touch::ButtonCode* buttonId,
    Vector2& screenPos, Vector2& localPos,
	Vector2& deltaPosW)
{
	if(!isActive()) {
        return ;
    }

    if(isDragActive && currentDragCursorIndex == cursorIndex && currentDragButtonId == buttonId) {
        lastDragPos.set(currentDragWorldPos.x, currentDragWorldPos.y);
        currentDragWorldPos.set(screenPos);
        currentDragLocalPos.set(localPos);
    }
}

void MIGUIV_DirectionalPad::dPad_onCursorUp(
    IInteractiveDrawable2D* target,
    int cursorIndex, base::Touch::ButtonCode* buttonId,
    Vector2& screenPos, Vector2& localPos)
{
	if(!isActive()) {
        return ;
    }

    if(isDragActive && currentDragCursorIndex == cursorIndex && currentDragButtonId == buttonId) {
        lastDragPos.set(currentDragWorldPos.x, currentDragWorldPos.y);
        currentDragWorldPos.set(screenPos);
        currentDragLocalPos.set(localPos);

        isDragActive = false;
    }
}

void MIGUIV_DirectionalPad::dPad_onCursorUpOutside(
    IInteractiveDrawable2D* target,
    int cursorIndex, base::Touch::ButtonCode* buttonId,
    Vector2& screenPos, Vector2& localPos)
{
	if(!isActive()) {
        return ;
    }

    if(isDragActive && currentDragCursorIndex == cursorIndex && currentDragButtonId == buttonId) {
        lastDragPos.set(currentDragWorldPos.x, currentDragWorldPos.y);
        currentDragWorldPos.set(screenPos);
        currentDragLocalPos.set(localPos);

        isDragActive = false;
    }
}

void MIGUIV_DirectionalPad::createPost() {
	super::createPost();

	//dp_knobHolder->setIsVisible(false);
	dp_knobHolder->setPosition(0.0f, 0.0f);
	dp_knobHolder->setAlpha(0.6f);
}

void MIGUIV_DirectionalPad::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

    (dp_taBg = getItemOptionalAs<IInteractiveDrawable2D*>("dPad.taBg"))->addTouchListener(&dPadTouchListener);
    dp_knobHolder = getItemOptionalAs<IDrawableNode2D*>("dPad.knobHolder");
}

void MIGUIV_DirectionalPad::setListenerDPad(IListenerDPad* listenerDPad) {
	this->listenerDPad = listenerDPad;
}

void MIGUIV_DirectionalPad::onSelectedStateChanged() {
    super::onSelectedStateChanged();

	cancelActiveDrag();
}

void MIGUIV_DirectionalPad::cancelActiveDrag() {
    if(isDragActive) {
        isDragActive = false;
    }
}

void MIGUIV_DirectionalPad::onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) {
    super::onGameLoop_localInput(deltaS, deltaMS, gameTimeS);

    if(isDragActive || isTouchDownPulse) {
        // Process active drag.

        if(isTouchDownPulse) {
            // Process first logic tick.

            // Show knob.
            //dp_knobHolder->setIsVisible(true);
			dp_knobHolder->setAlpha(1.0f);
        }

        float maxKnobRadiusHorizontal = getItemPropertyValue("dPad.props.maxKnobRadiusHorizontal", nullptr)->getDirectAs_float();
        float maxKnobRadiusVertical = getItemPropertyValue("dPad.props.maxKnobRadiusVertical", nullptr)->getDirectAs_float();

        float touchLocalDist = Math::dist(
            0.0f, 0.0f,
            currentDragLocalPos.x, currentDragLocalPos.y
        );

		float angR = Math::atan2(currentDragLocalPos.y, currentDragLocalPos.x);

        dp_knobHolder->setPosition(
            Math::cosRad(angR) * Math::min(touchLocalDist, maxKnobRadiusHorizontal),
            Math::sinRad(angR) * Math::min(touchLocalDist, maxKnobRadiusVertical)
        );

        isTouchDownPulse = false;

        lastDragPos.set(currentDragWorldPos.x, currentDragWorldPos.y);

        v1.set(currentDragLocalPos.x, currentDragLocalPos.y);// Set the direction.
        v1.nor();// Normalize to unit length.
	    v1.mul(touchLocalDist / Math::min(maxKnobRadiusHorizontal, maxKnobRadiusVertical));// Set the length to percentage mode.
	    v1.clipLength(1.0f);// Make sure the length is <=1.0f;

        if(!lastMoveDirVector.equals(v1)) {
        	lastMoveDirVector.set(v1.x, v1.y);

	        if(listenerDPad != nullptr) {
	        	listenerDPad->onSetMoveDirection(lastMoveDirVector.x, lastMoveDirVector.y);
	        }
        }
    }else if(!isDragEndProcessed) {
        // Process drag end.


        isDragEndProcessed = true;

        // Hide knob.
        //dp_knobHolder->setIsVisible(false);
		dp_knobHolder->setAlpha(0.6f);
		dp_knobHolder->setPosition(0.0f, 0.0f);

	    if(!lastMoveDirVector.equals(0.0f, 0.0f)) {
		    lastMoveDirVector.set(0.0f, 0.0f);

		    if (listenerDPad != nullptr) {
			    listenerDPad->onSetMoveDirection(lastMoveDirVector.x, lastMoveDirVector.y);
		    }
	    }
    }
}

MIGUIV_DirectionalPad::~MIGUIV_DirectionalPad() {
	//void
}
