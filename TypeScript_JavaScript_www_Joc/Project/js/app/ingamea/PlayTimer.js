/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/util/VisualUtil",
	"app/main/view/util/ViewType"
], function(declare, ModuleBase,
	VisualUtil, ViewType
){
	"use strict";
	return declare("app.ingamea.PlayTimer", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitDependantComponents", cbName:"onGInitDependantComponents", isUnique:false},
			{channel:"gGameLoopGameLogic", cbName:"onGGameLoopGameLogic", isUnique:false},
			{channel:"gGridInterractionOccured", cbName:"onGGridInterractionOccured", isUnique:false},
			{channel:"gAddExtraPlayTime", cbName:"onGAddExtraPlayTime", isUnique:false}
		],
		publishMsg: {
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gLevelPlayTimeExpired:{channel:"gLevelPlayTimeExpired", listenerType:"any", mustExist:false},
			gPlayTimerChangedMS:{channel:"gPlayTimerChangedMS", listenerType:"any", mustExist:false},
			gPlayTimeUsed:{channel:"gPlayTimeUsed", listenerType:"any", mustExist:false}
		},
		
		game: null,
		
		isGameFrozen: false,
		isTimerPaused: true,
		isEventDispatchedCounter: 0,
		
		constructor: function() {
			//void
		},
		
		onGInitDependantComponents: function() {
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			var playTimeMS = this.game.waveTemplate.playTime;
			
			this.game.data.totalPlayTimeMS = playTimeMS;
			this.game.data.playTimeMS = playTimeMS;
		},
		
		onGGameLoopGameLogic: function(gameTime, deltaMS, deltaS) {
			if(this.game.flags.iEnablePlayTimer < 1) {
				return ;
			}
			
			if(this.isGameFrozen && this.game.data.playTimeMS > 0) {
				this.isGameFrozen = false;
				this.game.flags.iEnableGameInteraction++;
				
				this.isEventDispatchedCounter = 0;
			}
			
			if(this.game.isUnpaused && !this.isTimerPaused && !this.game.cinematicMode && this.game.data.playTimeMS > 0) {
				this.game.data.playTimeMS = Math.max(0, this.game.data.playTimeMS - deltaMS);
				
				this.handleTimeChange();
			}
			
			if(this.isGameFrozen && this.isEventDispatchedCounter < 5) {
				if(this.game.grid.isSettleAnimationV || this.game.grid.isSettleAnimationH || this.game.flags.iVisualFXComponentActive > 0) {
					this.isEventDispatchedCounter = 0;
				}else {
					this.isEventDispatchedCounter++;
				}
				
				if(this.isEventDispatchedCounter >= 5) {
					this.publisher.publish(this.publishMsg.gLevelPlayTimeExpired);
				}
			}
		},
		
		handleTimeChange: function() {
			this.publisher.publish(this.publishMsg.gPlayTimerChangedMS, this.game.data.playTimeMS);
			
			if(this.game.data.playTimeMS <= 0 && this.isGameFrozen == false) {
				this.isGameFrozen = true;
				this.game.flags.iEnableGameInteraction--;
				
				this.publisher.publish(this.publishMsg.gPlayTimeUsed);
			}
		},
		
		onGGridInterractionOccured: function() {
			this.isTimerPaused = false;
		},
		
		onGAddExtraPlayTime: function(extraTimeMS, pauseTimer) {
			this.game.data.playTimeMS += extraTimeMS;
			this.game.data.totalPlayTimeMS += extraTimeMS;
			
			if(pauseTimer) {
				this.isTimerPaused = true;
			}
			
			this.handleTimeChange();
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}

	});
});
