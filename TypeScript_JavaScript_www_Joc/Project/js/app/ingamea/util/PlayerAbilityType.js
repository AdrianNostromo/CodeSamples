/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	var cls = declare("app.ingamea.util.PlayerAbilityType", null, {});
	
	cls.EXTRA_TIME = "EXTRA_TIME";
	cls.RANDOMIZE_COLORS = "RANDOMIZE_COLORS";
	cls.DESTROY_BLOCK = "DESTROY_BLOCK";
	cls.PAINT_BLOCK = "PAINT_BLOCK";
	
	return cls;
	
});
