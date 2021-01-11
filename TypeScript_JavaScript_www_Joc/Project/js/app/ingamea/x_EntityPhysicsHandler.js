/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/EntityType",
	"app/ingamea/entity/worldentity/EntityModulePhysics",
	"app/ingamea/entity/worldentity/EntityModulePhysicsDebug",
	"app/util/PhysicsUtil",
	"app/ingamea/util/WorldObjectType",
	
	// Global libs.
	"lib/p2js/p2"
], function(declare, ModuleBase,
	EntityType, EntityModulePhysics, EntityModulePhysicsDebug, PhysicsUtil, WorldObjectType
){
	"use strict";
	return declare("app.ingamea.EntityPhysicsHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false},
			{channel:"gInitEntityBase", cbName:"onGInitEntityBase", isUnique:false},
			{channel:"gGameLoopSinkVisualsToPhysics", cbName:"onGGameLoopSinkVisualsToPhysics", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game: null,
		
		constructor: function() {
			//void
		},
		
		onGInitBaseComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
		},
		
		onGInitEntityBase: function(entity, entityInfo) {
			if(!("physics" in entity.config.modules)) {
				return ;
			}
			
			var moduleConfig = entity.config.modules.physics;
			
			var entitySize = {
				width: entity.modules.visual.vElems.rootVisual.width,
				height: entity.modules.visual.vElems.rootVisual.height
			};
			
			var debugVElems = null;
			if(this.app.dev.debugPhysicsVisuals) {
				debugVElems = {};
			}
			
			var pos = entity.getPosition();
			
			var pElems = {};
			
			if(!PhysicsUtil.generatePhysicsFromConfig(this.app, this.moduleBranch, moduleConfig, pElems, debugVElems, entitySize, {x: pos.x, y: pos.y}, entity.modules.visual.vElems.rootVisual.rotation)) {
				this.postError("err 6");
    			
    			return false;
			}
			
			pElems.mainBody.userData = {
				worldEntity: entity
			};
			
			this.game.pWorld.addBody(pElems.mainBody);
			
			entity.modules.physics = new EntityModulePhysics(moduleConfig, entity, pElems);
			
			if(debugVElems) {
				this.game.layers.physicsDebug.addChild(debugVElems.rootVisual);
				
				entity.modules.physicsDebug = new EntityModulePhysicsDebug(entity, debugVElems, this.game.layers.physicsDebug);
			}
		},
		
		onGGameLoopSinkVisualsToPhysics: function(gameTime, deltaMS, deltaS) {
			// Update the units visuals;
			for(var i=0;i<this.game.pWorld.bodies.length;i++) {
				var body = this.game.pWorld.bodies[i];
				if(body && body.userData && body.userData.worldEntity && body.userData.worldEntity.worldObjectType === WorldObjectType.ENTITY) {
					body.userData.worldEntity.syncPosition(body.position[0], body.position[1]);
					body.userData.worldEntity.syncRotationR(body.angle);
				}
			}
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
