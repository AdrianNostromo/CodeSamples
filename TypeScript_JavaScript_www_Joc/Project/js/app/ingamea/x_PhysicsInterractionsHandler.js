/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/EntityType",
	"app/ingamea/entity/worldentity/EntityModulePhysics",
	"app/ingamea/entity/worldentity/EntityModulePhysicsDebug",
	"app/ingamea/util/DTWorldObjectsInterraction",
	
	// Global libs.
	"lib/p2js/p2"
], function(declare, ModuleBase,
	EntityType, EntityModulePhysics, EntityModulePhysicsDebug, DTWorldObjectsInterraction
){
	"use strict";
	return declare("app.ingamea.PhysicsInterractionsHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitDependantComponents", cbName:"onGInitDependantComponents", isUnique:false},
			{channel:"gGameLoopPhysicsInterractions", cbName:"onGGameLoopPhysicsInterractions", isUnique:false}
		],
		publishMsg: {
			gHandlePhysicsInterractions:{channel:"gHandlePhysicsInterractions", listenerType:"list", mustExist:false},
			
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game: null,
		
		beginContactsList: null,
		endContactsList: null,
		
		constructor: function() {
			//void
		},
		
		onGInitDependantComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.beginContactsList = [];
			this.endContactsList = [];
			
			this.game.pWorld.on("beginContact", this.onContactBegin.bind(this));
	        this.game.pWorld.on("endContact", this.onContactEnd.bind(this));
		},
		
		onContactBegin: function(event) {
			if(("userData" in event.bodyA) && ("userData" in event.bodyB) && event.bodyA.userData.worldEntity && event.bodyB.userData.worldEntity) {
				var interactionObject = new DTWorldObjectsInterraction(
					event.bodyA, event.bodyA.userData.worldEntity,
					event.bodyB, event.bodyB.userData.worldEntity
				);
				
				this.beginContactsList.push(interactionObject);
			}
		},
		
		onContactEnd: function(event) {
			if(("userData" in event.bodyA) && ("userData" in event.bodyB) && event.bodyA.userData.worldEntity && event.bodyB.userData.worldEntity) {
				var interactionObject = new DTWorldObjectsInterraction(
					event.bodyA, event.bodyA.userData.worldEntity,
					event.bodyB, event.bodyB.userData.worldEntity
				);
				
				this.endContactsList.push(interactionObject);
			}
		},
		
		onGGameLoopPhysicsInterractions: function(gameTime, deltaMS, deltaS) {
			var isContacts = false;
			
			if(this.beginContactsList.length > 0 || this.endContactsList.length > 0) {
				isContacts = true;
				
				// Dispatch contacts if any exists;
				this.publisher.publish(this.publishMsg.gHandlePhysicsInterractions, this.beginContactsList, this.endContactsList);
				
				if(this.beginContactsList.length > 0) {
					this.beginContactsList = [];
				}
				
				if(this.endContactsList.length > 0) {
					this.endContactsList = [];
				}
			}
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
