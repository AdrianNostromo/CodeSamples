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
	"app/main/view/util/ViewType",
	"app/main/util/PopupMessageType"
], function(declare, ModuleBase,
	BlockType, SimpleBlockVersion, EntityVisualType, VisualUtil, MathUtil, 
	PlayerAbilityType, ViewType, PopupMessageType
){
	"use strict";
	return declare("app.ingamea.PlayerAbilityRandomizeBlockColors", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitDependantComponents", cbName:"onGInitDependantComponents", isUnique:false},
			{channel:"gSetActivePlayerAbility", cbName:"onGSetActivePlayerAbility", isUnique:false},
			{channel:"gGameLoopGameElements", cbName:"onGGameLoopGameElements", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false},
			gGridInterractionOccured:{channel:"gGridInterractionOccured", listenerType:"any", mustExist:false},
			gIncrementUnpauseState:{channel:"gIncrementUnpauseState", listenerType:"any", mustExist:true},
			activatePopup:{channel:"activatePopup", listenerType:"any", mustExist:false}
		},
		
		app: null,
		game: null,
		
		isAnimationActive: false,
		
		phaseType: {
			COLOR_FADE_OUT: "COLOR_FADE_OUT",
			PRE_COLOR_CHANGE_DELAY: "PRE_COLOR_CHANGE_DELAY",
			COLOR_FADE_IN: "COLOR_FADE_IN"
		},
		
		cfg: {
			colorRandomizeBlockSpeedS: 600
		},
		
		constructor: function() {
			//void
		},
		
		onGInitDependantComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
		},
		
		onGSetActivePlayerAbility: function(playerAbilityType) {
			if(playerAbilityType != PlayerAbilityType.RANDOMIZE_COLORS) {
				return ;
			}
			
			if(this.isAnimationActive) {
				return ;
			}
			
			if(this.app.profileData.getDiamonds() < 1) {
				// Not enough diamonds.
				
				this.publisher.publish(this.publishMsg.activatePopup, PopupMessageType.INSUFFICIENT_DIAMONDS_FOR_ACTION, true);
				
				this.publisher.publish(this.publishMsg.gIncrementUnpauseState, false);
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.SHOP, "sIn", true, {
					showExtraBackground: true,
					cbcViewCloseAnimationFinish: {callback: this.onShopViewClose,  context: this}
				}, 0, null, null);
			}else {
				// Remove 1 diamond from the player.
				this.app.profileData.addDiamonds(-1);
				
				// Show a fx for "-1 Diamond" lost.
				var fxPos = {
					x: this.app.appSize.width * ((68 + 126) / 640),
					y: this.app.appSize.height * ((907 - 60) / 960)
				};
				this.app.visualFx.activateFxTemplate("diamondSpentOnUserInterface", 0, fxPos, this.moduleBranch, null, ViewType.IN_GAME);
				
				// Start colors replacing phases.
				this.activateAbilityPhases();
			}
		},
		
		onShopViewClose: function() {
			this.publisher.publish(this.publishMsg.gIncrementUnpauseState, true);
		},
		
		activateAbilityPhases: function() {
			this.publisher.publish(this.publishMsg.gGridInterractionOccured);
			
			this.game.flags.iEnablePlayTimer--;
			this.game.flags.iEnableGameInteraction--;
			this.game.flags.iEnableHudUIInteraction--;
			this.game.flags.iBoardSettleBlocked++;
			
			this.isAnimationActive = true;
			this.compoundBlocksList = [];
			
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
				
				this.compoundBlocksList.push({
					block: blockA,
					targetVPos: {
						x: blockB.vElems.rootVisual.x,
						y: blockB.vElems.rootVisual.y
					}
				});
				
				this.compoundBlocksList.push({
					block: blockB,
					targetVPos: {
						x: blockA.vElems.rootVisual.x,
						y: blockA.vElems.rootVisual.y
					}
				});
			}
		},
		
		onGGameLoopGameElements: function(gameTime, deltaMS, deltaS) {
			if(this.isAnimationActive) {
				var isActiveAnimation = false;
				for(var i=this.compoundBlocksList.length-1;i>=0;i--) {
					var entry = this.compoundBlocksList[i];
					
					if(entry) {
						var dist = MathUtil.computeDistance(entry.block.vElems.rootVisual.x, entry.block.vElems.rootVisual.y, entry.targetVPos.x, entry.targetVPos.y);
						var moveSpeed = this.cfg.colorRandomizeBlockSpeedS * deltaS;
						// Add a speed change with distance;
						moveSpeed *= Math.max(0.15, Math.min(1, Math.abs(dist) / 300));
						
						if(moveSpeed >= dist) {
							// Animation finished.
							
							entry.block.vElems.rootVisual.x = entry.targetVPos.x;
							entry.block.vElems.rootVisual.y = entry.targetVPos.y;
							
							if(this.compoundBlocksList.length > 1 && i < this.compoundBlocksList.length - 1) {
								this.compoundBlocksList[i] = this.compoundBlocksList[this.compoundBlocksList.length - 1];
							}
							this.compoundBlocksList.pop();
						}else {
							isActiveAnimation++;
							
							var angR = Math.atan2(entry.targetVPos.y - entry.block.vElems.rootVisual.y, entry.targetVPos.x - entry.block.vElems.rootVisual.x);
							
							entry.block.vElems.rootVisual.x += Math.cos(angR) * moveSpeed;
							entry.block.vElems.rootVisual.y += Math.sin(angR) * moveSpeed;
						}
					}
				}
				
				if(!isActiveAnimation) {
					this.isAnimationActive = false;
					this.compoundBlocksList = null;
					
					this.game.flags.iEnablePlayTimer++;
					this.game.flags.iEnableGameInteraction++;
					this.game.flags.iEnableHudUIInteraction++;
					this.game.flags.iBoardSettleBlocked--;
				}
			}
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
