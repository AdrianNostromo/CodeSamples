/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/util/SafeNumber",
	
	"app/ingamea/GameLayers",
	"app/ingamea/GameLoop",
	"app/ingamea/BlocksGridHandler",
	"app/ingamea/WorldGenerator",
	
	"app/ingamea/GridPanner",
	"app/ingamea/LevelEndHandler",
	"app/ingamea/PlayTimer",
	
	"app/ingamea/InGameData",
	"app/ingamea/UserInterfaceLevelEndDelayTimer",
	"app/ingamea/UserInterfaceBottomBar",
	"app/ingamea/UserInterfaceTimerBar",
	
	"app/ingamea/UserInterfaceActiveAbilityBar",
	"app/ingamea/PlayerAbilityExtraTime",
	"app/ingamea/ClasicBlocksInterraction",
	"app/ingamea/PlayerAbilityRandomizeBlockColors",
	"app/ingamea/PlayerAbilityDestroyBlock",
	
	"app/ingamea/PlayerAbilityPaintBlock"
], function(declare, ModuleBase, 
	SafeNumber, 
	GameLayers, GameLoop, BlocksGridHandler, WorldGenerator, 
	GridPanner, LevelEndHandler, PlayTimer,
	InGameData, UserInterfaceLevelEndDelayTimer, UserInterfaceBottomBar, UserInterfaceTimerBar, 
	UserInterfaceActiveAbilityBar, PlayerAbilityExtraTime, ClasicBlocksInterraction, PlayerAbilityRandomizeBlockColors, PlayerAbilityDestroyBlock,
	PlayerAbilityPaintBlock
){
	"use strict";
	return declare("app.InGameA", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites: [
			GameLayers,
			GameLoop,
			BlocksGridHandler,
			WorldGenerator, 
			
			GridPanner,
			LevelEndHandler,
			PlayTimer,
			
			InGameData,
			UserInterfaceLevelEndDelayTimer,
			UserInterfaceBottomBar,
			UserInterfaceTimerBar,
			
			UserInterfaceActiveAbilityBar,
			PlayerAbilityExtraTime,
			ClasicBlocksInterraction,
			PlayerAbilityRandomizeBlockColors,
			PlayerAbilityDestroyBlock,
			
			PlayerAbilityPaintBlock
		],
		subscribeListeners:[
			{channel:"gGetGame", cbName:"onGGetGame", isUnique:true},
			{channel:"gInitializeLevel", cbName:"onGInitializeLevel", isUnique:true},
			{channel:"gGameLevelPreDestructionBaseObjects", cbName:"onGGameLevelPreDestructionBaseObjects", isUnique:true}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gIncrementUnpauseState:{channel:"gIncrementUnpauseState", listenerType:"any", mustExist:true},
			gInitLayers:{channel:"gInitLayers", listenerType:"list", mustExist:false},
			gInitBaseComponents:{channel:"gInitBaseComponents", listenerType:"list", mustExist:false},
			gInitDependantComponents:{channel:"gInitDependantComponents", listenerType:"list", mustExist:false},
			gInitPersistentContent:{channel:"gInitPersistentContent", listenerType:"list", mustExist:false},
			gInitGeneralContent:{channel:"gInitGeneralContent", listenerType:"list", mustExist:false},
			gCreatePlayerUnit:{channel:"gCreatePlayerUnit", listenerType:"list", mustExist:false},
			gPostWorldPrepare:{channel:"gPostWorldPrepare", listenerType:"list", mustExist:false},
			gInitUserInterface:{channel:"gInitUserInterface", listenerType:"list", mustExist:false},
			gGenerateNewGameBoard:{channel:"gGenerateNewGameBoard", listenerType:"any", mustExist:true}
		},
		
		app: null,
		game: null,
		
		constructor: function() {
			//void
		},
		
		onGGetGame:function() {
			return this.game;
		},
		
		onGInitializeLevel:function(rootContainer, levelConfig, extraData) {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.app.game = this.game = {
				flags: {
					incrementFlags: this.incrementFlags.bind(this),
					decrementFlags: this.decrementFlags.bind(this),
					levelEnded: false,
					iEnableGameInteraction: 0,
					iEnableHudUIInteraction: 0,
					iEnablePlayTimer: 0,
					iBoardSettleBlocked: 0,
					iVisualFXComponentActive: 0,
					iBoardSettleVelocityChangeBlocked: 0
				}
			};
			this.game.flags.iEnableGameInteraction++;
			this.game.flags.iEnableHudUIInteraction++;
			this.game.flags.iEnablePlayTimer++;
			
			this.extraData = extraData;
			
			this.game.levelConfig = levelConfig;
			
			this.game.levelID = extraData.levelID;
			
			this.game.snLevelIndice = extraData.snLevelIndice;
			this.game.waveTemplate = this.game.levelConfig.levelWaves[Math.min(this.game.levelConfig.levelWaves.length - 1, this.game.snLevelIndice.getValue())];
			
			this.game.cfg = {
				grid: {
					cellSize: {
						width: 100,
						height: 100
					},
					sidePadding: 12
				}
			};
			
			this.publisher.publish(this.publishMsg.gInitLayers, rootContainer);
			this.publisher.publish(this.publishMsg.gInitBaseComponents);
			this.publisher.publish(this.publishMsg.gInitDependantComponents);
			this.publisher.publish(this.publishMsg.gInitPersistentContent);
			this.publisher.publish(this.publishMsg.gInitGeneralContent);
			this.publisher.publish(this.publishMsg.gGenerateNewGameBoard);
			this.publisher.publish(this.publishMsg.gCreatePlayerUnit);
			this.publisher.publish(this.publishMsg.gPostWorldPrepare);
			this.publisher.publish(this.publishMsg.gInitUserInterface);
			
			this.publisher.publish(this.publishMsg.gIncrementUnpauseState, true);
		},
		
		onGGameLevelPreDestructionBaseObjects: function() {
			this.app.game = null;
		},
		
		incrementFlags: function(flags) {
			if(!flags) {
				return ;
			}
			
			if(flags instanceof Array) {
				for(var i=0;i<flags.length;i++) {
					var entry = flags[i];
					if(!entry in this.game.flags) {
						this.postError("err 7");
						
						return false;
					}
					
					this.game.flags[entry]++;
				}
			}else {
				if(!flags in this.game.flags) {
					this.postError("err 7");
					
					return false;
				}
				
				this.game.flags[flags]++;
			}
			
			return true;
		},
		
		decrementFlags: function(flags) {
			if(!flags) {
				return ;
			}
			
			if(flags instanceof Array) {
				for(var i=0;i<flags.length;i++) {
					var entry = flags[i];
					if(!entry in this.game.flags) {
						this.postError("err 7");
						
						return false;
					}
					
					this.game.flags[entry]--;
				}
			}else {
				if(!flags in this.game.flags) {
					this.postError("err 7");
					
					return false;
				}
				
				this.game.flags[flags]--;
			}
			
			return true;
		},
		
		destroy: function() {
			if(this.game) {
				this.game = null;
			}
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
