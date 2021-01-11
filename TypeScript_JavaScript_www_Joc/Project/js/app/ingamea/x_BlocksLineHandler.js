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
	return declare("app.ingamea.BlocksLineHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitDependantComponents", cbName:"onGInitDependantComponents", isUnique:false},
			{channel:"gPlayPhaseChanged", cbName:"onGPlayPhaseChanged", isUnique:false},
			{channel:"gGameLoopGameElements", cbName:"onGGameLoopGameElements", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gCreateBlockItem:{channel:"gCreateBlockItem", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game: null,
		
		vElems: null,
		
		constructor: function() {
			//void
		},
		
		onGInitDependantComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.game.grid.blocksLine = null;
		},
		
		onGPlayPhaseChanged: function(playPhase) {
			if(this.game.playPhase == PlayPhaseType.BLOCKS_LINE_ACTIVE) {
				// Dispose current boxes line if exists.
				if(this.game.grid.blocksLine) {
					this.disposeBlocksLine();
				}
				
				// Create a boxes line.
				this.createBoxesLine();
			}else {
				// Dispose current boxes line if exists.
				if(this.game.grid.blocksLine) {
					this.disposeBlocksLine();
				}
			}
		},
		
		createBoxesLine: function() {
			this.game.grid.blocksLine = [];
			
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
				
				var entry = this.publisher.publish(this.publishMsg.gCreateBlockItem, BlockType.SIMPLE, version, BlockModifierType.NONE, {x: i, y: -1});
				
				entry.vElems.rootVisual.position.set(this.game.grid.util.gridX_To_VisualX(i), this.game.cfg.grid.cellSize.height / 2);
				this.game.layers.lineBlocks.addChild(entry.vElems.rootVisual);
				
				this.game.grid.blocksLine[i] = entry;
			}
		},
		
		asd_001;// implement in animatin.
		
		disposeBlocksLine: function() {
			if(this.game.grid.blocksLine) {
				while(this.game.grid.blocksLine.length > 0) {
					var entry = this.game.grid.blocksLine.pop();
					if(entry) {
						entry.vElems.rootVisual.removeAllListeners();
						
						this.app.cache.releaseVElems(this.moduleBranch, entry.vElems);
						
						entry.vElems = null;
					}
				}
				
				this.game.grid.blocksLine = null;
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
		
	});
});
