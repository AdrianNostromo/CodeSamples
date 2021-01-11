/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase"
], function(declare, ModuleBase
    ){
	"use strict";
	return declare("app.ingamea.GameLoop", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false},
			{channel:"gGetUnpausedCounter", cbName:"onGGetUnpausedCounter", isUnique:true},
			{channel:"gIncrementUnpauseState", cbName:"onGIncrementUnpauseState", isUnique:true},
			{channel:"appLoopInGame", cbName:"onAppLoopInGame", isUnique:true}
		],
		publishMsg: {
			gUnpausedStatusChanged:{channel:"gUnpausedStatusChanged", listenerType:"list", mustExist:false},
			
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gGameLoopGameLogicPre:{channel:"gGameLoopGameLogicPre", listenerType:"list", mustExist:false},
			gGameLoopGameLogic:{channel:"gGameLoopGameLogic", listenerType:"list", mustExist:false},
			gGameLoopPhysics:{channel:"gGameLoopPhysics", listenerType:"list", mustExist:false},
			gGameLoopSinkVisualsToPhysics:{channel:"gGameLoopSinkVisualsToPhysics", listenerType:"list", mustExist:false},
			gGameLoopPhysicsInterractions:{channel:"gGameLoopPhysicsInterractions", listenerType:"list", mustExist:false},
			gGameLoopGameElements:{channel:"gGameLoopGameElements", listenerType:"list", mustExist:false},
			gGameLoopEntityUtilPost:{channel:"gGameLoopEntityUtilPost", listenerType:"list", mustExist:false},
			gPersistentGameLoopUserInterface:{channel:"gPersistentGameLoopUserInterface", listenerType:"list", mustExist:false}
		},
		
		game: null,
		
		lastRawTime:-1,
		lastGameTime:0,
		
		constructor: function() {
			//void
		},
		
		onGInitBaseComponents: function() {
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			// [-infinity, 1];
			this.game.isUnpausedCounter = 0;
			this.game.isUnpaused = false;
		},
		
		onGGetUnpausedCounter:function() {
			return this.game.isUnpausedCounter;
		},
		
		onGIncrementUnpauseState:function(isUnpaused) {
			if(isUnpaused) {
				this.game.isUnpausedCounter++;
				
				if(this.game.isUnpausedCounter > 1) {
					this.postError("err 6");
					
					return ;
				}
				
				if(this.game.isUnpausedCounter == 1) {
					this.game.isUnpaused = true;
					
					this.publisher.publish(this.publishMsg.gUnpausedStatusChanged, this.game.isUnpausedCounter);
				}
			}else {
				this.game.isUnpausedCounter--;
				
				if(this.game.isUnpausedCounter == 0) {
					this.game.isUnpaused = false;
					
					this.publisher.publish(this.publishMsg.gUnpausedStatusChanged, this.game.isUnpausedCounter);
				}
			}
		},
		
		onAppLoopInGame:function(appTime, deltaMS, deltaS) {
			if(this.lastRawTime >= 0) {
				var gameDeltaMS = deltaMS;
				var gameDeltaS = gameDeltaMS / 1000;
				
				if(this.game.isUnpausedCounter > 0) {
					this.lastGameTime += gameDeltaMS;
					
					this.publisher.publish(this.publishMsg.gGameLoopGameLogicPre, this.lastGameTime, gameDeltaMS, gameDeltaS);
					this.publisher.publish(this.publishMsg.gGameLoopGameLogic, this.lastGameTime, gameDeltaMS, gameDeltaS);
					this.publisher.publish(this.publishMsg.gGameLoopPhysics, this.lastGameTime, gameDeltaMS, gameDeltaS);
					this.publisher.publish(this.publishMsg.gGameLoopSinkVisualsToPhysics, this.lastGameTime, gameDeltaMS, gameDeltaS);
					this.publisher.publish(this.publishMsg.gGameLoopPhysicsInterractions, this.lastGameTime, gameDeltaMS, gameDeltaS);
					this.publisher.publish(this.publishMsg.gGameLoopGameElements, this.lastGameTime, gameDeltaMS, gameDeltaS);
					this.publisher.publish(this.publishMsg.gGameLoopEntityUtilPost, this.lastGameTime, gameDeltaMS, gameDeltaS);
				}
				
				this.publisher.publish(this.publishMsg.gPersistentGameLoopUserInterface, this.lastGameTime, gameDeltaMS, gameDeltaS);
			}
			
			this.lastRawTime = appTime;
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
