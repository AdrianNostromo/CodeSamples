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
	"app/main/view/util/ViewType",
	"app/main/util/PopupMessageType"
], function(declare, ModuleBase,
	BlockType, SimpleBlockVersion, EntityVisualType, VisualUtil, MathUtil, 
	PlayerAbilityType, DTGridInteractor, BlockEventType, ViewType, PopupMessageType
){
	"use strict";
	return declare("app.ingamea.PlayerAbilityPaintBlock", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gSetActivePlayerAbility", cbName:"onGSetActivePlayerAbility", isUnique:false},
			
			{channel:"gInitDependantComponents", cbName:"onGInitDependantComponents", isUnique:false},
			{channel:"gUnpausedStatusChanged", cbName:"onGUnpausedStatusChanged", isUnique:false},
			{channel:"gPlayTimeUsed", cbName:"onGPlayTimeUsed", isUnique:false},
			{channel:"gGameLoopGameLogic", cbName:"onGGameLoopGameLogic", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gShowPlayerAbilityBar:{channel:"gShowPlayerAbilityBar", listenerType:"any", mustExist:false},
			gHidePlayerAbilityBar:{channel:"gHidePlayerAbilityBar", listenerType:"any", mustExist:false},
			gDestroyGridBlocksFromList:{channel:"gDestroyGridBlocksFromList", listenerType:"any", mustExist:false},
			gGridInterractionOccured:{channel:"gGridInterractionOccured", listenerType:"any", mustExist:false},
			gCreateGridBlock:{channel:"gCreateGridBlock", listenerType:"any", mustExist:true},
			gPushGridInteractorGroup:{channel:"gPushGridInteractorGroup", listenerType:"unique", mustExist:true},
			gPopGridInteractorGroup:{channel:"gPopGridInteractorGroup", listenerType:"unique", mustExist:true},
			gIncrementUnpauseState:{channel:"gIncrementUnpauseState", listenerType:"any", mustExist:true},
			activatePopup:{channel:"activatePopup", listenerType:"any", mustExist:false}
		},
		
		app: null,
		game: null,
		
		isAbilityActive: false,
		
		gridInterractor: null,
		
		touchStartBlock: null,
		touchEndBlock: null,
		touchEndOutsideBlock: null,
		backWallTouchStart: false,
		
		touchDownFocusBlock: null,
		focusBlockDragH: 0,
		focusBlockDragV: 0,
		
		paintSourceBlock: null,
		indicatorShowPercent: 0,
		reshowSourceIndicatorRequired: false,
		
		cfg: {
			indicatorInSpeedS: 4,
			indicatorOutSpeedS: 8,
			gridSlotSize: {
				width: 100,
				height: 100
			},
			visualIndicatorPSize: {
				width: 128 / 128,
				height: 128 / 128
			},
			visualArrowRightPSize: {
				width: 84 / 128,
				height: 115 / 128
			}
		},
		
		constructor: function() {
			//void
		},
		
		onGInitDependantComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.gridInterractor = new DTGridInteractor({callback: this.onBlocksInteractionProcesCycle, context: this}, {
				blockTouchStart: true,
				blockTouchEndOver: true,
				blockTouchEndOutside: true
			});
		},
		
		onGSetActivePlayerAbility: function(playerAbilityType) {
			if(playerAbilityType != PlayerAbilityType.PAINT_BLOCK) {
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
		
		stopAbility: function() {
			if(this.isAbilityActive) {
				this.isAbilityActive = false;
				
				// Pop the grid interactor if active.
				this.publisher.publish(this.publishMsg.gPopGridInteractorGroup, this.gridInterractor, 1);
				
				// Hide the ability bar if active.
				this.publisher.publish(this.publishMsg.gHidePlayerAbilityBar, PlayerAbilityType.PAINT_BLOCK, null);
				
				// Reset visuals.
				this.setActivePaintSourceBlock(null);
			}
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
		
		setTouchDownFocusBlock: function(touchDownFocusBlock) {
			if(this.touchDownFocusBlock) {
				this.touchDownFocusBlock.vElems.rootVisual.removeListener('touchmove', this.onTouchDownFocusBlockTouchMove, this);
				
				this.touchDownFocusBlock = null;
			}
			
			this.touchDownFocusBlock = touchDownFocusBlock;
			if(this.touchDownFocusBlock) {
				this.touchDownFocusBlock.vElems.rootVisual.on('touchmove', this.onTouchDownFocusBlockTouchMove, this);
			}
		},
		
		onTouchDownFocusBlockTouchMove: function(event) {
			var localPos = event.data.getLocalPosition(event.target);
			if(localPos.x < -this.cfg.gridSlotSize.width / 2) {
				this.focusBlockDragH = -1;
			}else if(localPos.x > this.cfg.gridSlotSize.width / 2) {
				this.focusBlockDragH = 1;
			}
			if(localPos.y < -this.cfg.gridSlotSize.height / 2) {
				this.focusBlockDragV = 1;
			}else if(localPos.y > this.cfg.gridSlotSize.height / 2) {
				this.focusBlockDragV = -1;
			}
		},
		
		onBlocksInteractionProcesCycle: function(bufferedBlockEventCompounds, gameTime, deltaMS, deltaS) {
			if(bufferedBlockEventCompounds.blockTouchStart.length > 0) {
				this.touchStartBlock = bufferedBlockEventCompounds.blockTouchStart[0].block;
			}
			if(bufferedBlockEventCompounds.blockTouchEndOver.length > 0) {
				this.touchEndBlock = bufferedBlockEventCompounds.blockTouchEndOver[0].block;
			}
			if(bufferedBlockEventCompounds.blockTouchEndOutside.length > 0) {
				this.touchEndOutsideBlock = bufferedBlockEventCompounds.blockTouchEndOutside[0].block;
			}
			if(bufferedBlockEventCompounds.backWallTouchStart) {
				this.backWallTouchStart = true;
			}
		},
		
		onGGameLoopGameLogic: function(gameTime, deltaMS, deltaS) {
			var currentPaintSourceBlock = this.paintSourceBlock;
			
			if(this.backWallTouchStart) {
				this.setTouchDownFocusBlock(null);
				
				currentPaintSourceBlock = null;
			}
			
			if(this.touchStartBlock) {
				if(this.touchStartBlock.type == BlockType.SIMPLE) {
					this.setTouchDownFocusBlock(this.touchStartBlock);
					
					if(currentPaintSourceBlock 
						&& currentPaintSourceBlock != this.touchStartBlock
						&& (Math.abs(this.touchStartBlock.gPos.x - currentPaintSourceBlock.gPos.x) + Math.abs(this.touchStartBlock.gPos.y - currentPaintSourceBlock.gPos.y) != 1)
					) {
						// Lose the paint source.
						
						currentPaintSourceBlock = null;
					}
				}else {
					this.setTouchDownFocusBlock(null);
				}
			}
			if(this.touchEndBlock) {
				if(this.touchEndBlock.type != BlockType.SIMPLE) {
					// Touch end on a complex block occured, lose paint source.
					currentPaintSourceBlock = null;
				}else {
					if(!this.touchDownFocusBlock) {
						// Missing touch down focus.
						
						currentPaintSourceBlock = null;
					}else if(this.touchDownFocusBlock == this.touchEndBlock) {
						if(!currentPaintSourceBlock) {
							// Set the paint source as a result of a tap.
							
							currentPaintSourceBlock = this.touchEndBlock;
						}else if(currentPaintSourceBlock === this.touchEndBlock || currentPaintSourceBlock.version === this.touchEndBlock.version) {
							// Tap on paint source occured.
							
							currentPaintSourceBlock = null;
						}else {
							if(Math.abs(this.touchEndBlock.gPos.x - currentPaintSourceBlock.gPos.x) + Math.abs(this.touchEndBlock.gPos.y - currentPaintSourceBlock.gPos.y) == 1) {
								this.setTouchDownFocusBlock(null);
								
								// Valid paint drag occured.
								// Transfer paint from: currentPaintSourceBlock to: this.touchEndBlock;
								this.paintBlock(currentPaintSourceBlock, this.touchEndBlock);
								
								// Lose paint source and focus.
								currentPaintSourceBlock = null;
							}else {
								currentPaintSourceBlock = this.touchEndBlock;
							}
						}
					}else {
						// Coding error. this.touchDownFocusBlock must be null at this location.
						this.postError("Coding err 6.");
						
						this.animation = null;
					}
				}
				
				this.setTouchDownFocusBlock(null);
			}
			
			if(this.touchEndOutsideBlock) {
				// Remove focus.
				this.setTouchDownFocusBlock(null);
				
				// Remove paint source.
				currentPaintSourceBlock = null;
			}
			
			if(this.touchDownFocusBlock) {
				if((this.focusBlockDragH != 0 && this.focusBlockDragV == 0) 
					|| (this.focusBlockDragH == 0 && this.focusBlockDragV != 0)
				) {
					var draggedOverBlock = this.game.grid.getBlock(this.touchDownFocusBlock.gPos.x + this.focusBlockDragH, this.touchDownFocusBlock.gPos.y + this.focusBlockDragV);
					
					if(draggedOverBlock && draggedOverBlock.type == BlockType.SIMPLE) {
						// Transfer paint from: this.touchDownFocusBlock to: draggedOverBlock;
						this.paintBlock(this.touchDownFocusBlock, draggedOverBlock);
						
						// Remove focus.
						this.setTouchDownFocusBlock(null);
						
						// Remove paint source.
						currentPaintSourceBlock = null;
					}
				}
			}
			
			this.touchStartBlock = null;
			this.touchEndBlock = null;
			this.touchEndOutsideBlock = null;
			this.backWallTouchStart = false;
			this.focusBlockDragH = 0;
			this.focusBlockDragV = 0;
			
			if(this.paintSourceBlock !== currentPaintSourceBlock) {
				// Update the source block.
				
				this.setActivePaintSourceBlock(currentPaintSourceBlock);
			}
			
			if((this.paintSourceBlock && (this.indicatorShowPercent < 1 || this.reshowSourceIndicatorRequired)) 
				|| (!this.paintSourceBlock && this.indicatorShowPercent > 0)
			) {
				var updateVisuals = false;
				
				if(!this.paintSourceBlock || this.reshowSourceIndicatorRequired) {
					// Animate out.
					
					this.indicatorShowPercent = Math.max(0, this.indicatorShowPercent - this.cfg.indicatorOutSpeedS * deltaS);
					
					if(this.indicatorShowPercent <= 0) {
						this.reshowSourceIndicatorRequired = false;
						
						if(!this.paintSourceBlock) {
							// Remove indicator.
							
							if(this.vElems.container.parent) {
								this.vElems.container.parent.removeChild(this.vElems.container);
							}
						}
					}else {
						updateVisuals = true;
					}
				}else {
					if(!this.vElems) {
						// Create the visuals.
						
						this.createIndicatorVisuals();
					}
					
					if(this.indicatorShowPercent <= 0) {
						// Prepare the arrows visibility.
						var sideBlocksList = [
							this.game.grid.getBlock(this.paintSourceBlock.gPos.x + 1, this.paintSourceBlock.gPos.y + 0),
							this.game.grid.getBlock(this.paintSourceBlock.gPos.x - 1, this.paintSourceBlock.gPos.y + 0),
							this.game.grid.getBlock(this.paintSourceBlock.gPos.x, this.paintSourceBlock.gPos.y + 1),
							this.game.grid.getBlock(this.paintSourceBlock.gPos.x, this.paintSourceBlock.gPos.y - 1)
						];
						var sideArrowVisuals = [
							this.vElems.arrow_right,
							this.vElems.arrow_left,
							this.vElems.arrow_up,
							this.vElems.arrow_down
						];
						
						for(var i=0;i<4;i++) {
							var block = sideBlocksList[i];
							var arrowVisual = sideArrowVisuals[i];
							
							if(block && block.type == BlockType.SIMPLE) {
								if(!arrowVisual.parent) {
									this.vElems.container.addChild(arrowVisual);
								}
							}else {
								if(arrowVisual.parent) {
									arrowVisual.parent.removeChild(arrowVisual);
								}
							}
						}
						
						var slotVisualPos = this.game.grid.util.gridPosToVisualPos(this.paintSourceBlock.gPos.x, this.paintSourceBlock.gPos.y);
						this.vElems.container.x = slotVisualPos.x;
						this.vElems.container.y = slotVisualPos.y;
					}
					
					if(!this.vElems.container.parent) {
						// Add visuals to display list.
						
						this.game.layers.gridBlockIndicator.addChild(this.vElems.container);
					}
					
					// Animate in.
					this.indicatorShowPercent = Math.min(1, this.indicatorShowPercent + this.cfg.indicatorOutSpeedS * deltaS);
					
					// Update the visuals.
					updateVisuals = true;
				}
				
				if(updateVisuals) {
					var n = this.indicatorShowPercent;
					
					this.vElems.sourceIndicator.alpha = n;
					
					if(this.vElems.arrow_right.parent) {
						this.vElems.arrow_right.alpha = n;
						this.vElems.arrow_right.width = n * (this.cfg.visualArrowRightPSize.width * this.cfg.gridSlotSize.width);
						this.vElems.arrow_right.height = n * (this.cfg.visualArrowRightPSize.height * this.cfg.gridSlotSize.height);
					}
					
					if(this.vElems.arrow_left.parent) {
						this.vElems.arrow_left.alpha = n;
						this.vElems.arrow_left.width = n * (this.cfg.visualArrowRightPSize.width * this.cfg.gridSlotSize.width);
						this.vElems.arrow_left.height = n * (this.cfg.visualArrowRightPSize.height * this.cfg.gridSlotSize.height);
					}
					
					if(this.vElems.arrow_up.parent) {
						this.vElems.arrow_up.alpha = n;
						this.vElems.arrow_up.width = n * (this.cfg.visualArrowRightPSize.width * this.cfg.gridSlotSize.width);
						this.vElems.arrow_up.height = n * (this.cfg.visualArrowRightPSize.height * this.cfg.gridSlotSize.height);
					}
					
					if(this.vElems.arrow_down.parent) {
						this.vElems.arrow_down.alpha = n;
						this.vElems.arrow_down.width = n * (this.cfg.visualArrowRightPSize.width * this.cfg.gridSlotSize.width);
						this.vElems.arrow_down.height = n * (this.cfg.visualArrowRightPSize.height * this.cfg.gridSlotSize.height);
					}
				}
			}
		},
		
		paintBlock: function(paintBucketBlock, targetBlock) {
			// Check if enough diamonds exist.
			if(this.app.profileData.getDiamonds() < 1) {
				// Not enough diamonds.
				
				this.setTouchDownFocusBlock(null);
				currentPaintSourceBlock = null;
				
				this.publisher.publish(this.publishMsg.activatePopup, PopupMessageType.INSUFFICIENT_DIAMONDS_FOR_ACTION, true);
				
				this.publisher.publish(this.publishMsg.gIncrementUnpauseState, false);
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.SHOP, "sIn", true, {
					showExtraBackground: true,
					cbcViewCloseAnimationFinish: {callback: this.onShopViewClose,  context: this}
				}, 0, null, null);
				
				return ;
			}
			
			// Remove 1 diamond from the player.
			this.app.profileData.addDiamonds(-1);
			
			// Show a fx for "-1 Diamond" lost.
			var fxPos = targetBlock.getVPos();
			this.app.visualFx.activateFxTemplate("diamondSpentInGame", 0, fxPos, this.moduleBranch, null, ViewType.IN_GAME);
			
			var blockCfg = {
				gridX: targetBlock.gPos.x, 
				gridY: targetBlock.gPos.y, 
				type: paintBucketBlock.type, 
				version: paintBucketBlock.version, 
				inAnimationConfig: null
			};
			
			targetBlock.activateEvent(BlockEventType.COLOR_PAINTING);
			
			targetBlock.destroyBlock(false, false);
			
			this.publisher.publish(this.publishMsg.gCreateGridBlock, blockCfg);
			
			this.stopAbility();
		},
		
		onShopViewClose: function() {
			this.publisher.publish(this.publishMsg.gIncrementUnpauseState, true);
		},
		
		createIndicatorVisuals: function() {
			var vElemsLocal = this.vElems = {};
			
			this.vElems.container = this.app.popAsset(this.moduleBranch, "container");
			
			this.vElems.sourceIndicator = this.app.popAsset(this.moduleBranch, "sprite", "inGame/misc/swapTargetBox");
			this.vElems.sourceIndicator.anchor.set(0.5, 0.5);
			this.vElems.sourceIndicator.width = this.cfg.visualIndicatorPSize.width * this.cfg.gridSlotSize.width;
			this.vElems.sourceIndicator.height = this.cfg.visualIndicatorPSize.height * this.cfg.gridSlotSize.height;
			vElemsLocal.container.addChild(vElemsLocal.sourceIndicator);
			
			this.vElems.arrow_right = this.app.popAsset(this.moduleBranch, "sprite", "inGame/misc/arrowSideBlock_right");
			this.vElems.arrow_right.anchor.set(0, 0.5);
			this.vElems.arrow_right.x = this.cfg.gridSlotSize.width / 2;
			this.vElems.arrow_right.rotation = 0;
			
			this.vElems.arrow_left = this.app.popAsset(this.moduleBranch, "sprite", "inGame/misc/arrowSideBlock_right");
			this.vElems.arrow_left.anchor.set(0, 0.5);
			this.vElems.arrow_left.x = -this.cfg.gridSlotSize.width / 2;
			this.vElems.arrow_left.rotation = Math.PI;
			
			this.vElems.arrow_up = this.app.popAsset(this.moduleBranch, "sprite", "inGame/misc/arrowSideBlock_right");
			this.vElems.arrow_up.anchor.set(0, 0.5);
			this.vElems.arrow_up.y = -this.cfg.gridSlotSize.height / 2;
			this.vElems.arrow_up.rotation = -Math.PI / 2;
			
			this.vElems.arrow_down = this.app.popAsset(this.moduleBranch, "sprite", "inGame/misc/arrowSideBlock_right");
			this.vElems.arrow_down.anchor.set(0, 0.5);
			this.vElems.arrow_down.y = this.cfg.gridSlotSize.height / 2;
			this.vElems.arrow_down.rotation = Math.PI / 2;
		},
		
		setActivePaintSourceBlock: function(paintSourceBlock) {
			// If the same, nothing happens.
			if(!this.paintSourceBlock && !paintSourceBlock || this.paintSourceBlock === paintSourceBlock) {
				return ;
			}
			
			if(this.paintSourceBlock) {
				this.reshowSourceIndicatorRequired = true;
			}
			
			this.paintSourceBlock = paintSourceBlock;
		},
		
		destroy: function() {
			if(this.vElems) {
				this.app.cache.releaseVElems(this.moduleBranch, this.vElems);
				
				this.vElems = null;
			}
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
