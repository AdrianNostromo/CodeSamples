/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	var cls = declare("app.ingamea.util.LevelEndExtendedType", null, {});
	
	cls.DEFEAT_MANUAL_REQUEST = "DEFEAT_MANUAL_REQUEST";
	cls.DEFEAT_TIME_EXPIRED_AND_NO_STARS_COLLECTED = "DEFEAT_TIME_EXPIRED_AND_NO_STARS_COLLECTED";
	cls.VICTORY_TIME_EXPIRED_AND_STARS_COLLECTED = "VICTORY_TIME_EXPIRED_AND_STARS_COLLECTED";
	cls.VICTORY_ALL_STARS_COLLECTED = "VICTORY_ALL_STARS_COLLECTED";
	
	return cls;
	
});
