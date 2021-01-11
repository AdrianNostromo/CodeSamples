/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	// Global libs.
	"lib/p2js/p2"
], function(declare, ModuleBase
){
	"use strict";
	return declare("app.ingamea.PhysicsHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false},
			{channel:"gGameLoopPhysics", cbName:"onGGameLoopPhysics", isUnique:false}
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
			
			this.game.pWorld = new p2.World({
				gravity: [0, 0]
			});
		},
		
		onGGameLoopPhysics: function(gameTime, deltaMS, deltaS) {
			this.game.pWorld.step(deltaS);
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
