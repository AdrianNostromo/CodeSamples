/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	var cls = declare("app.ingamea.util.PlayPhaseType", null, {});
	
	cls.PRE_WAVE = "PRE_WAVE";
	cls.WAVE_ACTIVE = "WAVE_ACTIVE";
	
	return cls;
	
});
