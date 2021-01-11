/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	
	"app/ingamea/entity/worldentity/util/EntityActionType"
], function(declare,
	EntityActionType
){
	"use strict";
	return declare("app.ingamea.entity.worldentity.EntityModuleController", null, {
		
		config: null,
		entity: null,
		
		logicProcessor: null,
		
		constructor: function(config, entity, logicProcessor) {
			this.config = config;
			this.entity = entity;
			
			this.logicProcessor = logicProcessor;
			
			this.entity.events.register("controllerLoop", this.onControllerLoop, this, false);
		},
		
		onControllerLoop: function(entity, gameTime, deltaMS, deltaS) {
			var actionCompound = this.logicProcessor.computeAction(this, gameTime, deltaMS, deltaS);
			
			this.entity.events.dispatch("setEntityAction", this.entity, actionCompound, gameTime, deltaMS, deltaS);
		},
		
		destroy: function() {
			//void
		}

	});
});
