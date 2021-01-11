/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/BlockType",
	"app/ingamea/util/SimpleBlockVersion",
	"app/ingamea/util/EntityVisualType",
	"app/util/VisualUtil",
	"app/util/MathUtil",
	
	"app/ingamea/util/PlayerAbilityType",
	"app/ingamea/blocksgridhandler/util/DTGridInteractor",
	"app/ingamea/util/BlockEventType",
	"lib/pixijs/pixi",
	"app/main/view/util/ViewType",
	
	"app/main/util/PopupMessageType"
], function(declare, ModuleBase,
	BlockType, SimpleBlockVersion, EntityVisualType, VisualUtil, MathUtil, 
	PlayerAbilityType, DTGridInteractor, BlockEventType, PIXI, ViewType,
	PopupMessageType
){
	"use strict";
	return declare("app.ingamea.PlayerAbilityDestroyBlock", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gSetActivePlayerAbility", cbName:"onGSetActivePlayerAbility", isUnique:false},
			
			{channel:"gInitDependantComponents", cbName:"onGInitDependantComponents", isUnique:false},
			{channel:"gUnpausedStatusChanged", cbName:"onGUnpausedStatusChanged", isUnique:false},
			{channel:"gPlayTimeUsed", cbName:"onGPlayTimeUsed", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gShowPlayerAbilityBar:{channel:"gShowPlayerAbilityBar", listenerType:"any", mustExist:false},
			gHidePlayerAbilityBar:{channel:"gHidePlayerAbilityBar", listenerType:"any", mustExist:false},
			gDestroyGridBlocksFromList:{channel:"gDestroyGridBlocksFromList", listenerType:"any", mustExist:false},
			gGridInterractionOccured:{channel:"gGridInterractionOccured", listenerType:"any", mustExist:false},
			gPushGridInteractorGroup:{channel:"gPushGridInteractorGroup", listenerType:"unique", mustExist:true},
			gPopGridInteractorGroup:{channel:"gPopGridInteractorGroup", listenerType:"unique", mustExist:true},
			gIncrementUnpauseState:{channel:"gIncrementUnpauseState", listenerType:"any", mustExist:true},
			activatePopup:{channel:"activatePopup", listenerType:"any", mustExist:false}
		},
		
		app: null,
		game: null,
		
		isAbilityActive: false,
		
		gridInterractor: null,
		
		cfg: {
		},
		
		constructor: function() {
			//void
		},
		
		onGInitDependantComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.gridInterractor = new DTGridInteractor({callback: this.onBlocksInteractionProcesCycle, context: this}, {
				blockTouchStart: true
			});
		},
		
		stopAbility: function() {
			if(this.isAbilityActive) {
				this.isAbilityActive = false;
				
				// Pop the grid interactor if active.
				this.publisher.publish(this.publishMsg.gPopGridInteractorGroup, this.gridInterractor, 1);
				
				// Hide the ability bar if active.
				this.publisher.publish(this.publishMsg.gHidePlayerAbilityBar, PlayerAbilityType.DESTROY_BLOCK, null);
			}
		},
		
		onGSetActivePlayerAbility: function(playerAbilityType) {
			if(playerAbilityType != PlayerAbilityType.DESTROY_BLOCK) {
				if(this.isAbilityActive) {
					this.stopAbility();
				}
				
				return ;
			}
			
			if(this.isAbilityActive) {
				return ;
			}
			
			this.isAbilityActive = true;
			
			// Activate ability bar.
			this.publisher.publish(this.publishMsg.gShowPlayerAbilityBar, {playerAbilityID: playerAbilityType, cbcStopAbility: {callback: this.onAbilityBarCloseRequest, context: this}});
			
			this.publisher.publish(this.publishMsg.gPushGridInteractorGroup, this.gridInterractor, 1);
		},
		
		onBlocksInteractionProcesCycle: function(bufferedBlockEventCompounds, gameTime, deltaMS, deltaS) {
			if(this.isAbilityActive) {
				while(bufferedBlockEventCompounds.blockTouchStart.length > 0) {
					var blockCompound = bufferedBlockEventCompounds.blockTouchStart.pop();
					
					var block = blockCompound.block;
					var event = blockCompound.event;
					
					if(block.isDestroyed) {
						continue;
					}
					
					if(block.type == BlockType.SIMPLE || block.type == BlockType.CEMENT) {
						if(this.app.profileData.getDiamonds() < 1) {
							// Not enough diamonds.
							
							this.publisher.publish(this.publishMsg.activatePopup, PopupMessageType.INSUFFICIENT_DIAMONDS_FOR_ACTION, true);
							
							this.publisher.publish(this.publishMsg.gIncrementUnpauseState, false);
							this.publisher.publish(this.publishMsg.changeViewState, ViewType.SHOP, "sIn", true, {
								showExtraBackground: true,
								cbcViewCloseAnimationFinish: {callback: this.onShopViewClose,  context: this}
							}, 0, null, null);
							
							// Ignore the rest of the touches.
							return ;
						}
						
						// Remove 1 diamond from the player.
						this.app.profileData.addDiamonds(-1);
						
						// Show a fx for "-1 Diamond" lost.
						var fxPos = block.getVPos();
						this.app.visualFx.activateFxTemplate("diamondSpentInGame", 0, fxPos, this.moduleBranch, null, ViewType.IN_GAME);
						
						this.publisher.publish(this.publishMsg.gGridInterractionOccured);
						
						if(block.type == BlockType.SIMPLE) {
							block.activateEvent(BlockEventType.CRUSHED);
						}else if(block.type == BlockType.CEMENT) {
							block.activateEvent(BlockEventType.CRUSHED);
						}else {
							this.postError("err 7");
							
							return null;
						}
						
						block.destroyBlock(true, false);
						
						this.onGSetActivePlayerAbility(null);
					}else {
						// Un activatable.
						
						block.activateEvent(BlockEventType.NO_ACTIVATION);
					}
				}
			}
		},
		
		onShopViewClose: function() {
			this.publisher.publish(this.publishMsg.gIncrementUnpauseState, true);
		},
		
		onGUnpausedStatusChanged: function(isUnpausedCounter) {
			if(this.isAbilityActive) {
				this.stopAbility();
			}
		},
		
		onAbilityBarCloseRequest: function() {
			if(this.isAbilityActive) {
				this.stopAbility();
			}
		},
		
		onGPlayTimeUsed: function() {
			if(this.isAbilityActive) {
				this.stopAbility();
			}
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
