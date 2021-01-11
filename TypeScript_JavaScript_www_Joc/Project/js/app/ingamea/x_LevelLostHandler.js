/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/EntityType",
	"app/ingamea/entity/worldentity/EntityModulePhysics",
	"app/ingamea/entity/worldentity/EntityModulePhysicsDebug",
	"app/ingamea/util/WorldObjectType",
	"app/ingamea/util/LevelLostReason",
	
	// Global libs.
	"lib/p2js/p2"
], function(declare, ModuleBase,
	EntityType, EntityModulePhysics, EntityModulePhysicsDebug, WorldObjectType, LevelLostReason
){
	"use strict";
	return declare("app.ingamea.LevelLostHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gLevelLost", cbName:"onGLevelLost", isUnique:false},
			
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false}
		],
		publishMsg: {
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gIncrementUnpauseState:{channel:"gIncrementUnpauseState", listenerType:"any", mustExist:false}
		},
		
		game: null,
		
		constructor: function() {
			//void
		},
		
		onGInitBaseComponents: function() {
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.game.isLevelLost = false;
		},
		
		onGLevelLost: function(lostReasonType) {
			if(!this.game.isLevelLost) {
				this.game.isLevelLost = true;
				
				//asd_002;
				console.log("asd; level lost: " + lostReasonType);
				
				this.publisher.publish(this.publishMsg.gIncrementUnpauseState, false);
			}
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
