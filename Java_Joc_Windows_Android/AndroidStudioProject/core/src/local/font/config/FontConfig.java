package local.font.config;

import com.badlogic.gdx.graphics.Color;

import java.util.HashMap;
import java.util.Map;

import base.font.util.FontSourceGenerated;

public class FontConfig {

    //asdqqqq;// remove this class and localize the fonts.

	private static final Map<String, FontConfig> entriesMap = new HashMap<String, FontConfig>();

    private static String CHARS_LIST_N = "\u00000123456789" + " .-,:;";
    // The letter O(capital o) may be used instead of the number 0(zero) because it is a circle and looks better.
    private static String CHARS_LIST_NCO = CHARS_LIST_N + "O";
    private static String CHARS_LIST_UPPER_ALPHA_NUMERIC = "\u0000ABCDEFGHIJKLMNOPQRSTUVWXYZ .,:;-_+=!@#$%^&*()[]{}'\"\\|<>?/~`\u2026";
    private static String CHARS_LIST_AaN = "\u0000ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;-_+=!@#$%^&*()[]{}'\"\\|<>?/~`\u2026";

    private static final FontConfig MediumText_A_N_TSCALED = new FontConfig(
            "MediumText_A_N_TSCALED", CHARS_LIST_N,
            new FontSourceGenerated("font/Roboto_Regular.ttf", 28, Color.WHITE, "_appSize_width", 360f)
    );
    private static final FontConfig PlayerCurrency_AaN_TSCALED = new FontConfig(
            "PlayerCurrency_AaN_TSCALED", CHARS_LIST_AaN,
            new FontSourceGenerated("font/Roboto_Regular.ttf", 16, Color.WHITE, "_appSize_width", 360f)
    );
    private static final FontConfig MediumText_B_AaN_TSCALED = new FontConfig(
            "MediumText_B_AaN_TSCALED", CHARS_LIST_AaN,
            new FontSourceGenerated("font/Roboto_Medium.ttf", 16, Color.WHITE, "_appSize_width", 360f)
    );
    private static final FontConfig InGame_Score_NCO_TSCALE = new FontConfig(
            "InGame_Score_NCO_TSCALE", CHARS_LIST_NCO,
            new FontSourceGenerated("font/Quicksand_Book.ttf", 90, Color.WHITE, "_appSize_width", 360f)
    );

    // In case of login error a rectangular popup will be shown on the bottom of the screen.
    private static final FontConfig BottomPopupText_AaN_DPM = new FontConfig(
            "Roboto_Regular_White_AaN_14_DPM", CHARS_LIST_AaN,
            new FontSourceGenerated("font/Roboto_Regular.ttf", 14, Color.WHITE, "_dpm_width", 1f)
    );
    private static final FontConfig BottomPopupTextFont = BottomPopupText_AaN_DPM;
    private static final FontConfig BottomPopupActionFont = BottomPopupText_AaN_DPM;

    /*
    private static final FontConfig Roboto_Regular_White_AaN_16_DPM = new FontConfig(
            "Roboto_Regular_White_AaN_16_DPM", CHARS_LIST_AaN,
            new FontSourceGenerated("font/Roboto_Regular.ttf", 16, Color.WHITE, "_dpm_width", 1f)
    );*/
    private static final FontConfig Dialog_Body = null;//Roboto_Regular_White_AaN_16_DPM;

    /*
    private static final FontConfig Roboto_Medium_White_AaN_14_DPM = new FontConfig(
            "Roboto_Medium_White_AaN_14_DPM", CHARS_LIST_AaN,
            new FontSourceGenerated("font/Roboto_Medium.ttf", 14, Color.WHITE, "_dpm_width", 1f)
    );*/
    private static final FontConfig Dialog_LblButton = null;//Roboto_Medium_White_AaN_14_DPM;

    /*
    private static final FontConfig Roboto_Medium_White_AaN_20_DPM = new FontConfig(
            "Roboto_Medium_White_AaN_20_DPM", CHARS_LIST_AaN,
            new FontSourceGenerated("font/Roboto_Medium.ttf", 20, Color.WHITE, "_dpm_width", 1f)
    );*/
    private static final FontConfig Dialog_Title = null;//Roboto_Medium_White_AaN_20_DPM;

    private String usageFontID;

    private String characters;

    private Object fontSource;

    private FontConfig(String usageFontID, String characters, Object fontSource) {
		super();

		this.usageFontID = usageFontID;

        this.characters = characters;

		this.fontSource = fontSource;

        entriesMap.put(usageFontID, this);
	}

    private static String ComputeBakedFontId(FontConfig fontConfig, int bakedFontSize) {
        String s = fontConfig.usageFontID + "_" + Integer.toString(bakedFontSize);

        return s;
    }

}
