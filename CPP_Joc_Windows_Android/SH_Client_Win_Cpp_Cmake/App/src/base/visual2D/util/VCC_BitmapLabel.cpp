#include "VCC_BitmapLabel.h"

// NOTE. Characters must not appear multiple times in a range.

std::string VCC_BitmapLabel::CharacterRanges::Part_Base = "\u0000";
std::string VCC_BitmapLabel::CharacterRanges::Part_SymbolsA = " .-,:;";
std::string VCC_BitmapLabel::CharacterRanges::Part_SymbolsB = "_+=!@#$%^&*()[]{}'\"\\|<>?/~`\u2026";
std::string VCC_BitmapLabel::CharacterRanges::Part_A = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
std::string VCC_BitmapLabel::CharacterRanges::Part_a = "abcdefghijklmnopqrstuvwxyz";
std::string VCC_BitmapLabel::CharacterRanges::Part_N = "0123456789";

std::string VCC_BitmapLabel::CharacterRanges::A = Part_Base + Part_A;
std::string VCC_BitmapLabel::CharacterRanges::a = Part_Base + Part_a;
std::string VCC_BitmapLabel::CharacterRanges::NS = Part_Base + Part_N + Part_SymbolsA;
std::string VCC_BitmapLabel::CharacterRanges::NSO = NS + "O";
std::string VCC_BitmapLabel::CharacterRanges::AaNSS = Part_A + Part_a + Part_N + Part_Base + Part_SymbolsA + Part_SymbolsB;

std::string& VCC_BitmapLabel::CharacterRanges::fromId(std::string& id) {
    if(id == "A") {
        return A;
    }else if(id == "a") {
        return a;
    }else if(id == "NS") {
        return NS;
    }else if(id == "NSO") {
        return NSO;
    }else if(id == "AaNSS") {
        return AaNSS;
    }

    throw LogicException(LOC);
}

std::string VCC_BitmapLabel::FontWeightMainNames[9]{
    "Thin",
    "ExtraLight",
    "Light",
    "Regular",
    "Medium",
    "SemiBold",
    "Bold",
    "ExtraBold",
    "Black"
};

std::string& VCC_BitmapLabel::GetFontWeightNameFromWeightValue(int fontWeight) {
    int group = Math::floor(fontWeight / 100) - 1;
    if (group < 0 || group >= 9 || (group + 1) * 100 != fontWeight) {
        // Unhandler font group.
        throw LogicException(LOC);
    }

    return FontWeightMainNames[group];
}

int VCC_BitmapLabel::TYPE = super::NewType();

VCC_BitmapLabel::VCC_BitmapLabel(
    ManagedAssetPath* managedAssetPath,
    std::string initialText)
    : super(TYPE),
      managedAssetPath(managedAssetPath),
      initialText(initialText)
{
    //void
}

VCC_BitmapLabel* VCC_BitmapLabel::New(
    ManagedAssetPath* managedAssetPath,
    std::string initialText)
{
    VCC_BitmapLabel* ret = new VCC_BitmapLabel(
        managedAssetPath,
        initialText
    );

    return ret;
}

VCC_BitmapLabel::~VCC_BitmapLabel() {
    //void
}
