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
	return declare("app.horizontalhackandslash.objects.worldentity.EntityModuleGroundUnit", null, {
		
		config: null,
		entity: null,
		
		constructor: function(config, entity) {
			this.config = config;
			this.entity = entity;
			
			this.entity.events.register("processControllerAction", this.onProcessControllerAction, this, false);
		},
		
		onProcessControllerAction: function(entity, action) {
			// Handle action result.
			if(action.type == EntityActionType.RUN) {
				if(action.directionX > 0) {
					// Run right;
					
					//asd_002;
				}else {
					// Run left;
					
					//asd_002;
				}
			}
		},
		
		destroy: function() {
			//void
		}
		
	});
});
