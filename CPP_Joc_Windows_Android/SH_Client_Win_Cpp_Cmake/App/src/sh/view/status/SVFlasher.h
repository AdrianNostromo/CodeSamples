#pragma once

#include <base/interaction/CbTouchListener.h>
#include <graphics/visual/drawable/PseudoVisualData.h>
#include "SVJitIndicator.h"

namespace sh {
class SVFlasher : public SVJitIndicator {
private: typedef SVJitIndicator super;
private:
    class FlashPhases {
    public:
        static const int Iddle = -1;
        static const int Phase_1 = 0;
        static const int Phase_2 = 1;
        static const int Phase_3 = 2;
        static const int Phase_4 = 3;
        static const int Phase_5 = 4;
        static const int Phase_6 = 5;
    };

    static const float phaseSpeeds[];
protected:
    static MenuItemConfig* new_viewItemConfig_flasher();

private:
    ISizedDrawable2D* flasherSprite;
    PseudoVisualData* props;

    int activeFlashPhase = FlashPhases::Iddle;
    float activeFlashCp = 0.0f;

    float flash_screenWidth = 0.0f;
    float flash_screenHeight = 0.0f;

public:
    SVFlasher(SVFlasher const&) = delete;
    SVFlasher(SVFlasher &&) = default;
    SVFlasher& operator=(SVFlasher const&) = delete;
    SVFlasher& operator=(SVFlasher &&) = default;

    explicit SVFlasher(
        IAppSquareHeads* app,
        ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList);

    void visualLoop(float deltaS, int deltaMS) override;

    ~SVFlasher() override;
protected:
    void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

    void syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) override;

    void startFlash() override;

};
};
