/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/BlockType",
	"app/ingamea/util/SimpleBlockVersion",
	"lib/pixijs/pixi",
	"app/util/MathUtil",
	"app/ingamea/entity/DTBlockAnimationTemplate",
	
	"app/ingamea/entity/DTBlockAnimationPropertyKeyframe"
], function(declare, ModuleBase,
	BlockType, SimpleBlockVersion, PIXI, MathUtil, DTBlockAnimationTemplate,
	DTBlockAnimationPropertyKeyframe
){
	"use strict";
	return declare("app.ingamea.WorldGenerator", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false},
			{channel:"gGenerateNewGameBoard", cbName:"onGGenerateNewGameBoard", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gResetBlocksGrid:{channel:"gResetBlocksGrid", listenerType:"any", mustExist:true},
			gCreateGridBlocksList:{channel:"gCreateGridBlocksList", listenerType:"any", mustExist:true}
		},
		
		app: null,
		game: null,
		
		cfg: {
		},
		
		constructor: function() {
			//void
		},
		
		onGInitBaseComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
		},
		
		onGGenerateNewGameBoard: function() {
			this.publisher.publish(this.publishMsg.gResetBlocksGrid);
			
			var starColumns = [];
			// Get star columns.
			for(var i=0;i<this.game.waveTemplate.starColumns.length;i++) {
				var entry = this.game.waveTemplate.starColumns[i];
				
				var n;
				if(typeof entry == "number") {
					n = entry;
				}else {
					n = MathUtil.randomInRangeInt(entry.min, entry.max);
				}
				starColumns.push(n);
			}
			
			var gridColumnStarsStatus = [];
			var nonStarColumns = [];
			for(var i=0;i<this.game.waveTemplate.gridWidth;i++) {
				var isStarColumn = false;
				for(var n=0;n<starColumns.length;n++) {
					if(starColumns[n] == i) {
						isStarColumn = true;
					}
				}
				
				if(!isStarColumn) {
					nonStarColumns.push(i);
					
					gridColumnStarsStatus[i] = false;
				}else {
					gridColumnStarsStatus[i] = true;
				}
			}
			
			var grid = [];
			var gridColumnEntries = [];
			
			var baseLinesCount = this.game.waveTemplate.gridHeight;
			var minLinesCount = baseLinesCount - 1;
			var maxLinesCount = baseLinesCount + 1;
			var prevColumnLinesCount = this.game.waveTemplate.gridHeight;
			for(var i=0;i<this.game.waveTemplate.gridWidth;i++) {
				var currentLinesCount = prevColumnLinesCount;
				
				var r = Math.random();
				if(r < 0.33) {
					if(prevColumnLinesCount > minLinesCount) {
						// Remove block.
						
						currentLinesCount = prevColumnLinesCount - 1;
					}
				}else if(r < 0.66) {
					if(prevColumnLinesCount < maxLinesCount) {
						// Add block.
						
						currentLinesCount = prevColumnLinesCount + 1;
					}
				}
				
				// Create the blocks column.
				grid[i] = [];
				gridColumnEntries[i] = 0;
				
				for(var j=0;j<currentLinesCount;j++) {
					var n = MathUtil.randomInRangeInt(0, this.game.waveTemplate.simpleBlockVersions.length - 1);
					var blockVersion = this.game.waveTemplate.simpleBlockVersions[n];
					grid[i][j] = {
						type: BlockType.SIMPLE,
						version: blockVersion
					};
					gridColumnEntries[i]++;
				}
				
				prevColumnLinesCount = currentLinesCount;
			}
			
			// Place stars on highest columns.
			for(var i=0;i<starColumns.length;i++) {
				var columnIndice = starColumns[i];
				grid[columnIndice][gridColumnEntries[columnIndice]] = {
					type: BlockType.STAR,
					version: "A"
				};
				
				gridColumnEntries[columnIndice]++;
			}
			
			// Create special blocks.
			for(var i=0;i<this.game.waveTemplate.specialBlocks.length;i++) {
				var entry = this.game.waveTemplate.specialBlocks[i];
				
				for(var n=0;n<entry.count;n++) {
					var retryCount = 0;
					while(true) {
						// Get a random valid column indice.
						var columnIndice;
						
						if(entry.columnType === "any") {
							columnIndice = MathUtil.randomInRangeInt(0, this.game.waveTemplate.gridWidth - 1);
						}else if(entry.columnType === "star") {
							columnIndice = starColumns[MathUtil.randomInRangeInt(0, starColumns.length - 1)];
						}else if(entry.columnType === "nonStar") {
							columnIndice = nonStarColumns[MathUtil.randomInRangeInt(0, nonStarColumns.length - 1)];
						}else {
							this.postError("Err 6.");
							
							return null;
						}
						// Column must not have any special blocks.
						// If none found, use the 
						
						// Get a random simple block entry line indice.
						// Limit line to 2/3 of height;
						var maxLineIndice = Math.ceil((gridColumnEntries[columnIndice] - 1) / 3 * 2);
						var lineIndice = MathUtil.randomInRangeInt(1, maxLineIndice);
						if(grid[columnIndice][lineIndice].type == BlockType.SIMPLE) {
							// Replace the entry with the special block.
							grid[columnIndice][lineIndice] = {
								type: entry.type,
								version: entry.version
							};
							
							break;
						}
						
						retryCount++;
						if(retryCount > 50) {
							this.postError("Err 6.");
							
							return null;
						}
					}
				}
			}
			
			// Populate blocks list.
			var blocksList = [];
			for(var i=0;i<grid.length;i++) {
				if(grid[i]) {
					for(var j=0;j<grid[i].length;j++) {
						var entry = grid[i][j];
						if(entry) {
							blocksList.push({
								gridX: i, 
								gridY: j, 
								type: entry.type, 
								version: entry.version, 
								showInAnimation: true,
								inAnimationDelayMS: 50 * j
							});
						}
					}
				}
			}
			
			this.publisher.publish(this.publishMsg.gCreateGridBlocksList, blocksList);
		},
		
		destroy: function() {
			//void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
