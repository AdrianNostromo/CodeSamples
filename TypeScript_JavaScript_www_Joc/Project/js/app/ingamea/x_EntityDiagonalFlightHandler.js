/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/entity/worldentity/EntityModuleDiagonalFlight",
], function(declare, ModuleBase,
	EntityModuleDiagonalFlight
){
	"use strict";
	return declare("app.ingamea.EntityDiagonalFlightHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false},
			{channel:"gInitEntityBase", cbName:"onGInitEntityBase", isUnique:false}
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
			if(!("diagonalFlight" in entity.config.modules)) {
				return ;
			}
			
			var moduleConfig = entity.config.modules.diagonalFlight;
			
			entity.modules.diagonalFlight = new EntityModuleDiagonalFlight(moduleConfig, entity, this.game.levelData);
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
