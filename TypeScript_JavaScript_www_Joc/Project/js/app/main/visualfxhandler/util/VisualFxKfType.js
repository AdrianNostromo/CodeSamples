/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	var cls = declare("app.main.visualfxhandler.util.VisualFxKfType", null, {});
	
	cls.DELAY = "DELAY";
	cls.ANIMATION = "ANIMATION";
	cls.FUNCTION_CALL = "FUNCTION_CALL";
	cls.VALUE_DETECTOR = "VALUE_DETECTOR";
	
	return cls;
	
});
