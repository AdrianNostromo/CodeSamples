/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	return declare("app.ingamea.entity.worldentity.EntityModuleVisual_SpriteSheet", null, {
		
		config: null,
		entity: null,
		
		vElems: null,
		blankTexturesListRefference: null,
		
		textureGroups: null,
		
		parentLayer: null,
		
		constructor: function(config, entity, vElems, blankTexturesListRefference, textureGroups, parentLayer) {
			this.config = config;
			this.entity = entity;
			
			this.vElems = vElems;
			this.blankTexturesListRefference = blankTexturesListRefference;
			
			this.textureGroups = textureGroups;
			
			this.parentLayer = parentLayer;
			
			this.entity.events.register("positionChanged", this.onEntityPosChange, this, false);
			this.entity.events.register("rotationChanged", this.onRotationChanged, this, false);
			this.entity.events.register("isPhasedInChanged", this.onEntityIsPhasedInChange, this, false);
		},
		
		onEntityPosChange: function(entity, posX, posY) {
			if(this.vElems.rootVisual.position.x != posX || this.vElems.rootVisual.position.y != posY) {
				this.vElems.rootVisual.position.set(posX, posY);
			}
		},
		
		onRotationChanged: function(entity, rotR) {
			if(this.vElems.rootVisual.rotation != rotR) {
				this.vElems.rootVisual.rotation = rotR;
			}
		},
		
		onEntityIsPhasedInChange: function(entity, isPhasedIn) {
			if(isPhasedIn) {
				this.parentLayer.addChild(this.vElems.rootVisual);
			}else {
				if(this.vElems.rootVisual.parent) {
					this.vElems.rootVisual.parent.removeChild(this.vElems.rootVisual);
				}
			}
		},
		
		destroy: function() {
			//void
		}

	});
});