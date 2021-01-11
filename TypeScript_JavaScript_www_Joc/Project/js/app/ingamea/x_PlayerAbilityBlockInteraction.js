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
	"app/ingamea/util/FXConfig",
	"app/ingamea/blocksgridhandler/util/DTGridInteractor",
	"app/main/view/util/ViewType",
	"app/main/util/PopupMessageType",
	
	"app/ingamea/util/BlockEventType"
], function(declare, ModuleBase,
	BlockType, SimpleBlockVersion, EntityVisualType, VisualUtil, MathUtil, 
	PlayerAbilityType, FXConfig, DTGridInteractor, ViewType, PopupMessageType,
	BlockEventType
){
	"use strict";
	return declare("app.ingamea.PlayerAbilityBlockInteraction", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitDependantComponents", cbName:"onGInitDependantComponents", isUnique:false},
			{channel:"gSetActivePlayerAbility", cbName:"onGSetActivePlayerAbility", isUnique:false},
			{channel:"gUnpausedStatusChanged", cbName:"onGUnpausedStatusChanged", isUnique:false},
			{channel:"gGameLoopGameElements", cbName:"onGGameLoopGameElements", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gShowPlayerAbilityBar:{channel:"gShowPlayerAbilityBar", listenerType:"any", mustExist:false},
			gHidePlayerAbilityBar:{channel:"gHidePlayerAbilityBar", listenerType:"any", mustExist:false},
			gDestroyGridBlocksFromList:{channel:"gDestroyGridBlocksFromList", listenerType:"any", mustExist:false},
			gGridInterractionOccured:{channel:"gGridInterractionOccured", listenerType:"any", mustExist:false},
			gActivateFXList:{channel:"gActivateFXList", listenerType:"any", mustExist:false},
			gPushGridInteractorGroup:{channel:"gPushGridInteractorGroup", listenerType:"unique", mustExist:true},
			gPopGridInteractorGroup:{channel:"gPopGridInteractorGroup", listenerType:"unique", mustExist:true},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false},
			gAddExtraPlayTime:{channel:"gAddExtraPlayTime", listenerType:"any", mustExist:false}
		},
		
		app: null,
		game: null,
		
		gridInterractor: null,
		
		activeAbilityType: null,
		activeAbilityConfig: null,
		
		isAnimationActiveRandomizeColor: false,
		animatingBlocksListRandomizeColor: null,
		
		cfg: {
			colorRandomizeBlockSpeedS: 600
		},
		
		bailitiesConfig: {
			EXTRA_TIME: {
				requiresAbilityBar: false
			},
			RANDOMIZE_COLORS: {
				requiresAbilityBar: false
			},
			DESTROY_BLOCK: {
				requiresAbilityBar: true
			},
			PAINT_BLOCK: {
				requiresAbilityBar: true
			}
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
		
		onBlocksInteractionProcesCycle: function(bufferedBlockEventCompounds, gameTime, deltaMS, deltaS) {
			if(this.activeAbilityType === PlayerAbilityType.DESTROY_BLOCK) {
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
							
							this.publisher.publish(this.publishMsg.changeViewState, ViewType.EDGE_POPUP, "sIn", true, {msgID: PopupMessageType.INSUFFICIENT_DIAMONDS_FOR_ACTION}, 0, null, null);
							
							// asd_001;// this doesn't pause the game.
							this.publisher.publish(this.publishMsg.changeViewState, ViewType.SHOP, "sIn", true, {showExtraBackground: true}, 0, null, null);
							
							// Ignore the rest of the touches.
							return ;
						}
						
						// Remove 1 diamond from the player.
						this.app.profileData.addDiamonds(-1);
						
						// Show a fx for "-1 Diamond" lost.
						// asd_001;
						
						this.publisher.publish(this.publishMsg.gGridInterractionOccured);
						
						if(block.type == BlockType.SIMPLE) {
							block.activateEvent(BlockEventType.CRUSHED);
							
							block.destroyBlock(true, false);
						}else if(block.type == BlockType.CEMENT) {
							block.activateEvent(BlockEventType.CRUSHED);
							
							block.destroyBlock(true, false);
						}
						
						this.onGSetActivePlayerAbility(null);
					}else {
						// Un activatable.
						
						block.activateEvent(BlockEventType.NO_ACTIVATION);
					}
				}
			}else if(this.activeAbilityType === PlayerAbilityType.PAINT_BLOCK) {
				asd_001;
			}else {
				this.postError("Err 6.");
				
				return null;
			}
		},
		
		onGUnpausedStatusChanged: function(isUnpausedCounter) {
			if(this.activeAbilityType) {
				this.onGSetActivePlayerAbility(null);
			}
		},
		
		onGSetActivePlayerAbility: function(p_playerAbilityType) {
			if(this.isAnimationActiveRandomizeColor) {
				return ;
			}
			
			var playerAbilityType = p_playerAbilityType;
			if(!this.activeAbilityType && !playerAbilityType) {
				return ;
			}
			
			var oldIsAbilityBar = false;
			var newIsAbilityBar = false;
			var oldIsGridInteractor = false;
			var newIsGridInteractor = false;
			
			// If same ability active, close it.
			if(this.activeAbilityType && this.activeAbilityType === playerAbilityType) {
				return this.onGSetActivePlayerAbility(null);
			}
			
			if(this.activeAbilityType) {
				oldIsGridInteractor = true;
				
				if(this.activeAbilityConfig.requiresAbilityBar) {
					// Hide the ability bar.
					
					oldIsAbilityBar = true;
				}
				
				this.activeAbilityType = null;
				this.activeAbilityConfig = null;
			}
			
			// Save new ability.
			if(playerAbilityType) {
				var newAbilityConfig = this.bailitiesConfig[playerAbilityType];
				if(!newAbilityConfig) {
					this.postError("Err 6.");
					
					return null;
				}
				if(!newAbilityConfig.requiresAbilityBar) {
					// Is instant ability.
					if(playerAbilityType == PlayerAbilityType.EXTRA_TIME) {
						if(this.app.profileData.getDiamonds() < 1) {
							// Not enough diamonds.
							
							this.publisher.publish(this.publishMsg.changeViewState, ViewType.EDGE_POPUP, "sIn", true, {msgID: PopupMessageType.INSUFFICIENT_DIAMONDS_FOR_ACTION}, 0, null, null);
							
							// asd_002;// this doesn't pause the game.
							this.publisher.publish(this.publishMsg.changeViewState, ViewType.SHOP, "sIn", true, {showExtraBackground: true}, 0, null, null);
						}else {
							// Remove 1 diamond from the player.
							this.app.profileData.addDiamonds(-1);
							
							// Show a fx for "-1 Diamond" lost.
							// asd_002;
							
							// Give the player a extra 15 seconds and pause the timer.
							var tm = this.game.data.playTimeMS + 15 * 1000;
							tm = Math.ceil(tm / 1000) * 1000 + 1;
							this.publisher.publish(this.publishMsg.gAddExtraPlayTime, tm - this.game.data.playTimeMS, true);
						}
					}else if(playerAbilityType == PlayerAbilityType.RANDOMIZE_COLORS) {
						this.tryAbilityActivate_RANDOMIZE_COLORS();
					}else {
						this.postError("Err 6.");
						
						return null;
					}
					
					playerAbilityType = null;
				}
			}
			
			if(playerAbilityType) {
				newIsGridInteractor = true;
				
				this.activeAbilityType = playerAbilityType;
				this.activeAbilityConfig = this.bailitiesConfig[this.activeAbilityType];
				if(!this.activeAbilityConfig) {
					this.postError("Err 6.");
					
					return null;
				}
				
				// Show the UI if needed. Use avility config for this.
				if(this.activeAbilityConfig.requiresAbilityBar) {
					newIsAbilityBar = true;
				}
			}
			
			// Handle the grid interactor.
			if(!oldIsGridInteractor && newIsGridInteractor) {
				this.publisher.publish(this.publishMsg.gPushGridInteractorGroup, this.gridInterractor);
			}else if(oldIsGridInteractor && !newIsGridInteractor) {
				this.publisher.publish(this.publishMsg.gPopGridInteractorGroup);
			}
			
			// Handle ability bar visibility.
			if(!oldIsAbilityBar && newIsAbilityBar) {
				this.publisher.publish(this.publishMsg.gShowPlayerAbilityBar, {playerAbilityID: this.activeAbilityType, cbcStopAbility: {callback: this.onAbilityBarCloseRequest, context: this}});
			}else if(oldIsAbilityBar && !newIsAbilityBar) {
				this.publisher.publish(this.publishMsg.gHidePlayerAbilityBar, null);
			}
		},
		
		tryAbilityActivate_RANDOMIZE_COLORS: function() {
			if(this.app.profileData.getDiamonds() < 1) {
				// Not enough diamonds.
				
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.EDGE_POPUP, "sIn", true, {msgID: PopupMessageType.INSUFFICIENT_DIAMONDS_FOR_ACTION}, 0, null, null);
				
				// asd_001;// this doesn't pause the game.
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.SHOP, "sIn", true, {showExtraBackground: true}, 0, null, null);
				
				return ;
			}
			
			// Remove 1 diamond from the player.
			this.app.profileData.addDiamonds(-1);
			
			// Show a fx for "-1 Diamond" lost.
			// asd_002;
			
			this.activateAbilityPhases_RANDOMIZE_COLORS();
		},
		
		activateAbilityPhases_RANDOMIZE_COLORS: function() {
			this.publisher.publish(this.publishMsg.gGridInterractionOccured);
			
			this.game.flags.iEnablePlayTimer--;
			this.game.flags.iEnableGameInteraction--;
			this.game.flags.iEnableHudUIInteraction--;
			
			this.isAnimationActiveRandomizeColor = true;
			this.animatingBlocksListRandomizeColor = [];
			
			var blocksList = [];
			
			// Save all blocks in a list;
			if(this.game.grid.blocks) {
				for(var i=this.game.grid.blocks.length - 1; i>=0; i--) {
					var column = this.game.grid.blocks[i];
					if(column) {
						for(var j=column.length-1;j>=0;j--) {
							var entry = column[j];
							if(entry && entry.type == BlockType.SIMPLE) {
								if(entry.activeAnimation) {
									this.postError("Err 6.");
									
									return null;
								}
								
								blocksList.push(entry);
							}
						}
					}
				}
			}
			
			// Shuffle the blocks.
			MathUtil.shuffleFisherYates(blocksList);
			
			// Swap the blocks in logic only and prepare for the animation.
			for(var m=1;m<blocksList.length;m=m+2) {
				var blockA = blocksList[m-1];
				var blockB = blocksList[m];
				
				var obj;
				var parent;
				
				this.game.grid.blocks[blockB.gPos.x][blockB.gPos.y] = blockA;
				this.game.grid.blocks[blockA.gPos.x][blockA.gPos.y] = blockB;
				
				obj = blockA.gPos;
				blockA.gPos = blockB.gPos;
				blockB.gPos = obj;
				
				obj = blockA.vSettle;
				blockA.vSettle = blockB.vSettle;
				blockB.vSettle = obj;
				
				obj = blockA.hSettle;
				blockA.hSettle = blockB.hSettle;
				blockB.hSettle = obj;
				
				parent = blockA.vElems.rootVisual.parent;
				if(parent) {
					parent.removeChild(blockA.vElems.rootVisual);
					parent.addChild(blockA.vElems.rootVisual);
				}
				
				parent = blockB.vElems.rootVisual.parent;
				if(parent) {
					parent.removeChild(blockB.vElems.rootVisual);
					parent.addChild(blockB.vElems.rootVisual);
				}
				
				this.animatingBlocksListRandomizeColor.push({
					block: blockA,
					targetVPos: {
						x: blockB.vElems.rootVisual.x,
						y: blockB.vElems.rootVisual.y
					}
				});
				
				this.animatingBlocksListRandomizeColor.push({
					block: blockB,
					targetVPos: {
						x: blockA.vElems.rootVisual.x,
						y: blockA.vElems.rootVisual.y
					}
				});
			}
		},
		
		onGGameLoopGameElements: function(gameTime, deltaMS, deltaS) {
			if(this.isAnimationActiveRandomizeColor) {
				var isActiveAnimation = false;
				for(var i=this.animatingBlocksListRandomizeColor.length-1;i>=0;i--) {
					var entry = this.animatingBlocksListRandomizeColor[i];
					
					if(entry) {
						var dist = MathUtil.computeDistance(entry.block.vElems.rootVisual.x, entry.block.vElems.rootVisual.y, entry.targetVPos.x, entry.targetVPos.y);
						var moveSpeed = this.cfg.colorRandomizeBlockSpeedS * deltaS;
						// Add a speed change with distance;
						moveSpeed *= Math.max(0.15, Math.min(1, Math.abs(dist) / 300));
						
						if(moveSpeed >= dist) {
							// Animation finished.
							
							entry.block.vElems.rootVisual.x = entry.targetVPos.x;
							entry.block.vElems.rootVisual.y = entry.targetVPos.y;
							
							if(this.animatingBlocksListRandomizeColor.length > 1 && i < this.animatingBlocksListRandomizeColor.length - 1) {
								this.animatingBlocksListRandomizeColor[i] = this.animatingBlocksListRandomizeColor[this.animatingBlocksListRandomizeColor.length - 1];
							}
							this.animatingBlocksListRandomizeColor.pop();
						}else {
							isActiveAnimation++;
							
							var angR = Math.atan2(entry.targetVPos.y - entry.block.vElems.rootVisual.y, entry.targetVPos.x - entry.block.vElems.rootVisual.x);
							
							entry.block.vElems.rootVisual.x += Math.cos(angR) * moveSpeed;
							entry.block.vElems.rootVisual.y += Math.sin(angR) * moveSpeed;
						}
					}
				}
				
				if(!isActiveAnimation) {
					this.isAnimationActiveRandomizeColor = false;
					this.animatingBlocksListRandomizeColor = null;
					
					this.game.flags.iEnablePlayTimer++;
					this.game.flags.iEnableGameInteraction++;
					this.game.flags.iEnableHudUIInteraction++;
				}
			}
		},
		
		onAbilityBarCloseRequest: function() {
			this.onGSetActivePlayerAbility(null);
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
