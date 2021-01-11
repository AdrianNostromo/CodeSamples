/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare
){
	"use strict";
	return declare("app.ingamea.entity.worldentity.EntityModuleSegmentedBody", null, {
		
		config: null,
		entity: null,
		
		// [].{x: Number, y: Number, angR: Number};
		positionsHistory: null,
		
		segmentsCount: 0,
		vElems: null,
		
		cfg: {
			// Save positions for a extra 5 segments;
			extraSegmentsPositions: 5
		},
		
		constructor: function(config, entity, pElems) {
			this.config = config;
			this.entity = entity;
			
			var pos = this.entity.getPosition();
			var angR = this.entity.getRotationR();
			
			this.positionsHistory = [{
				x: pos.x,
				y: pos.y,
				angR: angR
			}];
			
			this.vElems = {};
			this.entity.events.register("entityUtilStep", this.onEntityUtilStep, this, false);
		},
		
		onEntityUtilStep: function(entity, gameTime, deltaMS, deltaS) {
			var totalSegments = this.segmentsCount + this.cfg.extraSegmentsPositions;
			var maxStepsCount = Math.min(1, totalSegments) * this.config.delayStepsFirst 
								+ Math.max(0, totalSegments - 1) * this.config.delayStepsSecondaries;
			
			var i = Math.min(maxStepsCount - 2, this.positionsHistory.length - 1);
			while(i >= 0) {
				this.positionsHistory[i + 1] = this.positionsHistory[i];
				
				i--;
			}
			
			var pos = this.entity.getPosition();
			var angR = this.entity.getRotationR();
			
			this.positionsHistory[0] = {
				x: pos.x,
				y: pos.y,
				angR: angR
			};
			this.updateSegmentsPosition();
		},
		
		updateSegmentsPosition: function() {
			if(this.positionsHistory.length <= 0) {
				return ;
			}
			
			for(var i=0;i<this.segmentsCount;i++) {
				var posIndice = this.config.delayStepsFirst + i * this.config.delayStepsSecondaries;
				posIndice = Math.min(this.positionsHistory.length - 1, posIndice);
				var posCompound = this.positionsHistory[posIndice];
				
				var visual = this.vElems["bodySegment_" + i];
				
				visual.position.set(posCompound.x, posCompound.y);
				visual.rotation = posCompound.angR;
			}
		},
		
		addNewSegment: function() {
			this.entity.events.dispatch("bodySegmentsCountChangeRequest", this.entity, this.segmentsCount + 1);
		},
		
		destroy: function() {
			//void
		}
		
	});
});
