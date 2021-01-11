/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/blocksgridhandler/GridInteraction",
	"app/ingamea/blocksgridhandler/BlockActionsHandler",
	
	"app/ingamea/util/BlockType",
	"app/ingamea/util/BlockEventType",
	"app/ingamea/util/EntityVisualType",
	"app/ingamea/entity/WorldBlock",
	"lib/pixijs/pixi",
	
	"app/util/VisualUtil",
	"app/util/EventsDispatcher",
	"app/ingamea/entity/DTBlockAnimation",
	"app/ingamea/entity/DTBlockAnimationTemplate",
	"app/util/ObjectUtil",
	
	"app/ingamea/entity/DTBlockAnimationPropertyKeyframe"
], function(declare, ModuleBase,
	GridInteraction, BlockActionsHandler,
	BlockType, BlockEventType, EntityVisualType, WorldBlock, PIXI,
	VisualUtil, EventsDispatcher, DTBlockAnimation, DTBlockAnimationTemplate, ObjectUtil,
	DTBlockAnimationPropertyKeyframe
){
	"use strict";
	return declare("app.ingamea.BlocksGridHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:[
			GridInteraction,
			BlockActionsHandler
		],
		subscribeListeners:[
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false},
			
			{channel:"gResetBlocksGrid", cbName:"onGResetBlocksGrid", isUnique:true},
			{channel:"gCreateGridBlock", cbName:"onGCreateGridBlock", isUnique:true},
			{channel:"gCreateBlockItem", cbName:"onGCreateBlockItem", isUnique:true},
			{channel:"gCreateGridBlocksList", cbName:"onGCreateGridBlocksList", isUnique:true},
			{channel:"gDestroyGridBlocksFromList", cbName:"onGDestroyGridBlocksFromList", isUnique:true},
			{channel:"gHandleGridBlocksSettle", cbName:"onGHandleGridBlocksSettle", isUnique:true},
			{channel:"gGameLoopGameElements", cbName:"onGGameLoopGameElements", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gBoxGridStructureChange:{channel:"gBoxGridStructureChange", listenerType:"list", mustExist:false},
			gStarBlockCollected:{channel:"gStarBlockCollected", listenerType:"any", mustExist:false}
		},
		
		app: null,
		game: null,
		
		// Settle computed but needs animation.
		resetSettleSpeedsImpulseV: false,
		resetSettleSpeedsImpulseH: false,
		
		cfg: {
			blockVerticalGravityS: 400,
			blockSideAccellerationS: 400,
			gridSlotSize: {
				width: 100,
				height: 100
			},
			distanceDelta: 0.001
		},
		
		sharedVLayouts: {
			genericBlockVLayout: [
				{
					id:"rootVisual",
					visual: {
						type: "container"
					}
				},
				{
					id:"blockVisual",
					visual: {
						type: "sprite",
						assetId: "inGame/blocks/#BLOCK_ID#",
						parentID: "rootVisual"
					},
					initParams: [
						{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
						{prop: "width", val: 100}, {prop: "height", val: 100}
					]
				}
			]
		},
		
		blockConfigs: {
			SIMPLE: {
				BLUE:{
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "SIMPLE_BLUE"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_BLUE"}
								]
							}
						},
						DESTROY_MATCHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleMatchedBlockOut",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_BLUE"}
								]
							}
						},
						CRUSHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockCrushed",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_BLUE"},
									{id: "#BLOCK_VERSION#", value: "BLUE"}
								]
							}
						},
						COLOR_PAINTING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockColorPainting",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_BLUE"}
								]
							}
						}
					}
				},
				BROWN:{
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "SIMPLE_BROWN"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_BROWN"}
								]
							}
						},
						DESTROY_MATCHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleMatchedBlockOut",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_BROWN"}
								]
							}
						},
						CRUSHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockCrushed",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_BROWN"},
									{id: "#BLOCK_VERSION#", value: "BROWN"}
								]
							}
						},
						COLOR_PAINTING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockColorPainting",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_BROWN"}
								]
							}
						}
					}
				},
				GREEN:{
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "SIMPLE_GREEN"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_GREEN"}
								]
							}
						},
						DESTROY_MATCHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleMatchedBlockOut",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_GREEN"}
								]
							}
						},
						CRUSHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockCrushed",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_GREEN"},
									{id: "#BLOCK_VERSION#", value: "GREEN"}
								]
							}
						},
						COLOR_PAINTING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockColorPainting",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_GREEN"}
								]
							}
						}
					}
				},
				GREY:{
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "SIMPLE_GREY"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_GREY"}
								]
							}
						},
						DESTROY_MATCHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleMatchedBlockOut",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_GREY"}
								]
							}
						},
						CRUSHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockCrushed",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_GREY"},
									{id: "#BLOCK_VERSION#", value: "GREY"}
								]
							}
						},
						COLOR_PAINTING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockColorPainting",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_GREY"}
								]
							}
						}
					}
				},
				ORANGE:{
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "SIMPLE_ORANGE"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_ORANGE"}
								]
							}
						},
						DESTROY_MATCHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleMatchedBlockOut",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_ORANGE"}
								]
							}
						},
						CRUSHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockCrushed",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_ORANGE"},
									{id: "#BLOCK_VERSION#", value: "ORANGE"}
								]
							}
						},
						COLOR_PAINTING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockColorPainting",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_ORANGE"}
								]
							}
						}
					}
				},
				PINK:{
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "SIMPLE_PINK"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_PINK"}
								]
							}
						},
						DESTROY_MATCHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleMatchedBlockOut",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_PINK"}
								]
							}
						},
						CRUSHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockCrushed",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_PINK"},
									{id: "#BLOCK_VERSION#", value: "PINK"}
								]
							}
						},
						COLOR_PAINTING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockColorPainting",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_PINK"}
								]
							}
						}
					}
				},
				PURPLE:{
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "SIMPLE_PURPLE"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_PURPLE"}
								]
							}
						},
						DESTROY_MATCHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleMatchedBlockOut",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_PURPLE"}
								]
							}
						},
						CRUSHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockCrushed",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_PURPLE"},
									{id: "#BLOCK_VERSION#", value: "PURPLE"}
								]
							}
						},
						COLOR_PAINTING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockColorPainting",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_PURPLE"}
								]
							}
						}
					}
				},
				RED:{
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "SIMPLE_RED"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_RED"}
								]
							}
						},
						DESTROY_MATCHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleMatchedBlockOut",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_RED"}
								]
							}
						},
						CRUSHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockCrushed",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_RED"},
									{id: "#BLOCK_VERSION#", value: "RED"}
								]
							}
						},
						COLOR_PAINTING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockColorPainting",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_RED"}
								]
							}
						}
					}
				},
				YELLOW:{
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "SIMPLE_YELLOW"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_YELLOW"}
								]
							}
						},
						DESTROY_MATCHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleMatchedBlockOut",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_YELLOW"}
								]
							}
						},
						CRUSHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockCrushed",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_YELLOW"},
									{id: "#BLOCK_VERSION#", value: "YELLOW"}
								]
							}
						},
						COLOR_PAINTING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockColorPainting",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_YELLOW"}
								]
							}
						}
					}
				},
				BLACK:{
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "SIMPLE_BLACK"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_BLACK"}
								]
							}
						},
						DESTROY_MATCHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleMatchedBlockOut",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_BLACK"}
								]
							}
						},
						CRUSHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockCrushed",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_BLACK"},
									{id: "#BLOCK_VERSION#", value: "BLACK"}
								]
							}
						},
						COLOR_PAINTING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "simpleBlockColorPainting",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "SIMPLE_BLACK"}
								]
							}
						}
					}
				}
			},
			STAR: {
				A: {
					blockBehaviour: {
						collectibleOnGroundTouch: true
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "STAR_A"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "specialBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "STAR_A"}
								]
							}
						},
						COLLECTED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "itemBlockCollectedUp",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "STAR_A"}
								]
							}
						},
						NO_ACTIVATION: {
							animationConfig: {
								delayMS: 0,
								templateID: "specialItemNoActivation"
							}
						}
					}
				}
			},
			CLOCK: {
				A: {
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "CLOCK_A"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "specialBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "CLOCK_A"}
								]
							}
						},
						COLLECTED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "itemBlockCollectedZoom",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "CLOCK_A"}
								]
							}
						},
						NO_ACTIVATION: {
							animationConfig: {
								delayMS: 0,
								templateID: "specialItemNoActivation"
							}
						}
					}
				}
			},
			DIAMOND: {
				A: {
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "DIAMOND_A"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "specialBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "DIAMOND_A"}
								]
							}
						},
						COLLECTED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "itemBlockCollectedZoom",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "DIAMOND_A"}
								]
							}
						},
						NO_ACTIVATION: {
							animationConfig: {
								delayMS: 0,
								templateID: "specialItemNoActivation"
							}
						}
					}
				}
			},
			BOMB: {
				A: {
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "BOMB_A"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "specialBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "BOMB_A"}
								]
							}
						},
						EXPLODING_BOMB_SOURCE: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "bombBlockExplosion",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "BOMB_A"}
								]
							}
						},
						NO_ACTIVATION: {
							animationConfig: {
								delayMS: 0,
								templateID: "specialItemNoActivation"
							}
						}
					}
				}
			},
			LINE_DESTROYED: {
				A: {
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "LINE_DESTROYER_A"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "specialBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "LINE_DESTROYER_A"}
								]
							}
						},
						EXPLODING_LINE_DESTROYER_SOURCE: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "lineDestroyerExplosion",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "LINE_DESTROYER_A"}
								]
							}
						},
						NO_ACTIVATION: {
							animationConfig: {
								delayMS: 0,
								templateID: "specialItemNoActivation"
							}
						}
					}
				}
			},
			COLUMN_DESTROYED: {
				A: {
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "COLUMN_DESTROYER_A"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "specialBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "COLUMN_DESTROYER_A"}
								]
							}
						},
						EXPLODING_COLUMN_DESTROYER_SOURCE: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "columnDestroyerExplosion",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "COLUMN_DESTROYER_A"}
								]
							}
						},
						NO_ACTIVATION: {
							animationConfig: {
								delayMS: 0,
								templateID: "specialItemNoActivation"
							}
						}
					}
				}
			},
			CEMENT: {
				A: {
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "CEMENT_A_1"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "specialBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "CEMENT_A_1"}
								]
							}
						},
						NO_ACTIVATION: {
							animationConfig: {
								delayMS: 0,
								templateID: "cementItemNoActivation"
							}
						},
						CRUSHED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "cementBlockCrushed",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "CEMENT_A_1"},
									{id: "#BLOCK_VERSION#", value: "BLACK"}
								]
							}
						}
					}
				}
			},
			KEY: {
				A: {
					blockBehaviour: {
						collectibleOnGroundTouch: true
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "KEY_A"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "specialBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "KEY_A"}
								]
							}
						},
						NO_ACTIVATION: {
							animationConfig: {
								delayMS: 0,
								templateID: "specialItemNoActivation"
							}
						},
						COLLECTED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "keyBlockCollectedToLock",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "KEY_A"}
								]
							}
						}
					}
				}
			},
			LOCK: {
				A: {
					blockBehaviour: {
						//void
					},
					sharedVLayoutID: "genericBlockVLayout",
					assetIDs:[
						{id: "#BLOCK_ID#", value: "LOCK_A"}
					],
					eventEffects: {
						CREATE_INITIAL: {
							animationConfig: {
								delayMS: 0,
								templateID: "blockInAll"
							}
						},
						BLOCK_OUT_BOARD_CLEARING: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "specialBlockOutBoardClearing",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "LOCK_A"}
								]
							}
						},
						NO_ACTIVATION: {
							animationConfig: {
								delayMS: 0,
								templateID: "specialItemNoActivation"
							}
						},
						UNLOCKED: {
							visualFXConfig: {
								delayMS: 0,
								templateID: "lockBlockUnlocked",
								assetIDs: [
									{id: "#BLOCK_ID#", value: "LOCK_A"}
								]
							}
						}
					}
				}
			}
		},
		
		constructor: function() {
			//void
		},
		
		onGInitBaseComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.game.grid = {
				blocks: [],
				occupiedLines: 0,
				blocksPerColumn: [],
				blocksPerLine: [],
				blocksInterractor: new EventsDispatcher(),
				getBlock: this.getBlock.bind(this),
				getBlocksSquareRegion: this.getBlocksSquareRegion.bind(this),
				util: {
					gridXToVisualX: this.gridXToVisualX.bind(this),
					gridYToVisualY: this.gridYToVisualY.bind(this),
					gridPosToVisualPos: this.gridPosToVisualPos.bind(this)
				},
				isSettleAnimationV: false,
				isSettleAnimationH: false
			};
			
			this.game.grid.blocksInterractor.register("blockDestroyRequest", this.onBlockDestroyRequest, this, false);
		},
		
		onBlockDestroyRequest: function(block, settleBlocks, resetSettleVelocities) {
			this.handleGridStatistics_blockRemoved(block.gPos.x, block.gPos.y);
			
			this.game.grid.blocks[block.gPos.x][block.gPos.y] = null;
			
			this.recycleBlock(block);
			
			if(settleBlocks) {
				this.onGHandleGridBlocksSettle(resetSettleVelocities);
			}
		},
		
		getBlock: function(gridX, gridY) {
			if(this.game.grid.blocks[gridX]) {
				return this.game.grid.blocks[gridX][gridY];
			}
			
			return null;
		},
		
		getBlocksSquareRegion: function(gridX, gridY, regionWidth, regionHeight) {
			var blocksList = [];
			for(var i=0;i<regionWidth;i++) {
				for(var j=0;j<regionHeight;j++) {
					if(this.game.grid.blocks[gridX + i]) {
						var block = this.game.grid.blocks[gridX + i][gridY + j];
						if(block) {
							blocksList.push(block);
						}
					}
				}
			}
			
			return blocksList;
		},
		
		gridXToVisualX: function(gridX) {
			return this.game.cfg.grid.cellSize.width * gridX + this.game.cfg.grid.cellSize.width / 2;
		},
		
		gridYToVisualY: function(gridY) {
			return -this.game.cfg.grid.cellSize.height * gridY - this.game.cfg.grid.cellSize.height / 2;
		},
		
		gridPosToVisualPos: function(gridX, gridY) {
			return {
				x: this.game.cfg.grid.cellSize.width * gridX + this.game.cfg.grid.cellSize.width / 2,
				y: -this.game.cfg.grid.cellSize.height * gridY - this.game.cfg.grid.cellSize.height / 2
			};
		},
		
		onGResetBlocksGrid: function() {
			if(this.game.grid) {
				this.removeAllGridBlocks();
				
				this.game.grid.blocks = [];
				for(var i=0;i<this.game.waveTemplate.gridWidth;i++) {
					this.game.grid.blocks[i] = [];
				}
				
				this.game.grid.occupiedLines = 0;
				this.game.grid.blocksPerColumn = [];
				this.game.grid.blocksPerLine = [];
			}
		},
		
		removeAllGridBlocks: function() {
			if(this.game.grid.blocks) {
				for(var i=this.game.grid.blocks.length - 1; i>=0; i--) {
					var column = this.game.grid.blocks[i];
					if(column) {
						for(var j=column.length-1;j>=0;j--) {
							var entry = column[j];
							if(entry) {
								this.onBlockDestroyRequest(entry);
							}
						}
						
						this.game.grid.blocks[i] = null;
					}
				}
				this.game.grid.blocks = null;
				
				this.game.grid.occupiedLines = 0;
				this.game.grid.blocksPerColumn = [];
				this.game.grid.blocksPerLine = [];
			}
		},
		
		recycleBlock: function(entry) {
			entry.isDestroyed = true;
			
			if(entry.vElems.rootVisual.userData.block) {
				entry.vElems.rootVisual.userData.block = null;
			}
			
			entry.vElems.rootVisual.removeAllListeners();
			
			this.app.cache.releaseVElems(this.moduleBranch, entry.vElems);
			
			entry.vElems = null;
		},
		
		onGCreateBlockItem: function(type, version, gPos) {
			var blockConfig = this.blockConfigs[type][version];
			if(!blockConfig) {
				this.postError("err 6");
				
				return null;
			}
			
			if(blockConfig.blockBehaviour.collectibleOnGroundTouch && gPos.y == 0) {
				// Stars may not be created on the first line. Generator error occured.
				
				this.postError("err 6");
				
				return null;
			}
			
			var vElems = {isVContainer: true};
			var blockVLayout;
			var sharedVLayoutAssetIDs = null;
			if(blockConfig.vLayout) {
				blockVLayout = blockConfig.vLayout;
			}else if(blockConfig.sharedVLayoutID) {
				blockVLayout = this.sharedVLayouts[blockConfig.sharedVLayoutID];
				sharedVLayoutAssetIDs = blockConfig.assetIDs;
			}else {
				this.postError("err 6");
				
				return null;
			}
			
			VisualUtil.createVLayoutVisuals(vElems, blockVLayout, this.app, this.moduleBranch, null, sharedVLayoutAssetIDs, null);
			
			var entity = new WorldBlock(this.game.grid.blocksInterractor, type, version, blockConfig, vElems, gPos);
			if(!entity.vElems.rootVisual.userData) {
				entity.vElems.rootVisual.userData = {};
			}
			entity.vElems.rootVisual.userData.block = entity;
			
			return entity;
		},
		
		onGCreateGridBlock: function(blockCfg) {
			if(blockCfg.gridX < 0 || blockCfg.gridY < 0 || blockCfg.gridX >= this.game.waveTemplate.gridWidth) {
				this.postError("err 6");
				
				return null;
			}
			
			var entity = this.onGCreateBlockItem(blockCfg.type, blockCfg.version, {x: blockCfg.gridX, y: blockCfg.gridY});
			
			var vPos = this.game.grid.util.gridPosToVisualPos(blockCfg.gridX, blockCfg.gridY);
			entity.vElems.rootVisual.position.set(vPos.x, vPos.y);
			this.game.layers.gridBlocks.addChild(entity.vElems.rootVisual);
			
			var oldOccupiedLines = this.game.grid.occupiedLines;
			
			this.handleGridStatistics_blockAdded(blockCfg.gridX, blockCfg.gridY);
			
			this.game.grid.blocks[blockCfg.gridX][blockCfg.gridY] = entity;
			
			if(oldOccupiedLines != this.game.grid.occupiedLines) {
				this.publisher.publish(this.publishMsg.gBoxGridStructureChange, false, true);
			}
			
			if(blockCfg.showInAnimation) {
				entity.activateEvent(BlockEventType.CREATE_INITIAL, {delayMS: blockCfg.inAnimationDelayMS});
			}
			
			return entity;
		},
		
		handleGridStatistics_blockAdded: function(gridX, gridY) {
			while(this.game.grid.blocksPerColumn.length <= gridX) {
				this.game.grid.blocksPerColumn[this.game.grid.blocksPerColumn.length] = 0;
			}
			this.game.grid.blocksPerColumn[gridX]++;
			
			while(this.game.grid.blocksPerLine.length <= gridY) {
				this.game.grid.blocksPerLine[this.game.grid.blocksPerLine.length] = 0;
			}
			this.game.grid.blocksPerLine[gridY]++;
			
			this.game.grid.occupiedLines = Math.max(this.game.grid.occupiedLines, gridY + 1);
		},
		
		handleGridStatistics_blockRemoved: function(gridX, gridY) {
			this.game.grid.blocksPerColumn[gridX]--;
			if(this.game.grid.blocksPerColumn[gridX] < 0) {
				this.postError("err 6");
				
				return ;
			}
			
			this.game.grid.blocksPerLine[gridY]--;
			if(this.game.grid.blocksPerLine[gridY] < 0) {
				this.postError("err 6");
				
				return ;
			}
			if(this.game.grid.blocksPerLine[gridY] <= 0 && this.game.grid.occupiedLines == gridY + 1) {
				while(this.game.grid.occupiedLines > 0 && this.game.grid.blocksPerLine[this.game.grid.occupiedLines - 1] <= 0) {
					this.game.grid.occupiedLines--;
				}
			}
		},
		
		onGCreateGridBlocksList: function(blocksList) {
			for(var i=0;i<blocksList.length;i++) {
				var entry = blocksList[i];
				if(!this.onGCreateGridBlock(entry)) {
					this.postError("err 6");
					
					return null;
				}
			}
		},
		
		onGDestroyGridBlocksFromList: function(blocksList, settleBlocks, resetSettleVelocities) {
			for(var i=0;i<blocksList.length;i++) {
				var entry = blocksList[i];
				
				entry.destroyBlock(false, false);
			}
			
			if(settleBlocks) {
				this.onGHandleGridBlocksSettle(resetSettleVelocities);
			}
		},
		
		onGHandleGridBlocksSettle: function(resetSettleVelocities) {
			var gridX = 0;
			for(var i=0;i<this.game.grid.blocks.length;i++) {
				var column = this.game.grid.blocks[i];
				
				var isColumnEmpty = true;
				
				for(var j=0;j<column.length;j++) {
					var block = column[j];
					if(block) {
						isColumnEmpty = false;
						
						// A block found, check if it can drop vertically.
						var finalGridY = j;
						while(finalGridY >= 1 && !this.game.grid.blocks[gridX][finalGridY - 1]) {
							finalGridY--;
						}
						
						if(finalGridY != block.gPos.y) {
							// Save the new grid position on the block.
							this.handleGridStatistics_blockRemoved(block.gPos.x, block.gPos.y);
							
							column[block.gPos.y] = null;
							block.gPos.y = finalGridY;
							column[block.gPos.y] = block;
							
							this.handleGridStatistics_blockAdded(block.gPos.x, block.gPos.y);
							
							var velocityY = 0;
							if(block.vSettle) {
								velocityY = block.vSettle.velocityY;
							}
							block.vSettle = {
								velocityY: velocityY,
								vPosY: this.game.grid.util.gridYToVisualY(finalGridY)
							};
						}
						
						if(gridX != block.gPos.x) {
							this.handleGridStatistics_blockRemoved(block.gPos.x, block.gPos.y);
							
							column[block.gPos.y] = null;
							block.gPos.x = gridX;
							this.game.grid.blocks[block.gPos.x][block.gPos.y] = block;
							
							this.handleGridStatistics_blockAdded(block.gPos.x, block.gPos.y);
							
							var velocityX = 0;
							if(block.hSettle) {
								velocityX = block.hSettle.velocityX;
							}
							block.hSettle = {
								velocityX: velocityX,
								vPosX: this.game.grid.util.gridXToVisualX(gridX)
							};
						}
					}
				}
				
				if(!isColumnEmpty) {
					gridX++;
				}
			}
			
			if(resetSettleVelocities) {
				this.resetSettleSpeedsImpulseV = true;
				this.resetSettleSpeedsImpulseH = true;
			}
			this.game.grid.isSettleAnimationV = true;
			this.game.grid.isSettleAnimationH = true;
		},
		
		onGGameLoopGameElements: function(gameTime, deltaMS, deltaS) {
			this.tickBlockEvents(gameTime, deltaMS, deltaS);
			
			if(this.game.flags.iBoardSettleBlocked > 0) {
				return ;
			}
			
			if(this.game.grid.isSettleAnimationV) {
				if(this.tickVSettle(deltaMS, deltaS) <= 0) {
					this.game.grid.isSettleAnimationV = false;
				}
				
				this.resetSettleSpeedsImpulseV = false;
				
				this.resetSettleSpeedsImpulseH = true;
			}else if(this.game.grid.isSettleAnimationH) {
				if(!this.tickHSettle(deltaMS, deltaS)) {
					this.game.grid.isSettleAnimationH = false;
				}
				
				this.resetSettleSpeedsImpulseH = false;
			}
		},
		
		tickBlockEvents: function(gameTime, deltaMS, deltaS) {
			for(var i=0;i<this.game.grid.blocks.length;i++) {
				var column = this.game.grid.blocks[i];
				
				for(var j=0;j<column.length;j++) {
					var block = column[j];
					if(block) {
						if(block.autoDestruction) {
							block.autoDestruction.delayMS = Math.max(0, block.autoDestruction.delayMS - deltaMS);
							
							if(block.autoDestruction.delayMS <= 0) {
								block.activateEvent(block.autoDestruction.eventType, block.autoDestruction.eventExtraData);
								
								block.destroyBlock(true, false);
							}
						}
					}
				}
			}
		},
		
		collectBlock: function(entry) {
			if(entry.type == BlockType.STAR) {
				var gridPos = entry.gPos;
				
				entry.activateEvent(BlockEventType.COLLECTED, null);
				
				entry.destroyBlock(false, false);
				
				this.publisher.publish(this.publishMsg.gStarBlockCollected, gridPos);
			}else if(entry.type == BlockType.KEY) {
				// Find the game lock and get it's visual pos.
				var lockBlock = this.getGridLockBlock();
				if(!lockBlock) {
					// A key was collected but no lock was found on the board.
					this.postError("err 6");
					
					return null;
				}
				
				var keyBlockVPos = entry.getVPos();
				var lockBlockVPos = lockBlock.getVPos();
				
				// Activate a action on the key to unlock the lock.
				// Activate a event on the lock with a fixed and good looking manual delay.
				entry.activateEvent(BlockEventType.COLLECTED, {customValues: {END_OFFSET_X: lockBlockVPos.x - keyBlockVPos.x, END_OFFSET_Y: lockBlockVPos.y - keyBlockVPos.y}});
				
				lockBlock.autoDestruction = {
					delayMS: 400,
					eventType: BlockEventType.UNLOCKED,
					eventExtraData: {
						particlesActivationDelayMS: 400
					}
				};
				
				entry.destroyBlock(true, false);
			}else {
				this.postError("err 6");
				
				return null;
			}
		},
		
		getGridLockBlock: function() {
			for(var i=0;i<this.game.grid.blocks.length;i++) {
				var column = this.game.grid.blocks[i];
				
				for(var j=0;j<column.length;j++) {
					var block = column[j];
					if(block && block.type == BlockType.LOCK) {
						return block;
					}
				}
			}
			
			return null;
		},
		
		tickVSettle: function(deltaMS, deltaS) {
			var ct = 0;
			
			for(var i=0;i<this.game.grid.blocks.length;i++) {
				var column = this.game.grid.blocks[i];
				
				for(var j=0;j<column.length;j++) {
					var block = column[j];
					if(block && block.vSettle) {
						var signedDist = block.vSettle.vPosY - block.vElems.rootVisual.y;
						var dirY = signedDist / Math.abs(signedDist);
						
						if(this.resetSettleSpeedsImpulseV) {
							block.vSettle.velocityY = 0;
						}
						
						if(this.game.flags.iBoardSettleVelocityChangeBlocked <= 0) {
							block.vSettle.velocityY += this.cfg.blockVerticalGravityS * deltaS * dirY;
						}
						
						var travelDist = block.vSettle.velocityY * deltaS;
						
						// This block makes it so blocks can't fall faster than blocks below and overlap.
						// This works only when blocks fall down. In this game version blocks fall only down.
						if(j >= 1 && column[j - 1] && column[j - 1].vSettle) {
							var underBlock = column[j - 1];
							if(block.vElems.rootVisual.y + this.cfg.gridSlotSize.height / 2 + travelDist > underBlock.vElems.rootVisual.y - this.cfg.gridSlotSize.height / 2 + this.cfg.distanceDelta) {
								travelDist = (underBlock.vElems.rootVisual.y - this.cfg.gridSlotSize.height / 2) - (block.vElems.rootVisual.y + this.cfg.gridSlotSize.height / 2);
								block.vSettle.velocityY = underBlock.vSettle.velocityY;
							}
						}
						
						if(((signedDist >= 0 && travelDist >= 0) || (signedDist < 0 && travelDist < 0)) && Math.abs(travelDist) >= Math.abs(signedDist)) {
							block.vElems.rootVisual.y = block.vSettle.vPosY;
							
							block.vSettle = null;
							
							if(block.config.blockBehaviour.collectibleOnGroundTouch && block.gPos.y == 0) {
								this.collectBlock(block);
								
								this.onGHandleGridBlocksSettle(false);
								
								// Reset the gridY to 1 because the star was at gridY 0;
								j = -1;
							}
						}else {
							block.vElems.rootVisual.y = block.vElems.rootVisual.y + travelDist;
							
							ct++;
						}
					}
				}
			}
			
			return ct;
		},
		
		tickHSettle: function(deltaMS, deltaS) {
			var ct = 0;
			
			for(var i=0;i<this.game.grid.blocks.length;i++) {
				var column = this.game.grid.blocks[i];
				
				for(var j=0;j<column.length;j++) {
					var block = column[j];
					if(block && block.hSettle) {
						var signedDist = block.hSettle.vPosX - block.vElems.rootVisual.x;
						var dirX = signedDist / Math.abs(signedDist);
						
						if(this.resetSettleSpeedsImpulseH) {
							block.hSettle.velocityX = 0;
						}
						
						if(this.game.flags.iBoardSettleVelocityChangeBlocked <= 0) {
							block.hSettle.velocityX += this.cfg.blockSideAccellerationS * deltaS * dirX;
						}
						
						var travelDist = block.hSettle.velocityX * deltaS;
						
						// This block makes it so blocks can't move to the left faster than blocks to the left of it and overlap.
						// This works only when blocks move only to the left. In this game version blocks move only to the left.
						if(i >= 1 && this.game.grid.blocks[i - 1][0] && this.game.grid.blocks[i - 1][0].hSettle) {
							var leftBlock = this.game.grid.blocks[i - 1][0];
							if(block.vElems.rootVisual.x - this.cfg.gridSlotSize.width / 2 + travelDist + this.cfg.distanceDelta < leftBlock.vElems.rootVisual.x + this.cfg.gridSlotSize.width / 2) {
								travelDist = (leftBlock.vElems.rootVisual.x + this.cfg.gridSlotSize.width / 2) - (block.vElems.rootVisual.x - this.cfg.gridSlotSize.width / 2);
								block.hSettle.velocityX = leftBlock.hSettle.velocityX;
							}
						}
						
						if(((signedDist >= 0 && travelDist >= 0) || (signedDist < 0 && travelDist < 0)) && Math.abs(travelDist) >= Math.abs(signedDist)) {
							block.vElems.rootVisual.x = block.hSettle.vPosX;
							
							block.hSettle = null;
						}else {
							block.vElems.rootVisual.x = block.vElems.rootVisual.x + travelDist;
						}
						
						if(block.hSettle) {
							ct++;
						}
					}
				}
			}
			
			return ct;
		},
		
		destroy: function() {
			this.removeAllGridBlocks();
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
