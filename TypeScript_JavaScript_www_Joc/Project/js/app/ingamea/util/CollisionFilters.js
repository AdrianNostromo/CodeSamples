/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	var cls = declare("app.ingamea.util.CollisionFilters", null, {});
	
	var HERO = Math.pow(2,0),
        WALL =  Math.pow(2,1),
        ITEM = Math.pow(2,2)
    
    cls.HERO = {
    	group: HERO,
    	mask: WALL | ITEM
    };
    cls.WALL = {
    	group: WALL,
    	mask: HERO
    };
    cls.ITEM = {
    	group: ITEM,
    	mask: HERO
    };
	
	return cls;
	
});
