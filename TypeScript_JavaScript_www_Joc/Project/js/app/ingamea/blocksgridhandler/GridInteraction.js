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
	"app/ingamea/blocksgridhandler/util/DTGridInteractor",
	
	"app/ingamea/util/BlockEventType"
], function(declare, ModuleBase,
	PlayPhaseType, PIXI, BlockType, SimpleBlockVersion, 
	MathUtil, VisualUtil, EntityVisualType, DTGridInteractor, 
	BlockEventType
){
	"use strict";
	return declare("app.ingamea.blocksgridhandler.GridInteraction", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gPushGridInteractorGroup", cbName:"onGPushGridInteractorGroup", isUnique:true},
			{channel:"gPopGridInteractorGroup", cbName:"onGPopGridInteractorGroup", isUnique:true},
			
			{channel:"gInitDependantComponents", cbName:"onGInitDependantComponents", isUnique:false},
			{channel:"gGameLoopGameLogicPre", cbName:"onGGameLoopGameLogicPre", isUnique:false},
			{channel:"gAnimateOutGridBlocks", cbName:"onGAnimateOutGridBlocks", isUnique:false},
			{channel:"appSizeChanged",  cbName:"onAppSizeChanged", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gCreateBlockItem:{channel:"gCreateBlockItem", listenerType:"unique", mustExist:true},
			gDestroyGridBlocksFromList:{channel:"gDestroyGridBlocksFromList", listenerType:"any", mustExist:false},
			gGridInterractionOccured:{channel:"gGridInterractionOccured", listenerType:"any", mustExist:false},
			gAddExtraPlayTime:{channel:"gAddExtraPlayTime", listenerType:"any", mustExist:false}
		},
		
		app: null,
		game: null,
		
		bufferedBlockEventCompounds: null,
		isBlockEvent: false,
		
		activeGridInteractor: null,
		gridInteractorsStack: null,
		
		backTouchDetector: null,
		
		cfg: {
			boardClearLayerDelayMS: 80
		},
		
		constructor: function() {
			//void
		},
		
		onGInitDependantComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.bufferedBlockEventCompounds = null;
			
			this.gridInteractorsStack = [];
			
			this.backTouchDetector = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/touchDetector");
			this.backTouchDetector.interactive = true;
			this.backTouchDetector.on('touchstart', this.onGridBackTouchStart, this);
			this.backTouchDetector.renderable = false;
			this.game.layers.gameContentUnder.addChild(this.backTouchDetector);
			
			this.updateTouchDetectorSize();
		},
		
		onGridBackTouchStart: function(event) {
			if(this.bufferedBlockEventCompounds) {
				this.isBlockEvent = true;
				
				this.bufferedBlockEventCompounds.backWallTouchStart = true;
			}
		},
		
		onAppSizeChanged: function() {
			this.updateTouchDetectorSize();
		},
		
		updateTouchDetectorSize: function() {
			this.backTouchDetector.width = this.app.appSize.width;
			this.backTouchDetector.height = this.app.appSize.height;
		},
		
		onBlockTouchStart: function(block, event) {
			if(this.bufferedBlockEventCompounds && this.bufferedBlockEventCompounds.blockTouchStart) {
				this.isBlockEvent = true;
				
				this.bufferedBlockEventCompounds.blockTouchStart.push({block: block, event: event});
			}
		},
		
		onBlockTouchEndOver: function(block, event) {
			if(this.bufferedBlockEventCompounds && this.bufferedBlockEventCompounds.blockTouchEndOver) {
				this.isBlockEvent = true;
				
				this.bufferedBlockEventCompounds.blockTouchEndOver.push({block: block, event: event});
			}
		},
		
		onBlockTouchEndOutside: function(block, event) {
			if(this.bufferedBlockEventCompounds && this.bufferedBlockEventCompounds.blockTouchEndOutside) {
				this.isBlockEvent = true;
				
				this.bufferedBlockEventCompounds.blockTouchEndOutside.push({block: block, event: event});
			}
		},
		
		onBlockTouchEndAnywhere: function(block, event) {
			if(this.bufferedBlockEventCompounds && this.bufferedBlockEventCompounds.blockTouchEndAnywhere) {
				this.isBlockEvent = true;
				
				this.bufferedBlockEventCompounds.blockTouchEndAnywhere.push({block: block, event: event});
			}
		},
		
		onBlockTouchDrag: function(block, event) {
			if(this.bufferedBlockEventCompounds && this.bufferedBlockEventCompounds.blockTouchDrag) {
				this.isBlockEvent = true;
				
				this.bufferedBlockEventCompounds.blockTouchDrag.push({block: block, event: event});
			}
		},
		
		onGPushGridInteractorGroup: function(gridInteractor, priorityIndice) {
			if(!gridInteractor) {
				this.postError("err 6");
				
				return null;
			}
			
			var oldGridInteractor = this.activeGridInteractor;
			
			this.gridInteractorsStack[priorityIndice] = gridInteractor;
			
			this.activeGridInteractor = null;
			for(var i=this.gridInteractorsStack.length-1;i>=0;i--) {
				if(this.gridInteractorsStack[i]) {
					this.activeGridInteractor = this.gridInteractorsStack[i];
					
					break;
				}
			}
			
			this.updateListenersState(oldGridInteractor, this.activeGridInteractor);
		},
		
		onGPopGridInteractorGroup: function(gridInteractor, priorityIndice) {
			if(!this.activeGridInteractor) {
				this.postError("err 6");
				
				return null;
			}
			
			if(priorityIndice < this.gridInteractorsStack.length && this.gridInteractorsStack[priorityIndice] && this.gridInteractorsStack[priorityIndice] === gridInteractor) {
				var oldGridInteractor = this.activeGridInteractor;
				
				this.gridInteractorsStack[priorityIndice] = null;
				
				this.activeGridInteractor = null;
				for(var i=this.gridInteractorsStack.length-1;i>=0;i--) {
					if(this.gridInteractorsStack[i]) {
						this.activeGridInteractor = this.gridInteractorsStack[i];
						
						break;
					}
				}
				
				this.updateListenersState(oldGridInteractor, this.activeGridInteractor);
			}
		},
		
		updateListenersState: function(oldGridInteractor, newGridInteractor) {
			var hookCompounds = [
				{oldState: (!oldGridInteractor) ? false : oldGridInteractor.hooks.blockTouchStart, newState: (!newGridInteractor) ? false : newGridInteractor.hooks.blockTouchStart, eventName: "blockTouchStart", callback: this.onBlockTouchStart},
				{oldState: (!oldGridInteractor) ? false : oldGridInteractor.hooks.blockTouchEndOver, newState: (!newGridInteractor) ? false : newGridInteractor.hooks.blockTouchEndOver, eventName: "blockTouchEndOver", callback: this.onBlockTouchEndOver},
				{oldState: (!oldGridInteractor) ? false : oldGridInteractor.hooks.blockTouchEndOutside, newState: (!newGridInteractor) ? false : newGridInteractor.hooks.blockTouchEndOutside, eventName: "blockTouchEndOutside", callback: this.onBlockTouchEndOutside},
				{oldState: (!oldGridInteractor) ? false : oldGridInteractor.hooks.blockTouchEndAnywhere, newState: (!newGridInteractor) ? false : newGridInteractor.hooks.blockTouchEndAnywhere, eventName: "blockTouchEndAnywhere", callback: this.onBlockTouchEndAnywhere},
				{oldState: (!oldGridInteractor) ? false : oldGridInteractor.hooks.blockTouchDrag, newState: (!newGridInteractor) ? false : newGridInteractor.hooks.blockTouchDrag, eventName: "blockTouchDrag", callback: this.onBlockTouchDrag},
			];
			
			this.bufferedBlockEventCompounds = {
				backWallTouchStart: false
			};
			
			for(var i=0;i<hookCompounds.length;i++) {
				var entry = hookCompounds[i];
				
				if(entry.newState) {
					this.bufferedBlockEventCompounds[entry.eventName] = [];
				}
				
				if(!entry.oldState && entry.newState) {
					// Add listener.
					
					this.game.grid.blocksInterractor.on(entry.eventName, entry.callback, this, false);
				}else if(!entry.oldState && entry.newState) {
					// Remove listener.
					
					this.game.grid.blocksInterractor.removeListener(entry.eventName, entry.callback, this);
				}
			}
		},
		
		onGGameLoopGameLogicPre: function(gameTime, deltaMS, deltaS) {
			if(this.bufferedBlockEventCompounds) {
				if(this.isBlockEvent) {
					if(this.game.flags.iEnableGameInteraction >= 1 && !this.game.flags.levelEnded) {
						this.activeGridInteractor.cbc.callback.call(this.activeGridInteractor.cbc.context, this.bufferedBlockEventCompounds, gameTime, deltaMS, deltaS);
					}
					
					this.bufferedBlockEventCompounds.backWallTouchStart = false;
					
					for (var key in this.bufferedBlockEventCompounds) {
						if (this.bufferedBlockEventCompounds.hasOwnProperty(key)) {
							var value = this.bufferedBlockEventCompounds[key];
							
							if(value instanceof Array) {
								this.bufferedBlockEventCompounds[key] = [];
							}
						}
					}
					
					this.isBlockEvent = false;
				}
			}
		},
		
		onGAnimateOutGridBlocks: function(endDispatcherGridPos) {
			for(var i=0;i<this.game.grid.blocks.length;i++) {
				var blocksColumn = this.game.grid.blocks[i];
				for(var j=0;j<blocksColumn.length;j++) {
					var entry = blocksColumn[j];
					if(entry) {
						var distanceIndice;
						if(!endDispatcherGridPos) {
							distanceIndice = j;
						}else {
							distanceIndice = MathUtil.computeDistance(endDispatcherGridPos.x, endDispatcherGridPos.y, entry.gPos.x, entry.gPos.y);
						}
						
						entry.activateEvent(BlockEventType.BLOCK_OUT_BOARD_CLEARING, {particlesActivationDelayMS: distanceIndice * this.cfg.boardClearLayerDelayMS});
						
						entry.destroyBlock(false, false);
					}
				}
			}
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
