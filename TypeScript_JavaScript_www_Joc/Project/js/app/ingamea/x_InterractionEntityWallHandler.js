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
	return declare("app.ingamea.InterractionEntityWallHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false},
			{channel:"gHandlePhysicsInterractions", cbName:"onGHandlePhysicsInterractions", isUnique:false}
		],
		publishMsg: {
			gLevelLost:{channel:"gLevelLost", listenerType:"list", mustExist:false},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		game: null,
		
		constructor: function() {
			//void
		},
		
		onGInitBaseComponents:function() {
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
		},
		
		onGHandlePhysicsInterractions: function(beginContactsList, endContactsList) {
			for(var i=0;i<beginContactsList.length;i++) {
				var entry = beginContactsList[i];
				
				var entity = null, wall = null;
				
				if(entry.objectA.worldObjectType === WorldObjectType.ENTITY && entry.objectB.worldObjectType === WorldObjectType.WALL) {
					entity = entry.objectA;
					wall = entry.objectB;
				}else if(entry.objectA.worldObjectType === WorldObjectType.WALL && entry.objectB.worldObjectType === WorldObjectType.ENTITY) {
					entity = entry.objectB;
					wall = entry.objectA;
				}
				
				if(entity && wall) {
					// Loss condition occured.
					
					this.publisher.publish(this.publishMsg.gLevelLost, LevelLostReason.IMPACT_WALL);
				}
			}
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
