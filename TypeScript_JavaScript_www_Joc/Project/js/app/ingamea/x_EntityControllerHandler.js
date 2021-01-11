/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/EntityType",
	"app/ingamea/entity/worldentity/EntityModuleController",
], function(declare, ModuleBase,
	EntityType, EntityModuleController
){
	"use strict";
	return declare("app.ingamea.EntityControllerHandler", ModuleBase, {
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
			if(!("controller" in entity.config.modules)) {
				return ;
			}
			
			var moduleConfig = entity.config.modules.controller;
			
			var logicProcessor;
			if(entityInfo.logicProcessorOverride) {
				logicProcessor = entityInfo.logicProcessorOverride;
			}else {
				this.postError("err 6");
    			
    			return false;
			}
			
			entity.modules.controller = new EntityModuleController(moduleConfig, entity, logicProcessor);
		},
		
		onGGameLoopEntityController: function(gameTime, deltaMS, deltaS) {
			for(var i=0;i<this.game.entities[EntityType.UNIT].length;i++) {
				var entity = this.game.entities[EntityType.UNIT][i];
				if(entity.modules.controller) {
					entity.events.dispatch("controllerLoop", entity, gameTime, deltaMS, deltaS);
				}
			}
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
