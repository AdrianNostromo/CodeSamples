/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	var cls = declare("app.main.view.util.ViewType", null, {});
	
	cls.PRELOADER = "PRELOADER";
	cls.BACKGROUND = "BACKGROUND";
	cls.SIDE_BUSHES = "SIDE_BUSHES";
	cls.MAIN = "MAIN";
	cls.IN_GAME = "IN_GAME";
	cls.SHOP = "SHOP";
	cls.LEVEL_DEFEAT = "LEVEL_DEFEAT";
	cls.LEVEL_CLEAR = "LEVEL_CLEAR";
	cls.IN_GAME_PAUSED = "IN_GAME_PAUSED";
	cls.EDGE_POPUP = "EDGE_POPUP";
	cls.DEV_CONSOLE = "DEV_CONSOLE";
	
	return cls;
	
});
