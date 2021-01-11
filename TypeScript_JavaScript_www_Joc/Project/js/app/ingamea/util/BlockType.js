/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	var cls = declare("app.ingamea.util.BlockType", null, {});
	
	cls.SIMPLE = "SIMPLE";
	
	cls.STAR = "STAR";
	cls.CLOCK = "CLOCK";
	cls.DIAMOND = "DIAMOND";
	cls.BOMB = "BOMB";
	cls.LINE_DESTROYED = "LINE_DESTROYED";
	cls.COLUMN_DESTROYED = "COLUMN_DESTROYED";
	
	cls.CEMENT = "CEMENT";
	
	cls.KEY = "KEY";
	cls.LOCK = "LOCK";
	
	return cls;
	
});
