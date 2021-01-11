/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/PlayPhaseType",
	"lib/pixijs/pixi",
	"app/ingamea/util/BlockType",
	"app/ingamea/util/BlockModifierType",
	"app/ingamea/util/SimpleBlockVersion",
	
	"app/util/MathUtil",
	"app/util/VisualUtil"
], function(declare, ModuleBase,
	PlayPhaseType, PIXI, BlockType, BlockModifierType, SimpleBlockVersion, 
	MathUtil, VisualUtil
){
	"use strict";
	return declare("app.ingamea.BlocksFocusHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitDependantComponents", cbName:"onGInitDependantComponents", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gCreateBlockItem:{channel:"gCreateBlockItem", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game: null,
		
		cfg: {
		},
		
		constructor: function() {
			//void
		},
		
		onGInitDependantComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.game.grid.blocksInterractor.register("blockTouchStart", this.onBlockTouchStart, this, false);
		},
		
		onBlockTouchStart: function(block) {
			// Make blocks match check.
			console.log("block touch start.");
			//asd_001;
		},
		
		/*asd_001
		onGPlayPhaseChanged: function(playPhase) {
			if(this.game.playPhase == PlayPhaseType.BLOCKS_LINE_ACTIVE) {
				// Dispose current boxes line if exists.
				if(this.game.blocksLine) {
					this.disposeBlocksLine();
				}
				
				// Create a boxes line.
				this.createBoxesLine();
			}else {
				// Dispose current boxes line if exists.
				if(this.game.blocksLine) {
					this.disposeBlocksLine();
				}
			}
		},
		
		onGGameLoopGameElements: function(gameTime, deltaMS, deltaS) {
			if(this.activeSwap) {
				var n = this.activeSwap.completionPercent;
				
				this.activeSwap.completionPercent = Math.min(1, this.activeSwap.completionPercent + this.cfg.swapSpeed * deltaS);
				
				if(n < 0.5 && this.activeSwap.completionPercent >= 0.5) {
					var m = this.activeSwap.startBlock.userData.gPos.x;
					this.activeSwap.startBlock.userData.gPos.x = this.activeSwap.endBlock.userData.gPos.x;
					this.activeSwap.endBlock.userData.gPos.x = m;
					
					var entry = this.game.blocksLine[this.activeSwap.startGridX];
					this.game.blocksLine[this.activeSwap.startGridX] = this.game.blocksLine[this.activeSwap.endGridX];
					this.game.blocksLine[this.activeSwap.endGridX] = entry;
				}
				
				this.activeSwap.startBlock.vElems.rootVisual.x = this.activeSwap.startBlockSlotX + (this.activeSwap.endBlockSlotX - this.activeSwap.startBlockSlotX) * this.activeSwap.completionPercent;
				
				this.activeSwap.endBlock.vElems.rootVisual.x = this.activeSwap.endBlockSlotX + (this.activeSwap.startBlockSlotX - this.activeSwap.endBlockSlotX) * this.activeSwap.completionPercent;
				
				if(this.activeSwap.completionPercent >= 1) {
					this.activeSwap = null;
				}
			}
			
			asd_001;
			if(this.focusBlockIndice >= 0 && this.focusShowPercent < 1) {
				// Animate in the visuals.
				
				var n = Math.min(1, this.focusShowPercent + this.cfg.focusAnimationSpeed * deltaS);
				
				this.updateFocusShowPercent(n);
			}else if(this.focusBlockIndice < 0 && this.focusShowPercent > 0) {
				// Animate out the visuals.
				
				var n = Math.max(0, this.focusShowPercent - this.cfg.focusAnimationSpeed * deltaS);
				
				this.updateFocusShowPercent(n);
			}
		},
		
		createBoxesLine: function() {
			this.game.blocksLine = [];
			
			var blockVersions = [
				SimpleBlockVersion.BLUE,
				SimpleBlockVersion.PURPLE,
				SimpleBlockVersion.PINK,
				SimpleBlockVersion.RED,
				SimpleBlockVersion.BROWN,
				SimpleBlockVersion.ORANGE,
				SimpleBlockVersion.YELLOW,
				SimpleBlockVersion.GREEN,
				SimpleBlockVersion.GREY
			];
			
			for(var i=0;i<this.game.wave.template.gridWidth;i++) {
				var version = blockVersions[MathUtil.randomInRangeInt(0, blockVersions.length - 1)];
				
				var entry = this.publisher.publish(this.publishMsg.gCreateBlockItem, BlockType.SIMPLE, version, BlockModifierType.NONE);
				if(!entry.userData) {
					entry.userData = {};
				}
				entry.userData.gPos = {x: i, y: -1};
				
				entry.vElems.rootVisual.interactive = true;
				entry.vElems.rootVisual.on('touchstart', this.boundFunction_onBlockTouchStart);
				
				entry.vElems.rootVisual.position.set(this.gridX_To_VisualX(i), this.game.cfg.grid.cellSize.height / 2);
				this.game.layers.lineBlocks.addChild(entry.vElems.rootVisual);
				
				this.game.blocksLine[i] = entry;
			}
		},
		
		onBlockTouchStart: function(data) {
			// Allow only one touch stream.
			if(this.isBlockTouchActive) {
				return ;
			}
			
			var gridX = data.target.userData.block.userData.gPos.x;
			var blockItem = this.game.blocksLine[gridX];
			
			this.isBlockTouchActive = true;
			this.endActiveSwap();
			
			// Add event listeners.
			blockItem.vElems.rootVisual.on('touchend', this.boundFunction_onBlockTouchEnd);
			blockItem.vElems.rootVisual.on('touchendoutside', this.boundFunction_onBlockTouchEndOutside);
			blockItem.vElems.rootVisual.on('touchmove', this.boundFunction_onBlockTouchMove);
			
			this.setFocusBlockIndice(gridX, true, false, false, false);
		},
		
		onBlockTouchEnd: function(data) {
			this.isBlockTouchActive = false;
			
			var gridX = data.target.userData.block.userData.gPos.x;
			var blockItem = this.game.blocksLine[gridX];
			
			// Remove event listeners.
			blockItem.vElems.rootVisual.off('touchend', this.boundFunction_onBlockTouchEnd);
			blockItem.vElems.rootVisual.off('touchendoutside', this.boundFunction_onBlockTouchEndOutside);
			blockItem.vElems.rootVisual.off('touchmove', this.boundFunction_onBlockTouchMove);
			
			// A tap event occured, show the full indicator.
			this.setFocusBlockIndice(gridX, true);
		},
		
		onBlockTouchEndOutside: function(data) {
			this.isBlockTouchActive = false;
			
			var gridX = data.target.userData.block.userData.gPos.x;
			var blockItem = this.game.blocksLine[gridX];
			
			// Remove event listeners.
			blockItem.vElems.rootVisual.off('touchend', this.boundFunction_onBlockTouchEnd);
			blockItem.vElems.rootVisual.off('touchendoutside', this.boundFunction_onBlockTouchEndOutside);
			blockItem.vElems.rootVisual.off('touchmove', this.boundFunction_onBlockTouchMove);
		},
		
		onBlockTouchMove: function(data) {
			console.log("asd move");
		},
		
		setFocusBlockIndice: function(focusBlockIndice, showBox, animateBox, showArrow, animateArrow) {
			if(this.focusBlockIndice != focusBlockIndice) {
				this.focusBlockIndice = focusBlockIndice;
				
				if(this.focusBlockIndice >= 0) {
					if(this.focusBoxVisual) {
						if(this.focusBoxVisual.parent) {
							this.focusBoxVisual.parent.removeChild(this.focusBoxVisual);
						}
						
						this.focusBoxVisual = null;
					}
					if(this.focusArrowVisual) {
						if(this.focusArrowVisual.parent) {
							this.focusArrowVisual.parent.removeChild(this.focusArrowVisual);
						}
						
						this.focusArrowVisual = null;
					}
				}
			}
			
			if(this.focusBlockIndice < 0) {
				// Hide all indicators.
				
				this.showFocusBox = false;
				if(!animateBox) {
					this.focusBoxShowPercent = 0;
				}
				
				this.showFocusArrow = false;
				if(!animateArrow) {
					this.focusArrowShowPercent = 0;
				}
			}else {
				// Show some indicators.
				
				if(showBox) {
					this.showFocusBox = true;
					
					if(!animateBox) {
						this.focusBoxShowPercent = 1;
					}
				}
				
				if(showArrow) {
					this.showFocusArrow = true;
					
					if(!animateArrow) {
						this.focusBoxShowPercent = 1;
					}
				}
			}
			
			this.syncFocusVisuals();
		},
		
		createIndicatorVisuals: function() {
			this.vElems = {
				swapTargetBox: VisualUtil.createConfiguredVisual(this.app, this.moduleBranch, this.vConfig.swapTargetBox.visual, null),
				swapTargetArrowLR: VisualUtil.createConfiguredVisual(this.app, this.moduleBranch, this.vConfig.swapTargetArrowLR.visual, null),
				swapTargetArrowL: VisualUtil.createConfiguredVisual(this.app, this.moduleBranch, this.vConfig.swapTargetArrowL.visual, null),
				swapTargetArrowR: VisualUtil.createConfiguredVisual(this.app, this.moduleBranch, this.vConfig.swapTargetArrowR.visual, null)
			};
		},
		
		syncFocusVisuals: function() {
			asd_001;
			asds;
			asd;
			if(this.vElems) {
				if(this.vElems.swapTargetBox.parent) {
					this.vElems.swapTargetBox.parent.removeChild(this.vElems.swapTargetBox);
				}
				if(this.vElems.swapTargetArrowLR.parent) {
					this.vElems.swapTargetArrowLR.parent.removeChild(this.vElems.swapTargetArrowLR);
				}
				if(this.vElems.swapTargetArrowL.parent) {
					this.vElems.swapTargetArrowL.parent.removeChild(this.vElems.swapTargetArrowL);
				}
				if(this.vElems.swapTargetArrowR.parent) {
					this.vElems.swapTargetArrowR.parent.removeChild(this.vElems.swapTargetArrowR);
				}
				
				this.focusBoxVisual = null;
				this.focusArrowVisual = null;
			}
			
			if(this.focusBlockIndice >= 0) {
				// Show indicators.
				
				var blockItem = this.game.blocksLine[this.focusBlockIndice];
				
				if(!this.vElems) {
					this.createIndicatorVisuals();
				}
				
				this.focusBoxVisual = this.vElems.swapTargetBox;
				blockItem.vElems.rootVisual.addChild(this.focusBoxVisual);
				
				if(this.focusBlockIndice <= 0) {
					// Activate right arrow.
					
					this.focusArrowVisual = this.vElems.swapTargetArrowR;
				}else if(this.focusBlockIndice >= this.game.wave.template.gridWidth - 1) {
					// Activate left arrow.
					
					this.focusArrowVisual = this.vElems.swapTargetArrowL;
				}else {
					// Ativate 2 sided arrow.
					
					this.focusArrowVisual = this.vElems.swapTargetArrowLR;
				}
				blockItem.vElems.rootVisual.addChild(this.focusArrowVisual);
				
				asd_001;
				if(allowAnimation) {
					this.updateFocusShowPercent(0);
				}else {
					this.updateFocusShowPercent(1);
				}
			}
			asde;
			
			if(this.focusShowPercent <= 0 && this.focusBlockIndice < 0) {
				// Remove visuals.
				if(this.focusBoxVisual) {
					if(this.focusBoxVisual.parent) {
						this.focusBoxVisual.parent.removeChild(this.focusBoxVisual);
					}
					
					this.focusBoxVisual = null;
				}
				if(this.focusArrowVisual) {
					if(this.focusArrowVisual.parent) {
						this.focusArrowVisual.parent.removeChild(this.focusArrowVisual);
					}
					
					this.focusArrowVisual = null;
				}
			}else {
				var n = this.focusShowPercent;
				
				// Update the visuals.
				if(this.focusBoxVisual) {
					this.focusBoxVisual.scale.set(n, n);
				}
				if(this.focusArrowVisual) {
					this.focusArrowVisual.scale.set(n, n);
					this.focusArrowVisual.y = -90 * n;
				}
			}
		},
		
		endActiveSwap: function() {
			if(this.activeSwap) {
				this.activeSwap.startBlock.vElems.rootVisual.x = this.activeSwap.endBlockSlotX;
				this.activeSwap.endBlock.vElems.rootVisual.x = this.activeSwap.startBlockSlotX;
				
				this.activeSwap = null;
			}
		},
		
		gridX_To_VisualX: function(gridX) {
			return this.game.cfg.grid.cellSize.width * gridX + this.game.cfg.grid.cellSize.width / 2;
		},
		
		swapBlocks: function(startGridX, endGridX, doAnimation) {
			if(this.activeSwap) {
				this.endActiveSwap();
			}
			
			this.activeSwap = {
				completionPercent: 0,
				
				startBlock: this.game.blocksLine[startGridX],
				startGridX: startGridX,
				startBlockSlotX: this.gridX_To_VisualX(startGridX),
				
				endBlock: this.game.blocksLine[endGridX],
				endGridX: endGridX,
				endBlockSlotX: this.gridX_To_VisualX(endGridX)
			};
			
			var parent = this.activeSwap.startBlock.vElems.rootVisual.parent;
			if(parent) {
				this.activeSwap.startBlock.vElems.rootVisual.parent.removeChild(this.activeSwap.startBlock.vElems.rootVisual);
				parent.addChild(this.activeSwap.startBlock.vElems.rootVisual);
			}
		},
		
		disposeBlocksLine: function() {
			if(this.game.blocksLine) {
				while(this.game.blocksLine.length > 0) {
					var entry = this.game.blocksLine.pop();
					if(entry) {
						entry.vElems.rootVisual.removeAllListeners();
						
						this.app.cache.releaseVElems(this.moduleBranch, entry.vElems);
						
						entry.vElems = null;
					}
				}
				
				this.game.blocksLine = null;
			}
		},
		
		destroy: function() {
			if(this.vElems) {
				this.app.cache.releaseVElems(this.moduleBranch, this.vElems);
				
				this.vElems = null;
			}
			
			this.disposeBlocksLine();
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
		/*asd_001
		onBlockTouchTap: function(data) {
			if(this.game.isUnpaused) {
				var gridX = data.target.userData.block.userData.gPos.x;
				
				if(this.focusBlockIndice >= 0) {
					// There is already a active block.
					
					if(Math.abs(this.focusBlockIndice - gridX) == 1) {
						// A valid swap is requested.
						
						// Start a swap animation.
						this.swapBlocks(this.focusBlockIndice, gridX, true);
						
						this.setFocusBlockIndice(-1);
					}else if(this.focusBlockIndice == gridX) {
						// The block with focus touched, removing focus.
						this.setFocusBlockIndice(-1);
					}else {
						// Invalid swap, activate the new block.
						
						this.setFocusBlockIndice(gridX);
					}
				}else {
					// No active block exists, activating the touched one.
					
					this.setFocusBlockIndice(gridX);
				}
			}
		},*/
		
	});
});
