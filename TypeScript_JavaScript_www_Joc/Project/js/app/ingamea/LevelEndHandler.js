/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/LevelEndType",
	"app/ingamea/util/LevelEndExtendedType",
	"app/main/view/util/ViewType"
], function(declare, ModuleBase,
	LevelEndType, LevelEndExtendedType, ViewType
){
	"use strict";
	return declare("app.ingamea.LevelEndHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false},
			{channel:"gUserInterfaceLevelEndRequest", cbName:"onGUserInterfaceLevelEndRequest", isUnique:false},
			{channel:"gLevelEndDelayTimerCloseRequest", cbName:"onGLevelEndDelayTimerCloseRequest", isUnique:false},
			{channel:"gLevelPlayTimeExpired", cbName:"onGLevelPlayTimeExpired", isUnique:false},
			{channel:"gPlayTimerChangedMS", cbName:"onGPlayTimerChangedMS", isUnique:false},
			{channel:"gPersistentGameLoopUserInterface", cbName:"onGPersistentGameLoopUserInterface", isUnique:false},
			{channel:"gCollectedStarsCountChanged", cbName:"onGCollectedStarsCountChanged", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gIncrementUnpauseState:{channel:"gIncrementUnpauseState", listenerType:"any", mustExist:true},
			gHideUserInterface:{channel:"gHideUserInterface", listenerType:"any", mustExist:false},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false},
			gShowLevelEndDelayPanel:{channel:"gShowLevelEndDelayPanel", listenerType:"any", mustExist:false},
			gHideLevelEndDelayPanel:{channel:"gHideLevelEndDelayPanel", listenerType:"any", mustExist:false},
			gLevelEndOccured:{channel:"gLevelEndOccured", listenerType:"any", mustExist:false},
			gAnimateOutGridBlocks:{channel:"gAnimateOutGridBlocks", listenerType:"any", mustExist:false}
		},
		
		app: null,
		game: null,
		
		isPreBlocksOutVisualFXFinished: false,
		// This variable is used to detect when the boar has settled and all visual fx finish and all chain events finish.
		gameIddleCounter: 0,
		
		isEndDelayPanelActive: false,
		
		phase: null,
		
		constructor: function() {
			//void
		},

		onGInitBaseComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
		},
		
		onGUserInterfaceLevelEndRequest: function() {
			this.handleHardGameEndOccured();
		},
		
		onGLevelPlayTimeExpired: function() {
			// If stars are still on board, show a level end panel in case the player want to buy extra time.
			if(this.game.data.snStarsCollected.getValue() < this.game.waveTemplate.starColumns.length) {
				// Don't pause the game, just disable blocks touch detection.
				
				this.isEndDelayPanelActive = true;
				
				this.game.flags.iEnableGameInteraction--;
				
				this.publisher.publish(this.publishMsg.gShowLevelEndDelayPanel);
			}else {
				this.handleHardGameEndOccured();
			}
		},
		
		onGPlayTimerChangedMS: function(playTimeMS) {
			if(this.isEndDelayPanelActive && playTimeMS > 0) {
				this.isEndDelayPanelActive = false;
				
				this.game.flags.iEnableGameInteraction++;
				
				this.publisher.publish(this.publishMsg.gHideLevelEndDelayPanel);
			}
		},
		
		onGLevelEndDelayTimerCloseRequest: function() {
			this.isEndDelayPanelActive = false;
			
			this.game.flags.iEnableGameInteraction++;
			
			this.publisher.publish(this.publishMsg.gHideLevelEndDelayPanel);
			
			this.handleHardGameEndOccured();
		},
		
		onGPersistentGameLoopUserInterface: function(gameTime, deltaMS, deltaS) {
			if(this.game.levelEndReason && this.gameIddleCounter < 5) {
				if(this.game.flags.iVisualFXComponentActive > 0 || this.game.grid.isSettleAnimationV || this.game.grid.isSettleAnimationH) {
					this.gameIddleCounter = 0;
				}else {
					this.gameIddleCounter++;
				}
				
				if(this.gameIddleCounter >= 5) {
					if(!this.isPreBlocksOutVisualFXFinished) {
						this.isPreBlocksOutVisualFXFinished = true;
						
						this.gameIddleCounter = 0;
						
						this.publisher.publish(this.publishMsg.gAnimateOutGridBlocks, this.game.levelEndReason.endDispatcherGridPos);
					}else {
						this.boardStabilizedProcessLevelEnd();
					}
				}
			}
		},
		
		boardStabilizedProcessLevelEnd: function() {
			// Dispatch a msg to hide the ingame ui to sOut.
			// NOTE : this must be dispatched before the game pause message.
			this.publisher.publish(this.publishMsg.gHideUserInterface);
			
			this.publisher.publish(this.publishMsg.gIncrementUnpauseState, false);
			
			if(LevelEndType.DEFEAT == this.game.levelEndReason.endType) {
				var extraData = {
					levelID: this.game.levelID,
					snLevelIndice: this.game.snLevelIndice.clone()
				};
				
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.LEVEL_DEFEAT, "sIn", true, extraData, 0, {callback: this.onDestroyViewInGame, context: this}, null);
			}else if(LevelEndType.VICTORY == this.game.levelEndReason.endType) {
				if(this.game.snLevelIndice.getValue() == 1) {
					this.app.profileData.incrementGamesPlayed();
				}
				
				if(this.game.snLevelIndice.getValue() > this.app.profileData.getMaxLevel()) {
					this.app.profileData.setMaxLevel(this.game.snLevelIndice.getValue());
				}
				
				if(this.game.data.snStarsCollected.getValue() < 1) {
					this.postError("err 6");
					
					return ;
				}
				
				var extraData = {
					levelID: this.game.levelID,
					snLevelIndice: this.game.snLevelIndice.clone(),
					snLevelStarsCollected: this.game.data.snStarsCollected.clone()
				};
				
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.LEVEL_CLEAR, "sIn", true, extraData, 0, {callback: this.onDestroyViewInGame, context: this}, null);
			}else {
				this.postError("err 6");
				
				return ;
			}
		},
		
		onDestroyViewInGame: function() {
			this.publisher.publish(this.publishMsg.changeViewState, ViewType.IN_GAME, "sOut", false, null, 0, null, null);
		},
		
		onGCollectedStarsCountChanged: function(collectedStarGridPos) {
			if(this.game.data.snStarsCollected.getValue() >= this.game.waveTemplate.starColumns.length) {
				// All stars collected. Stop game input and end the level now.
				
				this.handleLevelEnd(LevelEndType.VICTORY, LevelEndExtendedType.VICTORY_ALL_STARS_COLLECTED, collectedStarGridPos);
			}
		},
		
		handleHardGameEndOccured: function() {
			if(this.game.data.snStarsCollected.getValue() < 1) {
				// Defeat.
				
				this.handleLevelEnd(LevelEndType.DEFEAT, LevelEndExtendedType.DEFEAT_TIME_EXPIRED_AND_NO_STARS_COLLECTED, null);
			}else {
				// Level clear.
				
				this.handleLevelEnd(LevelEndType.VICTORY, LevelEndExtendedType.VICTORY_TIME_EXPIRED_AND_STARS_COLLECTED, null);
			}
		},
		
		handleLevelEnd: function(levelEndType, extendedLevelEndType, endDispatcherGridPos) {
			// Ignore if level already ended.
			if(this.game.levelEndReason) {
				return ;
			}
			
			this.game.flags.levelEnded = true;
			this.game.levelEndReason = {
				endType: levelEndType,
				endExtendedType: extendedLevelEndType,
				endDispatcherGridPos: endDispatcherGridPos
			};
			
			// Make the game inactive and dispatch a level end message;
			this.game.flags.iEnableGameInteraction--;
			
			this.publisher.publish(this.publishMsg.gLevelEndOccured);
			
			this.gameIddleCounter = 0;
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}

	});
});
