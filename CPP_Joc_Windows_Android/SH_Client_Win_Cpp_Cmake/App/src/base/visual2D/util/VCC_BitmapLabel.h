#pragma once

#include "VCC_Touchable.h"
#include <string>
#include <base/assets/util/ManagedAssetPath.h>

class VCC_BitmapLabel : public VCC_Touchable {priv typedef VCC_Touchable super;pub dCtor(VCC_BitmapLabel);
    pub class CharacterRanges {
        priv static std::string Part_Base;
        priv static std::string Part_SymbolsA;
        priv static std::string Part_SymbolsB;
        priv static std::string Part_A;
        priv static std::string Part_a;
        priv static std::string Part_N;

        pub static std::string A;
        pub static std::string a;
        pub static std::string NS;
        pub static std::string NSO;
        pub static std::string AaNSS;

        pub static std::string& fromId(std::string& id);

    };

    pub enum FontWeight {
        Thin = 100,
        ExtraLight = 200, UltraLight = 200,
        Light = 300,
        Regular = 400, Normal = 400,
        Medium = 500,
        SemiBold = 600, DemiBold = 600,
        Bold = 700,
        ExtraBold = 800, UltraBold = 800,
        Black = 900, Heavy = 900,
        ExtraBlack = 950, UltraBlack = 950
    };
    pub static std::string FontWeightMainNames[9];
    pub static std::string& GetFontWeightNameFromWeightValue(int fontWeight);

    pub static int TYPE;

    pub ManagedAssetPath* managedAssetPath;

    pub std::string initialText;

    priv explicit VCC_BitmapLabel(
        ManagedAssetPath* managedAssetPath,
        std::string initialText);

    pub static VCC_BitmapLabel* New(
        ManagedAssetPath* managedAssetPath,
        std::string initialText);

    pub ~VCC_BitmapLabel() override;
};
