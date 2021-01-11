/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/BlockType",
	"app/ingamea/util/EntityVisualType",
	"app/ingamea/entity/WorldBlock",
	"lib/pixijs/pixi",
	
	"app/util/VisualUtil",
	"app/util/EventsDispatcher",
	"app/ingamea/blocksgridhandler/util/DTGridInteractor",
	"app/ingamea/entity/DTBlockAnimation",
	
	"app/ingamea/util/BlockEventType",
	"app/util/MathUtil"
], function(declare, ModuleBase,
	BlockType, EntityVisualType, WorldBlock, PIXI,
	VisualUtil, EventsDispatcher, DTGridInteractor, DTBlockAnimation,
	BlockEventType, MathUtil
){
	"use strict";
	return declare("app.ingamea.ClasicBlocksInterraction", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites: null,
		subscribeListeners:[
			{channel:"gInitGeneralContent", cbName:"onGInitGeneralContent", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gPushGridInteractorGroup:{channel:"gPushGridInteractorGroup", listenerType:"unique", mustExist:true},
			gDestroyGridBlocksFromList:{channel:"gDestroyGridBlocksFromList", listenerType:"any", mustExist:false},
			gGridInterractionOccured:{channel:"gGridInterractionOccured", listenerType:"any", mustExist:false},
			gAddExtraPlayTime:{channel:"gAddExtraPlayTime", listenerType:"any", mustExist:false}
		},
		
		app: null,
		game: null,
		
		cfg: {
			matchedBlockOutLayerDelayMS: 100,
			bombLayerDestroyDelayMS: 50,
			lineDestroyerLayerDestroyDelayMS: 60,
			columnDestroyerLayerDestroyDelayMS: 60,
			clockCollectBonusExtraTimeMS: 15 * 1000
		},
		
		constructor: function() {
			//void
		},
		
		onGInitGeneralContent: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			var gridInterractor = new DTGridInteractor({callback: this.onBlocksInteractionProcesCycle, context: this}, {
				blockTouchStart: true
			});
			this.publisher.publish(this.publishMsg.gPushGridInteractorGroup, gridInterractor, 0);
		},
		
		onBlocksInteractionProcesCycle: function(bufferedBlockEventCompounds, gameTime, deltaMS, deltaS) {
			if(this.game.grid.isSettleAnimationV || this.game.grid.isSettleAnimationH) {
				return ;
			}
			
			while(bufferedBlockEventCompounds.blockTouchStart.length > 0) {
				var blockCompound = bufferedBlockEventCompounds.blockTouchStart.pop();
				
				var block = blockCompound.block;
				var event = blockCompound.event;
				
				if(block.isDestroyed) {
					continue;
				}
				
				if(block.type == BlockType.SIMPLE) {
					// Get a array with all connected blocks.
					var groupData = this.computeBlocksGroup(block.gPos.x, block.gPos.y);
					if(groupData.connectedBlocks.length >= 3) {
						// Destroy all the connected blocks.
						this.activateMatchedBlocksDestoyFX(groupData.connectedBlocksExtra);
						
						this.publisher.publish(this.publishMsg.gDestroyGridBlocksFromList, groupData.connectedBlocks, true, true);
						
						this.publisher.publish(this.publishMsg.gGridInterractionOccured);
					}
				}else if(block.type == BlockType.STAR) {
					block.activateEvent(BlockEventType.NO_ACTIVATION, null);
				}else if(block.type == BlockType.CLOCK) {
					this.publisher.publish(this.publishMsg.gGridInterractionOccured);
					
					// Add extra time.
					this.publisher.publish(this.publishMsg.gAddExtraPlayTime, this.cfg.clockCollectBonusExtraTimeMS, true);
					
					// Activate action on clock.
					block.activateEvent(BlockEventType.COLLECTED, null);
					
					// Remove block.
					block.destroyBlock(true, false);
				}else if(block.type == BlockType.DIAMOND) {
					this.publisher.publish(this.publishMsg.gGridInterractionOccured);
					
					// Add 1 diamond to the play pot.
					this.app.profileData.addDiamonds(1);
					
					// Activate action on clock.
					block.activateEvent(BlockEventType.COLLECTED, null);
					
					// Remove block.
					block.destroyBlock(true, false);
				}else if(block.type == BlockType.BOMB) {
					this.handleBombActivation(block);
				}else if(block.type == BlockType.LINE_DESTROYED) {
					this.handleLineDestroyerActivation(block);
				}else if(block.type == BlockType.COLUMN_DESTROYED) {
					this.handleColumnDestroyerActivation(block);
				}else if(block.type == BlockType.CEMENT) {
					block.activateEvent(BlockEventType.NO_ACTIVATION, null);
				}else if(block.type == BlockType.KEY) {
					block.activateEvent(BlockEventType.NO_ACTIVATION, null);
				}else if(block.type == BlockType.LOCK) {
					block.activateEvent(BlockEventType.NO_ACTIVATION, null);
				}else {
					this.postError("Err 6.");
					
					return ;
				}
			}
		},
		
		handleBombActivation: function(block) {
			this.publisher.publish(this.publishMsg.gGridInterractionOccured);
			
			var regionBlocks = this.game.grid.getBlocksSquareRegion(block.gPos.x - 1, block.gPos.y - 1, 3, 3);
			for(var i=0;i<regionBlocks.length;i++) {
				var regionBlock = regionBlocks[i];
				if(regionBlock && regionBlock != block && (regionBlock.type == BlockType.SIMPLE || regionBlock.type == BlockType.CEMENT)) {
					var distanceIndice = MathUtil.computeDistance(block.gPos.x, block.gPos.y, regionBlock.gPos.x, regionBlock.gPos.y);
					var delayMS = distanceIndice * this.cfg.bombLayerDestroyDelayMS;
					
					// Activate action on clock.
					regionBlock.activateEvent(BlockEventType.CRUSHED, {particlesActivationDelayMS: delayMS});
					
					// Remove block.
					regionBlock.destroyBlock(false, false);
				}
			}
			
			// Activate action on clock.
			block.activateEvent(BlockEventType.EXPLODING_BOMB_SOURCE, null);
			
			// Remove block.
			block.destroyBlock(true, false);
		},
		
		handleLineDestroyerActivation: function(block) {
			this.publisher.publish(this.publishMsg.gGridInterractionOccured);
			
			var regionBlocks = this.game.grid.getBlocksSquareRegion(0, block.gPos.y, this.game.waveTemplate.gridWidth, 1);
			for(var i=0;i<regionBlocks.length;i++) {
				var regionBlock = regionBlocks[i];
				if(regionBlock && regionBlock != block && (regionBlock.type == BlockType.SIMPLE || regionBlock.type == BlockType.CEMENT)) {
					var distanceIndice = MathUtil.computeDistance(block.gPos.x, block.gPos.y, regionBlock.gPos.x, regionBlock.gPos.y);
					var delayMS = distanceIndice * this.cfg.lineDestroyerLayerDestroyDelayMS;
					
					// Activate action on clock.
					regionBlock.activateEvent(BlockEventType.CRUSHED, {particlesActivationDelayMS: delayMS});
					
					// Remove block.
					regionBlock.destroyBlock(false, false);
				}
			}
			
			// Activate action on clock.
			block.activateEvent(BlockEventType.EXPLODING_LINE_DESTROYER_SOURCE, null);
			
			// Remove block.
			block.destroyBlock(true, false);
		},
		
		handleColumnDestroyerActivation: function(block) {
			this.publisher.publish(this.publishMsg.gGridInterractionOccured);
			
			var regionBlocks = this.game.grid.getBlocksSquareRegion(block.gPos.x, 0, 1, this.game.grid.blocksPerColumn[block.gPos.x]);
			for(var i=0;i<regionBlocks.length;i++) {
				var regionBlock = regionBlocks[i];
				if(regionBlock && regionBlock != block && (regionBlock.type == BlockType.SIMPLE || regionBlock.type == BlockType.CEMENT)) {
					var distanceIndice = MathUtil.computeDistance(block.gPos.x, block.gPos.y, regionBlock.gPos.x, regionBlock.gPos.y);
					var delayMS = distanceIndice * this.cfg.columnDestroyerLayerDestroyDelayMS;
					
					// Activate action on clock.
					regionBlock.activateEvent(BlockEventType.CRUSHED, {particlesActivationDelayMS: delayMS});
					
					// Remove block.
					regionBlock.destroyBlock(false, false);
				}
			}
			
			// Activate action on clock.
			block.activateEvent(BlockEventType.EXPLODING_COLUMN_DESTROYER_SOURCE, null);
			
			// Remove block.
			block.destroyBlock(true, false);
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
			for(var i=0;i<targetBlockCompounds.length;i++) {
				var entry = targetBlockCompounds[i];
				if(entry) {
					// Activate the event of the block by using the grid handler function.
					entry.block.activateEvent(BlockEventType.DESTROY_MATCHED, {particlesActivationDelayMS: entry.distanceIndice * this.cfg.matchedBlockOutLayerDelayMS});
				}
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
					block: entry,
					distanceIndice: algoData.distanceIndice
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
			//void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
