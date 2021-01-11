/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	var cls = declare("app.main.util.SoundTemplate", null, {});
	
	cls.BG_MUSIC = {soundID: "as_main", spriteID: "mainMenu/turretFire_Simple", playProps: new createjs.PlayPropsConfig().set({loop: -1, volume: 0.5}), isInit: false};
	cls.BTN_TOUCH_DOWN = {soundID: "as_main", spriteID: "mainMenu/turretFire_Simple", playProps: new createjs.PlayPropsConfig().set({loop: -1, volume: 0.5}), isInit: false};
	cls.BTN_TOUCH_RELEASE = {soundID: "as_main", spriteID: "mainMenu/turretFire_Simple", playProps: new createjs.PlayPropsConfig().set({loop: -1, volume: 0.5}), isInit: false};
	
	return cls;
	
});
