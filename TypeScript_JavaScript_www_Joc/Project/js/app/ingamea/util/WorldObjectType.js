/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	var cls = declare("app.ingamea.util.WorldObjectType", null, {});
	
	cls.ENTITY = "ENTITY";
	cls.WALL = "WALL";
	cls.ITEM = "ITEM";
	
	return cls;
	
});
