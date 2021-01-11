/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/InGameA",
	"app/ingamea/util/LevelID",
	"app/ingamea/util/TileID",
	"app/main/view/util/ViewType",
	"app/ingamea/util/SimpleBlockVersion",
	
	"app/ingamea/util/BlockType"
], function(declare, ModuleBase,
	InGameA, LevelID, TileID, ViewType, SimpleBlockVersion,
	BlockType
){
	"use strict";
	return declare("app.main.view.ViewInGame", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: [
		],
		subscribeListeners:[
			{channel:"initializeView", cbName:"onInitializeView", isUnique:false},
			{channel:"destroyView", cbName:"onDestroyView", isUnique:false},
			{channel:"activateInGame", cbName:"onActivateInGame", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			viewInGameActivated:{channel:"viewInGameActivated", listenerType:"list", mustExist:false},
			activateView:{channel:"activateView", listenerType:"any", mustExist:false},
			inGamePreDestruction:{channel:"inGamePreDestruction", listenerType:"any", mustExist:false},
			gGameLevelPreDestruction:{channel:"gGameLevelPreDestruction", listenerType:"any", mustExist:false},
			gGameLevelPreDestructionContent:{channel:"gGameLevelPreDestructionContent", listenerType:"any", mustExist:false},
			gGameLevelPreDestructionBaseObjects:{channel:"gGameLevelPreDestructionBaseObjects", listenerType:"any", mustExist:false},
			destroyModulesBranch:{channel:"destroyModulesBranch", listenerType:"any", mustExist:false},
			gInitializeLevel:{channel:"gInitializeLevel", listenerType:"unique", mustExist:true},
			mCreateModule:{channel:"mCreateModule", listenerType:"unique", mustExist:true},
			gGetUnpausedCounter:{channel:"gGetUnpausedCounter", listenerType:"unique", mustExist:true},
			gIncrementUnpauseState:{channel:"gIncrementUnpauseState", listenerType:"any", mustExist:false}
		},
		
		app: null,
		
		viewID: ViewType.IN_GAME,
		viewCompound: null,
		
		vElems:null,
		
		viewConfig: {
			destroyOnOutAnimationFinish: true,
			recreateOnShowRequest: true
		},
		
		viewLayout: [
		],
		
		levelConfigs: {
			Terra: {
				levelWaves: [
					null,
					{
						levelIndice: 1,
						playTime: 90 * 1000 + 1,
						gridWidth: 5,
						gridHeight: 4,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED
						],
						starColumns: [
							{min: 0, max: 1},
							{min: 2, max: 2},
							{min: 3, max: 4}
						],
						specialBlocks: [
							{count: 2, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 2, type: BlockType.BOMB, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 2,
						playTime: 80 * 1000 + 1,
						gridWidth: 5,
						gridHeight: 4,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED
						],
						starColumns: [
							{min: 0, max: 1},
							{min: 2, max: 2},
							{min: 3, max: 4}
						],
						specialBlocks: [
							{count: 2, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 1, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 1, type: BlockType.BOMB, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 3,
						playTime: 70 * 1000 + 1,
						gridWidth: 5,
						gridHeight: 5,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED
						],
						starColumns: [
							{min: 0, max: 1},
							{min: 2, max: 2},
							{min: 3, max: 4}
						],
						specialBlocks: [
							{count: 1, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 2, type: BlockType.DIAMOND, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 4,
						playTime: 60 * 1000 + 1,
						gridWidth: 5,
						gridHeight: 5,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED
						],
						starColumns: [
							{min: 0, max: 1},
							{min: 2, max: 2},
							{min: 3, max: 4}
						],
						specialBlocks: [
							{count: 1, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 2, type: BlockType.BOMB, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 5,
						playTime: 80 * 1000 + 1,
						gridWidth: 6,
						gridHeight: 4,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE
						],
						starColumns: [
							{min: 0, max: 1},
							{min: 2, max: 3},
							{min: 4, max: 5}
						],
						specialBlocks: [
							{count: 1, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 2, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LINE_DESTROYED, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 6,
						playTime: 100 * 1000 + 1,
						gridWidth: 6,
						gridHeight: 5,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE
						],
						starColumns: [
							{min: 0, max: 1},
							{min: 2, max: 3},
							{min: 4, max: 5}
						],
						specialBlocks: [
							{count: 2, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 1, type: BlockType.BOMB, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 7,
						playTime: 120 * 1000 + 1,
						gridWidth: 6,
						gridHeight: 6,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE
						],
						starColumns: [
							{min: 0, max: 1},
							{min: 2, max: 3},
							{min: 4, max: 5}
						],
						specialBlocks: [
							{count: 2, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 1, type: BlockType.BOMB, version: "A", columnType: "any"},
							{count: 1, type: BlockType.COLUMN_DESTROYED, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LOCK, version: "A", columnType: "star"},
							{count: 1, type: BlockType.KEY, version: "A", columnType: "nonStar"}
						]
					},
					{
						levelIndice: 8,
						playTime: 140 * 1000 + 1,
						gridWidth: 7,
						gridHeight: 5,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE
						],
						starColumns: [
							{min: 0, max: 1},
							{min: 3, max: 3},
							{min: 5, max: 6}
						],
						specialBlocks: [
							{count: 1, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LINE_DESTROYED, version: "A", columnType: "any"},
							{count: 1, type: BlockType.COLUMN_DESTROYED, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 9,
						playTime: 160 * 1000 + 1,
						gridWidth: 7,
						gridHeight: 6,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE
						],
						starColumns: [
							{min: 0, max: 1},
							{min: 3, max: 3},
							{min: 5, max: 6}
						],
						specialBlocks: [
							{count: 1, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 2, type: BlockType.LINE_DESTROYED, version: "A", columnType: "any"},
							{count: 1, type: BlockType.CEMENT, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 10,
						playTime: 180 * 1000 + 1,
						gridWidth: 7,
						gridHeight: 7,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE
						],
						starColumns: [
							{min: 0, max: 1},
							{min: 3, max: 3},
							{min: 5, max: 6}
						],
						specialBlocks: [
							{count: 1, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 2, type: BlockType.BOMB, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LOCK, version: "A", columnType: "star"},
							{count: 1, type: BlockType.KEY, version: "A", columnType: "nonStar"}
						]
					},
					{
						levelIndice: 11,
						playTime: 200 * 1000 + 1,
						gridWidth: 8,
						gridHeight: 6,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 3, max: 4},
							{min: 5, max: 7}
						],
						specialBlocks: [
							{count: 3, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 2, type: BlockType.COLUMN_DESTROYED, version: "A", columnType: "any"},
							{count: 2, type: BlockType.CEMENT, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 12,
						playTime: 220 * 1000 + 1,
						gridWidth: 8,
						gridHeight: 7,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 3, max: 4},
							{min: 5, max: 7}
						],
						specialBlocks: [
							{count: 1, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 1, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LINE_DESTROYED, version: "A", columnType: "any"},
							{count: 1, type: BlockType.COLUMN_DESTROYED, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LOCK, version: "A", columnType: "star"},
							{count: 1, type: BlockType.KEY, version: "A", columnType: "nonStar"}
						]
					},
					{
						levelIndice: 13,
						playTime: 220 * 1000 + 1,
						gridWidth: 8,
						gridHeight: 6,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 3, max: 4},
							{min: 5, max: 7}
						],
						specialBlocks: [
							{count: 1, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 1, type: BlockType.BOMB, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LINE_DESTROYED, version: "A", columnType: "any"},
							{count: 1, type: BlockType.CEMENT, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LOCK, version: "A", columnType: "star"},
							{count: 1, type: BlockType.KEY, version: "A", columnType: "nonStar"}
						]
					},
					{
						levelIndice: 14,
						playTime: 220 * 1000 + 1,
						gridWidth: 8,
						gridHeight: 7,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 3, max: 4},
							{min: 5, max: 7}
						],
						specialBlocks: [
							{count: 1, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 1, type: BlockType.BOMB, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 15,
						playTime: 220 * 1000 + 1,
						gridWidth: 8,
						gridHeight: 6,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 3, max: 4},
							{min: 5, max: 7}
						],
						specialBlocks: [
							{count: 2, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 1, type: BlockType.COLUMN_DESTROYED, version: "A", columnType: "any"},
							{count: 2, type: BlockType.CEMENT, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 16,
						playTime: 220 * 1000 + 1,
						gridWidth: 8,
						gridHeight: 8,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 3, max: 4},
							{min: 5, max: 7}
						],
						specialBlocks: [
							{count: 1, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 3, type: BlockType.BOMB, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LOCK, version: "A", columnType: "star"},
							{count: 1, type: BlockType.KEY, version: "A", columnType: "nonStar"}
						]
					},
					{
						levelIndice: 17,
						playTime: 220 * 1000 + 1,
						gridWidth: 9,
						gridHeight: 7,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 4, max: 4},
							{min: 6, max: 8}
						],
						specialBlocks: [
							{count: 1, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 2, type: BlockType.LINE_DESTROYED, version: "A", columnType: "any"},
							{count: 3, type: BlockType.CEMENT, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 18,
						playTime: 220 * 1000 + 1,
						gridWidth: 9,
						gridHeight: 8,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 4, max: 4},
							{min: 6, max: 8}
						],
						specialBlocks: [
							{count: 1, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LINE_DESTROYED, version: "A", columnType: "any"},
							{count: 1, type: BlockType.COLUMN_DESTROYED, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 19,
						playTime: 220 * 1000 + 1,
						gridWidth: 9,
						gridHeight: 9,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 4, max: 4},
							{min: 6, max: 8}
						],
						specialBlocks: [
							{count: 2, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 1, type: BlockType.BOMB, version: "A", columnType: "any"},
							{count: 2, type: BlockType.COLUMN_DESTROYED, version: "A", columnType: "any"},
							{count: 1, type: BlockType.CEMENT, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LOCK, version: "A", columnType: "star"},
							{count: 1, type: BlockType.KEY, version: "A", columnType: "nonStar"}
						]
					},
					{
						levelIndice: 20,
						playTime: 220 * 1000 + 1,
						gridWidth: 9,
						gridHeight: 7,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 4, max: 4},
							{min: 6, max: 8}
						],
						specialBlocks: [
							{count: 1, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 1, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LINE_DESTROYED, version: "A", columnType: "any"},
							{count: 1, type: BlockType.CEMENT, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 21,
						playTime: 220 * 1000 + 1,
						gridWidth: 9,
						gridHeight: 8,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 4, max: 4},
							{min: 6, max: 8}
						],
						specialBlocks: [
							{count: 3, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LINE_DESTROYED, version: "A", columnType: "any"},
							{count: 2, type: BlockType.CEMENT, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 22,
						playTime: 220 * 1000 + 1,
						gridWidth: 9,
						gridHeight: 9,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 4, max: 4},
							{min: 6, max: 8}
						],
						specialBlocks: [
							{count: 1, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 2, type: BlockType.BOMB, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LOCK, version: "A", columnType: "star"},
							{count: 1, type: BlockType.KEY, version: "A", columnType: "nonStar"}
						]
					},
					{
						levelIndice: 23,
						playTime: 220 * 1000 + 1,
						gridWidth: 9,
						gridHeight: 7,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN,
							SimpleBlockVersion.ORANGE
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 4, max: 4},
							{min: 6, max: 8}
						],
						specialBlocks: [
							{count: 1, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 1, type: BlockType.COLUMN_DESTROYED, version: "A", columnType: "any"},
							{count: 3, type: BlockType.CEMENT, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 24,
						playTime: 220 * 1000 + 1,
						gridWidth: 9,
						gridHeight: 8,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN,
							SimpleBlockVersion.ORANGE
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 4, max: 4},
							{min: 6, max: 8}
						],
						specialBlocks: [
							{count: 1, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 1, type: BlockType.BOMB, version: "A", columnType: "any"},
							{count: 1, type: BlockType.COLUMN_DESTROYED, version: "A", columnType: "any"},
							{count: 1, type: BlockType.CEMENT, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 25,
						playTime: 220 * 1000 + 1,
						gridWidth: 9,
						gridHeight: 9,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN,
							SimpleBlockVersion.ORANGE
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 4, max: 4},
							{min: 6, max: 8}
						],
						specialBlocks: [
							{count: 2, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 1, type: BlockType.BOMB, version: "A", columnType: "any"},
							{count: 2, type: BlockType.LINE_DESTROYED, version: "A", columnType: "any"},
							{count: 4, type: BlockType.CEMENT, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LOCK, version: "A", columnType: "star"},
							{count: 1, type: BlockType.KEY, version: "A", columnType: "nonStar"}
						]
					},
					{
						levelIndice: 26,
						playTime: 220 * 1000 + 1,
						gridWidth: 10,
						gridHeight: 8,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN,
							SimpleBlockVersion.ORANGE
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 3, max: 6},
							{min: 7, max: 9}
						],
						specialBlocks: [
							{count: 1, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 1, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LINE_DESTROYED, version: "A", columnType: "any"},
							{count: 1, type: BlockType.COLUMN_DESTROYED, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 27,
						playTime: 220 * 1000 + 1,
						gridWidth: 10,
						gridHeight: 9,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN,
							SimpleBlockVersion.ORANGE
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 3, max: 6},
							{min: 7, max: 9}
						],
						specialBlocks: [
							{count: 3, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 2, type: BlockType.COLUMN_DESTROYED, version: "A", columnType: "any"},
							{count: 1, type: BlockType.CEMENT, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 28,
						playTime: 220 * 1000 + 1,
						gridWidth: 10,
						gridHeight: 8,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN,
							SimpleBlockVersion.ORANGE
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 3, max: 6},
							{min: 7, max: 9}
						],
						specialBlocks: [
							{count: 1, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 3, type: BlockType.BOMB, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LOCK, version: "A", columnType: "star"},
							{count: 1, type: BlockType.KEY, version: "A", columnType: "nonStar"}
						]
					},
					{
						levelIndice: 29,
						playTime: 220 * 1000 + 1,
						gridWidth: 10,
						gridHeight: 9,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN,
							SimpleBlockVersion.ORANGE
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 3, max: 6},
							{min: 7, max: 9}
						],
						specialBlocks: [
							{count: 4, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LINE_DESTROYED, version: "A", columnType: "any"},
							{count: 2, type: BlockType.CEMENT, version: "A", columnType: "any"}
						]
					},
					{
						levelIndice: 30,
						playTime: 220 * 1000 + 1,
						gridWidth: 10,
						gridHeight: 10,
						simpleBlockVersions: [
							SimpleBlockVersion.BLUE,
							SimpleBlockVersion.RED,
							SimpleBlockVersion.PURPLE,
							SimpleBlockVersion.GREEN,
							SimpleBlockVersion.ORANGE
						],
						starColumns: [
							{min: 0, max: 2},
							{min: 3, max: 6},
							{min: 7, max: 9}
						],
						specialBlocks: [
							{count: 1, type: BlockType.CLOCK, version: "A", columnType: "any"},
							{count: 1, type: BlockType.DIAMOND, version: "A", columnType: "any"},
							{count: 1, type: BlockType.BOMB, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LINE_DESTROYED, version: "A", columnType: "any"},
							{count: 1, type: BlockType.COLUMN_DESTROYED, version: "A", columnType: "any"},
							{count: 1, type: BlockType.CEMENT, version: "A", columnType: "any"},
							{count: 1, type: BlockType.LOCK, version: "A", columnType: "star"},
							{count: 1, type: BlockType.KEY, version: "A", columnType: "nonStar"}
						]
					}
				]
			}
		},
		
		constructor: function() {
			//void
		},
		
		onInitializeView: function(viewID, ret) {
			if(viewID === this.viewID) {
				this.prepareView();
				
				this.viewCompound = {
					config: this.viewConfig,
					layout: this.viewLayout,
					vElems: this.vElems,
					parentLayer: this.app.layers.inGame,
					dependantViews: [
						ViewType.BACKGROUND
					],
					cbcViewLayoutUpdateFinished: null,
					cbcViewActivated: {callback: this.onViewActivated, context: this}
				};
				
				ret.viewCompound = this.viewCompound;
			}
		},
		
		prepareView: function() {
			if(!this.vElems) {
				this.app = this.publisher.publish(this.publishMsg.getApp);
				
				this.vElems = {};
				
				this.vElems.container = this.app.popAsset(this.moduleBranch, "container");
			}
			
			this.stopCurrentGameLevel();
		},
		
		onViewActivated: function(extraData) {
			this.startNewGame(extraData);
		},
		
		startNewGame:function(extraData) {
			var levelConfig = this.levelConfigs[extraData.levelID];
			if(!levelConfig) {
				this.postError("err 6");
				
				return null;
			}
			
			this.publisher.publish(this.publishMsg.mCreateModule, InGameA);
			this.publisher.publish(this.publishMsg.gInitializeLevel, this.vElems.container, levelConfig, extraData);
		},
		
		onDestroyView: function(viewID) {
			if(viewID === this.viewID) {
				this.stopCurrentGameLevel();
				
				if(this.vElems.container.parent) {
					this.vElems.container.parent.removeChild(this.vElems.container);
				}
				
				this.viewCompound = null;
			}
		},
		
		onActivateInGame: function() {
			if(this.publisher.publish(this.publishMsg.gGetUnpausedCounter) == 0) {
				this.publisher.publish(this.publishMsg.gIncrementUnpauseState, true);
			}
		},
		
		stopCurrentGameLevel:function() {
			this.publisher.publish(this.publishMsg.inGamePreDestruction);
			
			this.publisher.publish(this.publishMsg.gGameLevelPreDestruction);
			this.publisher.publish(this.publishMsg.gGameLevelPreDestructionContent);
			this.publisher.publish(this.publishMsg.gGameLevelPreDestructionBaseObjects);
			this.publisher.publish(this.publishMsg.destroyModulesBranch, "inGame");
			
			// ASD_002; implement a assets buffere on next level start and that semi-clears the buffers as needed to avoid recreating removed assets;
			// this.app.cache.clearVisualCacheGroup("inGame");
			// this.app.cache.clearObjectCacheGroup("inGame");
		},
		
		destroy: function() {
			//void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
