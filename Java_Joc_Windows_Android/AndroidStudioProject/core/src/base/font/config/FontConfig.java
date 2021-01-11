package base.font.config;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import base.font.util.FontSource;

public class FontConfig {

	public static final Vector<FontConfig> entriesList = new Vector<FontConfig>();
	public static final Map<String, FontConfig> entriesMap = new HashMap<String, FontConfig>();
    
    public static final int CHARS_ID_N = 0;
    public static final int CHARS_ID_NCO = 1;
    public static final int CHARS_ID_AN = 2;
    public static final int CHARS_ID_AaN = 3;
    
    public static final String[] CHARS_LIST = new String[4];
    public static final String[] CHARS_LIST_NAME = new String[4];
    static {
        CHARS_LIST[CHARS_ID_N] = "\u00000123456789" + " .-,:;";
        CHARS_LIST_NAME[CHARS_ID_N] = "N";
        
        CHARS_LIST[CHARS_ID_NCO] = CHARS_LIST[CHARS_ID_N] + "O";
        CHARS_LIST_NAME[CHARS_ID_NCO] = "NCO";
        
        CHARS_LIST[CHARS_ID_AN] = "\u0000ABCDEFGHIJKLMNOPQRSTUVWXYZ .,:;-_+=!@#$%^&*()[]{}'\"\\|<>?/~`\u2026";
        CHARS_LIST_NAME[CHARS_ID_AN] = "AN";
        
        CHARS_LIST[CHARS_ID_AaN] = "\u0000ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;-_+=!@#$%^&*()[]{}'\"\\|<>?/~`\u2026";
        CHARS_LIST_NAME[CHARS_ID_AaN] = "AaN";
    }
    
    public String usageFontID;
    
    public int charactersListId;
    
    public FontSource fontSource;
    
    public FontConfig(String usageFontID, int charactersListId, FontSource fontSource) {
        super();
        
        this.usageFontID = usageFontID;
        
        this.charactersListId = charactersListId;
        
        this.fontSource = fontSource;
    
        entriesList.add(this);
        entriesMap.put(usageFontID, this);
    }
    
    public static String ComputeBakedFontId(FontConfig fontConfig, int bakedFontSize) {
        String s = fontConfig.fontSource.cacheId + "_" + CHARS_LIST_NAME[fontConfig.charactersListId] + "_" + Integer.toString(bakedFontSize);
        
        return s;
    }
    
}
