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
	"app/ingamea/util/WorldItemType",
	
	// Global libs.
	"lib/p2js/p2"
], function(declare, ModuleBase,
	EntityType, EntityModulePhysics, EntityModulePhysicsDebug, WorldObjectType, WorldItemType
){
	"use strict";
	return declare("app.ingamea.InterractionEntityItemHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false},
			{channel:"gHandlePhysicsInterractions", cbName:"onGHandlePhysicsInterractions", isUnique:false}
		],
		publishMsg: {
			gPlayerCollectDiamonds:{channel:"gPlayerCollectDiamonds", listenerType:"unique", mustExist:false},
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
				
				var entity = null, item = null;
				
				if(entry.objectA.worldObjectType === WorldObjectType.ENTITY && entry.objectB.worldObjectType === WorldObjectType.ITEM) {
					entity = entry.objectA;
					item = entry.objectB;
				}else if(entry.objectA.worldObjectType === WorldObjectType.ITEM && entry.objectB.worldObjectType === WorldObjectType.ENTITY) {
					entity = entry.objectB;
					item = entry.objectA;
				}
				
				if(entity && item && entity === this.game.playerEntity && item.isActive) {
					// Loss condition occured.
					
					this.collectItem(entity, item);
				}
			}
		},
		
		collectItem: function(entity, item) {
			if(WorldItemType.DIAMOND === item.type) {
				// Give the player a diamond.
				
				item.itemCollected();
				
				this.publisher.publish(this.publishMsg.gPlayerCollectDiamonds, 1);
			}else if(WorldItemType.BODY_SEGMENT === item.type) {
				// Create a extra segment on the entity.
				
				item.itemCollected();
				
				entity.modules.segmentedBody.addNewSegment();
			}else {
				this.postError("err 6");
    			
    			return false;
			}
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
