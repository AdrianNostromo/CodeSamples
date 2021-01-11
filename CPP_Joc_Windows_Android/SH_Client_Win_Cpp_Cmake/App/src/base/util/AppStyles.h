#pragma once

class Color;

class MenuItemConfig;

class AppStyles {
public:
	static Color* THEME_COLOR;
    static Color* BG_COLOR_WHITE;
    static Color* BG_COLOR_GREY;
    static Color* BG_COLOR_DARK;
    static Color* BG_COLOR_BLACK;
    static Color* LIGHT_TEXT_COLOR;
    static Color* DARK_TEXT_COLOR;
    
    static Color* RED_COLOR;
    
    static Color* ICON_DARK_COLOR;
    static const float ICON_DARK_ALPHA;
    
    // This is used to scale sizes in elements (eg: Dialog width).
    // This is the same on all devices and desktop.
    static const float uiIncrementSizeDPM;
    
    static const float ICON_BLACK_ALPHA;
    
    static Color* DIVIDER_DARK_COLOR;
    static const float DIVIDER_DARK_ALPHA;
    static const float DIVIDER_THICKNESS_DPM;
    
    static const float DARK_PRIMARY_TEXT_ALPHA;
    static const float DARK_SECONDARY_TEXT_ALPHA;
    static const float DARK_DISABLED_TEXT_ALPHA;
    
    static const float LIGHT_PRIMARY_TEXT_ALPHA;
    static const float LIGHT_SECONDARY_TEXT_ALPHA;
    static const float LIGHT_DISABLED_TEXT_ALPHA;
    
    static const float SCREEN_DIMMER_ALPHA;
    static const float BG_LIGHT_ALPHA;
    static const float BG_DARK_ALPHA;
    
    static const float ANIMATION_COMPLEX_DURATION_S;
    static const float ANIMATION_ENTER_SCREEN_DURATION_S;
    static const float ANIMATION_LEAVE_SCREEN_DURATION_S;
    // This value is intuitive, not from material template.
    static const float ANIMATION_IN_STATE_CHANGE_DURATION;
    
    static const float ANIMATION_SWITCH_STATE_CHANGE;
    
    static const float SLIDER_MOVEMENT_SPEED_MULTIPLIER_S;
    
    static const float ANIMATION_DURATION_S_DEFAULT;
    
    static const float ANIMATION_SPLASH_SCREEN_OUT_DURATION_S;

	static MenuItemConfig* new_ITEM_CONFIG_shadow_CARD();
	static MenuItemConfig* new_ITEM_CONFIG_shadow_DIALOG();

private:
	static const float SHADOW_DIALOG_BASE_SIZE;

};
