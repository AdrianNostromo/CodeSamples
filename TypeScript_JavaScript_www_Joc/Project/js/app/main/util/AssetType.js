/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	var cls = declare("app.main.util.AssetType", null, {});
	
	cls.SPRITE_SHEET = "SPRITE_SHEET";
	cls.AUDIO = "AUDIO";
	cls.JSON = "JSON";
	cls.WEBFONT = "WEBFONT";
	cls.AUDIO_SPRITE = "AUDIO_SPRITE";
	
	return cls;
	
});
