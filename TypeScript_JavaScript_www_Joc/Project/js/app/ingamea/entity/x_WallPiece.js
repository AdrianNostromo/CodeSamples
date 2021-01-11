/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	
	"app/ingamea/util/WorldObjectType"
], function(declare,
	WorldObjectType
){
	"use strict";
	return declare("app.ingamea.entity.WallPiece", null, {
		
		worldObjectType: null,
		
		config: null,
		
		vElems: null,
		
		pElems: null,
		
		debugVElems: null,
		
		constructor: function(config, vElems, pElems, debugVElems) {
			this.worldObjectType = WorldObjectType.WALL;
			
			this.config = config;
			
			this.vElems = vElems;
			
			this.pElems = pElems;
			
			this.debugVElems = debugVElems;
		},
		
		destroy: function() {
			//void
		}

	});
});
