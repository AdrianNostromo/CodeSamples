#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include "SVFlasher.h"
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_MovieClip.h>
#include <base/exceptions/LogicException.h>
#include <base/math/Math.h>
#include <base/ease/Ease.h>

using namespace sh;

const float SVFlasher::phaseSpeeds[] = {
    2.4f,
    2.4f,
    0.25f,
    2.4f,
    2.4f,
    0.25f
};

MenuItemConfig* SVFlasher::new_viewItemConfig_flasher() {
	return new MenuItemConfig_ComposedVisual("flasher", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("screenWidth", 1.0f, "_appSize.width")
			->put("screenHeight", 1.0f, "_appSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->put("y", 0.0f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("flasherSprite", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)

			->put("x", 0.5f, "parent.props.width")
			->put("y",
				1.0f, "parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				-4.0f - 52.0f / 2.0f, "_dpm.height"
			)
			->put("tintColor", Color("#DC0000FF"))
		))
	);
}

SVFlasher::SVFlasher(
    IAppSquareHeads* app,
    ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList)
    : super(app, viewItemConfigs, viewEaseDurationsSList)
{
    //void
}

void SVFlasher::createVisuals(Map1D<std::string, void*>* createConfigMap) {
    super::createVisuals(createConfigMap);

    flasherSprite = getItemOptionalAs<ISizedDrawable2D*>("flasher.flasherSprite");
    flasherSprite->setIsVisible(false);

    props = getItemOptionalAs<PseudoVisualData*>("flasher.props");
}

void SVFlasher::syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) {
    super::syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

    std::shared_ptr<base::IWrappedValue> screenWidth = props->getPropertyValue("screenWidth");
    flash_screenWidth = screenWidth->getDirectAs_float();
    std::shared_ptr<base::IWrappedValue> screenHeight = props->getPropertyValue("screenHeight");
    flash_screenHeight = screenHeight->getDirectAs_float();
}

void SVFlasher::startFlash() {
    bool wasFlahserInProgress = false;
    if(activeFlashPhase != FlashPhases::Iddle) {
        wasFlahserInProgress = true;
    }

    activeFlashCp = 0.0f;
    activeFlashPhase = FlashPhases::Phase_1;

    if(!wasFlahserInProgress) {
        addViewActiveElement();
    }

    // NOTE : If visual is made visible here there may be a frame with wrong visual pos,rot,alpha.
}

void SVFlasher::visualLoop(float deltaS, int deltaMS) {
    super::visualLoop(deltaS, deltaMS);

    if(activeFlashPhase != FlashPhases::Iddle) {
        if(!flasherSprite->getIsVisible()) {
            flasherSprite->setIsVisible(true);
        }

        activeFlashCp = Math::min(1.0f, activeFlashCp + deltaS * phaseSpeeds[activeFlashPhase]);

        if(activeFlashPhase != FlashPhases::Phase_1) {
            float cp = activeFlashCp;

            flasherSprite->setSize(
                Ease::computeEaseValue(Ease::easeOutCubic, cp,
                    0.0f, flash_screenHeight * 2.0f
                ),
                Ease::computeEaseValue(Ease::easeOutCubic, cp,
                    0.0f, flash_screenHeight * 2.0f
                )
            );

            flasherSprite->setAlpha(Ease::computeEaseValue(Ease::easeOutCubic, cp,
                0.6f, 0.0f
            ));
        }else if(activeFlashPhase != FlashPhases::Phase_2) {
            float cp = activeFlashCp;

            flasherSprite->setSize(
                Ease::computeEaseValue(Ease::easeOutCubic, cp,
                    0.0f, flash_screenHeight * 2.0f
                ),
                Ease::computeEaseValue(Ease::easeOutCubic, cp,
                    0.0f, flash_screenHeight * 2.0f
                )
            );

            flasherSprite->setAlpha(Ease::computeEaseValue(Ease::easeOutCubic, cp,
                0.6f, 0.0f
            ));
        }else if(activeFlashPhase != FlashPhases::Phase_3) {
            float cp = activeFlashCp;

            flasherSprite->setSize(
                Ease::computeEaseValue(Ease::easeInCubic, cp,
                    0.0f, flash_screenHeight * 2.0f
                ),
                Ease::computeEaseValue(Ease::easeInCubic, cp,
                    0.0f, flash_screenHeight * 2.0f
                )
            );

            flasherSprite->setAlpha(Ease::computeEaseValue(Ease::easeInQuint, cp,
                1.0f, 0.0f
            ));
        }else if(activeFlashPhase != FlashPhases::Phase_4) {
            float cp = activeFlashCp;

            flasherSprite->setSize(
                Ease::computeEaseValue(Ease::easeOutCubic, cp,
                    0.0f, flash_screenHeight * 2.0f
                ),
                Ease::computeEaseValue(Ease::easeOutCubic, cp,
                    0.0f, flash_screenHeight * 2.0f
                )
            );

            flasherSprite->setAlpha(Ease::computeEaseValue(Ease::easeOutCubic, cp,
                0.6f, 0.0f
            ));
        }else if(activeFlashPhase != FlashPhases::Phase_5) {
            float cp = activeFlashCp;

            flasherSprite->setSize(
                Ease::computeEaseValue(Ease::easeOutCubic, cp,
                    0.0f, flash_screenHeight * 2.0f
                ),
                Ease::computeEaseValue(Ease::easeOutCubic, cp,
                    0.0f, flash_screenHeight * 2.0f
                )
            );

            flasherSprite->setAlpha(Ease::computeEaseValue(Ease::easeOutCubic, cp,
                0.6f, 0.0f
            ));
        }else if(activeFlashPhase != FlashPhases::Phase_6) {
            float cp = activeFlashCp;

            flasherSprite->setSize(
                Ease::computeEaseValue(Ease::easeInCubic, cp,
                    0.0f, flash_screenHeight * 2.0f
                ),
                Ease::computeEaseValue(Ease::easeInCubic, cp,
                    0.0f, flash_screenHeight * 2.0f
                )
            );

            flasherSprite->setAlpha(Ease::computeEaseValue(Ease::easeInQuint, cp,
                1.0f, 0.0f
            ));
        }else {
            throw LogicException(LOC);
        }

        if(activeFlashCp >= 1.0f) {
            activeFlashCp = 0.0f;

            if (activeFlashPhase == FlashPhases::Phase_6) {
                activeFlashPhase = FlashPhases::Iddle;

                removeViewActiveElement();

                if(flasherSprite->getIsVisible()) {
                    flasherSprite->setIsVisible(false);
                }
            }else {
                activeFlashPhase++;
            }
        }
    }
}

SVFlasher::~SVFlasher() {
    //void
}
