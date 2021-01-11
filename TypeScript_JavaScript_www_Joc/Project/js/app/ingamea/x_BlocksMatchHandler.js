/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/PlayPhaseType",
	"lib/pixijs/pixi",
	"app/ingamea/util/BlockType",
	"app/ingamea/util/SimpleBlockVersion",
	
	"app/util/MathUtil",
	"app/util/VisualUtil",
	"app/ingamea/util/EntityVisualType",
	"app/ingamea/util/FXConfig"
], function(declare, ModuleBase,
	PlayPhaseType, PIXI, BlockType, SimpleBlockVersion, 
	MathUtil, VisualUtil, EntityVisualType, FXConfig
){
	"use strict";
	return declare("app.ingamea.BlocksMatchHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			/*asd_001;// refactor to new logic is required.
			{channel:"gInitDependantComponents", cbName:"onGInitDependantComponents", isUnique:false},
			{channel:"gGameLoopGameLogic", cbName:"onGGameLoopGameLogic", isUnique:false},
			{channel:"gAnimateOutGridBlocks", cbName:"onGAnimateOutGridBlocks", isUnique:false}
			*/
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gCreateBlockItem:{channel:"gCreateBlockItem", listenerType:"unique", mustExist:true},
			gDestroyGridBlocksFromList:{channel:"gDestroyGridBlocksFromList", listenerType:"any", mustExist:false},
			gActivateFXList:{channel:"gActivateFXList", listenerType:"any", mustExist:false},
			gGridInterractionOccured:{channel:"gGridInterractionOccured", listenerType:"any", mustExist:false},
			gAddExtraPlayTime:{channel:"gAddExtraPlayTime", listenerType:"any", mustExist:false}
		},
		
		app: null,
		game: null,
		
		bufferedBlockTouches: null,
		
		cfg: {
			clockCollectBonusExtraTimeMS: 15 * 1000
		},
		
		constructor: function() {
			//void
		},
		
		onGInitDependantComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.bufferedBlockTouches = [];
			
			this.game.flags.fiEnableBlocksMatch = 1;
			
			this.game.grid.blocksInterractor.register("blockTouchStart", this.onBlockTouchStart, this, false);
		},
		
		onGGameLoopGameLogic: function(gameTime, deltaMS, deltaS) {
			while(this.bufferedBlockTouches.length > 0) {
				var block = this.bufferedBlockTouches.pop();
				
				if(block.isDestroyed) {
					// Block is not in the grid any more.
					
					continue;
				}
				
				if(block.type == BlockType.SIMPLE) {
					// Get a array with all connected blocks.
					var groupData = this.computeBlocksGroup(block.gPos.x, block.gPos.y);
					if(groupData.connectedBlocks.length >= 3) {
						// Destroy all the connected blocks.
						this.publisher.publish(this.publishMsg.gDestroyGridBlocksFromList, groupData.connectedBlocks, true, true);
						
						this.activateMatchedBlocksDestoyFX(groupData.connectedBlocksExtra);
						
						this.publisher.publish(this.publishMsg.gGridInterractionOccured);
					}
				}else if(block.type == BlockType.STAR) {
					// void
					
					//asd_002; add a effect of smaller scale bounce;
				}else if(block.type == BlockType.CLOCK) {
					this.publisher.publish(this.publishMsg.gAddExtraPlayTime, this.cfg.clockCollectBonusExtraTimeMS, true);
				}else if(block.type == BlockType.DIAMOND) {
					this.app.profileData.addDiamonds(1);
					
					// asd_002;// play a fx;
				}else if(block.type == BlockType.BOMB) {
					console.log("asd block touched: BOMB");
					
					// asd_001;
				}else if(block.type == BlockType.LINE_DESTROYED) {
					console.log("asd block touched: LINE_DESTROYED");
					
					// asd_001;
				}else if(block.type == BlockType.COLUMN_DESTROYED) {
					console.log("asd block touched: COLUMN_DESTROYED");
					
					// asd_001;
				}else if(block.type == BlockType.CEMENT) {
					console.log("asd block touched: CEMENT");
					
					// asd_001;
				}else if(block.type == BlockType.KEY) {
					console.log("asd block touched: KEY");
					
					// asd_001;
				}else if(block.type == BlockType.LOCK) {
					console.log("asd block touched: LOCK");
					
					// asd_001;
				}else {
					this.postError("Err 6.");
					
					return ;
				}
			}
		},
		
		onGAnimateOutGridBlocks: function() {
			var gridWidth = this.game.waveTemplate.gridWidth;
			var currentGridY = this.game.grid.occupiedLines - 1;
			var distanceIndice = 0;
			
			var fxList = [];
			
			while(currentGridY >= 0) {
				for(var i=0;i<gridWidth;i++) {
					var entry = this.game.grid.blocks[i][currentGridY];
					if(entry) {
						var visualPos = {
							x: entry.vElems.rootVisual.x,
							y: entry.vElems.rootVisual.y
						};
						
						fxList.push(FXConfig.newBlockDestroy(entry.type, entry.version, distanceIndice, visualPos));
						
						entry.destroyBlock();
					}
				}
				
				currentGridY--;
				distanceIndice++;
			}
			
			if(fxList.length > 0) {
				this.publisher.publish(this.publishMsg.gActivateFXList, fxList);
			}
		},
		
		/*
			targetBlockCompounds: {
				type,
				version,
				distanceIndice,
				visualPos: {
					x,
					y
				}
			}
		*/
		activateMatchedBlocksDestoyFX: function(targetBlockCompounds) {
			var fxList = [];
			for(var i=0;i<targetBlockCompounds.length;i++) {
				var entry = targetBlockCompounds[i];
				if(entry) {
					fxList.push(FXConfig.newBlockDestroy(entry.type, entry.version, entry.distanceIndice, entry.visualPos));
				}
			}
			
			this.publisher.publish(this.publishMsg.gActivateFXList, fxList);
		},
		
		onBlockTouchStart: function(block) {
			if(this.game.flags.fiEnableBlocksMatch < 1 || this.game.flags.iEnableGameInteraction < 1 || this.game.levelEndReason || this.game.cinematicMode) {
				return ;
			}
			
			// Cancel action if blocks destruction fx is in progress.
			if(this.game.grid.isSettleAnimationV || this.game.grid.isSettleAnimationH) {
				return ;
			}
			
			if(this.bufferedBlockTouches.indexOf(block) < 0) {
				this.bufferedBlockTouches.push(block);
			}
		},
		
		computeBlocksGroup: function(gridX, gridY) {
			var sourceBlock = this.game.grid.getBlock(gridX, gridY);
			
			var algoData = {
				distanceIndice: 0,
				connectedBlocks: [],
				connectedBlocksExtra: [],
				connectedBlocksCount: 0,
				layerBlocks: [sourceBlock],
				blockType: BlockType.SIMPLE,
				blockVersion: sourceBlock.version
			};
			
			do {
				this.processNextLayerBlocks(algoData);
				algoData.distanceIndice++;
			}while(algoData.layerBlocks.length > 0);
			
			return algoData;
		},
		
		processNextLayerBlocks: function(algoData) {
			var nextLayerBlocks = [];
			while(algoData.layerBlocks.length) {
				var entry = algoData.layerBlocks.pop();
				
				// block is already filtered to be connectable.
				
				// Save block in connected list.
				var m = algoData.connectedBlocks.length;
				
				algoData.connectedBlocks[m] = entry;
				// Also save a indice in the distance
				algoData.connectedBlocksExtra[m] = {
					type: entry.type,
					version: entry.version,
					distanceIndice: algoData.distanceIndice,
					visualPos: {
						x: entry.vElems.rootVisual.x,
						y: entry.vElems.rootVisual.y
					}
				};
				
				// Save connected and unprocessed blocks in the layerBlocks.
				var posOffsets = [
					{x: 1, y:0},
					{x: -1, y:0},
					{x: 0, y:1},
					{x: 0, y:-1}
				];
				
				var sideBlock;
				for(var i=0;i<posOffsets.length;i++) {
					var pOffset = posOffsets[i];
					
					if((sideBlock = this.game.grid.getBlock(entry.gPos.x + pOffset.x, entry.gPos.y + pOffset.y)) 
						&& sideBlock.type == algoData.blockType && sideBlock.version == algoData.blockVersion
						&& algoData.connectedBlocks.indexOf(sideBlock) < 0 && nextLayerBlocks.indexOf(sideBlock) < 0
					){
						nextLayerBlocks.push(sideBlock);
					}
				}
			}
			
			algoData.layerBlocks = nextLayerBlocks;
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
