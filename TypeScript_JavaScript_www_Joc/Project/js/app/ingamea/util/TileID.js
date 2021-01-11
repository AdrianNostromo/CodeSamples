/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	var cls = declare("app.ingamea.util.TileID", null, {});
	
	cls.WALL = "WALL";
	cls.DIRT = "DIRT";
	cls.ROCK = "ROCK";
	cls.COAL = "COAL";
	cls.COPPER = "COPPER";
	cls.IRON = "IRON";
	cls.GOLD = "GOLD";
	cls.DIAMOND = "DIAMOND";
	
	return cls;
	
});
