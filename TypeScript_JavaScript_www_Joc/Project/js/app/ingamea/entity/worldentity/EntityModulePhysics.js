/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare
){
	"use strict";
	return declare("app.ingamea.entity.worldentity.EntityModulePhysics", null, {
		
		config: null,
		entity: null,
		
		pElems: null,
		
		constructor: function(config, entity, pElems) {
			this.config = config;
			this.entity = entity;
			
			this.pElems = pElems;
			
			this.entity.events.register("positionChangedManually", this.onEntityPosChangeManually, this, false);
			this.entity.events.register("rotationChangedManually", this.onRotationChangedManually, this, false);
		},
		
		onEntityPosChangeManually: function(entity, posX, posY) {
			this.pElems.mainBody.position[0] = posX;
			this.pElems.mainBody.position[1] = posY;
		},
		
		onRotationChangedManually: function(entity, rotR) {
			this.pElems.mainBody.angle = rotR;
		},
		
		destroy: function() {
			//void
		}
		
	});
});
