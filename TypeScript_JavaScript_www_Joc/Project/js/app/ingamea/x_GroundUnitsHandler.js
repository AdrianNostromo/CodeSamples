/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/EntityType",
	"app/horizontalhackandslash/objects/worldentity/EntityModuleGroundUnit",
], function(declare, ModuleBase,
	EntityType, EntityModuleGroundUnit
){
	"use strict";
	return declare("app.horizontalhackandslash.GroundUnitsHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false},
			{channel:"gInitEntityBase", cbName:"onGInitEntityBase", isUnique:false},
			{channel:"gGameLoopEntityController", cbName:"onGGameLoopEntityController", isUnique:false}
		],
		publishMsg: {
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		game: null,
		
		constructor: function() {
			//void
		},
		
		onGInitBaseComponents: function() {
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
		},
		
		onGInitEntityBase: function(entity, entityInfo) {
			if(!("groundUnit" in entity.config.modules)) {
				return ;
			}
			
			var moduleConfig = entity.config.modules.groundUnit;
			
			entity.modules.physics = new EntityModuleGroundUnit(moduleConfig, entity);
		},
		
		onGGameLoopEntityController: function(gameTime, deltaMS, deltaS) {
			var list = this.game.entities[EntityType.UNIT];
			for(var i=0;i<list.length;i++) {
				var entity = list[i];
				if(entity.isActive && entity.isPhasedIn && entity.modules.controller) {
					entity.modules.controller.controllerLoop(gameTime, deltaMS, deltaS);
				}
			}
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
