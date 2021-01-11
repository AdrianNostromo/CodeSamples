/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	
	"app/main/visualfxhandler/util/DTVisualFXGeneratorTemplate",
	"app/main/visualfxhandler/util/DTVisualFxKf",
	"app/main/visualfxhandler/util/VisualKfFinishEventType"
], function(declare,
	DTVisualFXGeneratorTemplate, DTVisualFxKf, VisualKfFinishEventType
){
	"use strict";
	var cls = declare("app.main.visualfxhandler.util.VisualFxTemplates", null, {});
	
	cls.visualFxPlayTemplates = {
		diamondSpentOnUserInterface: {
			visualFXConfig: {
				delayMS: 0,
				templateID: "diamondSpentOnUserInterface",
				assetIDs: [
					//void
				],
				pauseOnInGamePause: false
			}
		},
		diamondSpentInGame: {
			visualFXConfig: {
				delayMS: 0,
				templateID: "diamondSpentInGame",
				assetIDs: [
					//void
				],
				pauseOnInGamePause: false
			}
		}
	};
	
	cls.particleTemplates = {
		basicFaceBlockParticle: {
			vLayout: [
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
				},
				{
					id:"blockFace",
					visual: {
						type: "sprite",
						assetId: "inGame/blocks/#BLOCK_ID#_blockFace",
						parentID: "rootVisual"
					},
					initParams: [
						{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
						{prop: "width", val: 100}, {prop: "height", val: 100}
					]
				}
			]
		},
		simpleBlockNoFaceParticle: {
			vLayout: [
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
		specialBlockParticle: {
			vLayout: [
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
		diamondSpentParticleOnUserInterface: {
			vLayout: [
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
						assetId: "inGame/blocks/DIAMOND_A",
						parentID: "rootVisual"
					},
					initParams: [
						{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
						{prop:"width", val:{percent:(128 / 640) * 0.58, param:"width", targetID:"_app"}},
						{prop:"height", val:{percent:(128 / 960) * 0.58, param:"height", targetID:"_app"}}
					]
				}
			]
		},
		diamondSpentParticleInGame: {
			vLayout: [
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
						assetId: "inGame/blocks/DIAMOND_A",
						parentID: "rootVisual"
					},
					initParams: [
						{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
						{prop:"width", val:100},
						{prop:"height", val:100}
					]
				}
			]
		},
		splatParticle: {
			vLayout: [
				{
					id:"rootVisual",
					visual: {
						type: "container"
					}
				},
				{
					id:"dropsVisual",
					visual: {
						type: "movieClip",
						assetId: "inGame/visualFX/splat_#BLOCK_VERSION#/splat_###",
						parentID: "rootVisual"
					},
					initParams: [
						{prop: ["anchor", "x"], val: 118 / 230}, {prop: ["anchor", "y"], val: 124 / 318},
						{prop: "width", val: 230}, {prop: "height", val: 318}, 
						{prop: "loop", val: false}, {prop: "animationSpeed", val: 0.4},
						{functionName: "play"}
					]
				}
			]
		},
		boomParticle: {
			vLayout: [
				{
					id:"rootVisual",
					visual: {
						type: "container"
					}
				},
				{
					id:"mainVisual",
					visual: {
						type: "movieClip",
						assetId: "inGame/visualFX/boom/boom_###",
						parentID: "rootVisual"
					},
					initParams: [
						{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
						{prop: "width", val: 317}, {prop: "height", val: 308}, 
						{prop: "loop", val: false}, {prop: "animationSpeed", val: 0.4},
						{functionName: "play"}
					]
				}
			]
		},
		genericStar_large: {
			vLayout: [
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
						assetId: "uiGeneral/star_large",
						parentID: "rootVisual"
					},
					initParams: [
						{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
						{prop: "_widthHeight", val: {percent:150 / 640, param:"width", targetID:"_app"}}
					]
				}
			]
		},
		genericDiamond_large: {
			vLayout: [
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
						assetId: "uiGeneral/diamond_large",
						parentID: "rootVisual"
					},
					initParams: [
						{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
						{prop: "_widthHeight", val: {percent:157 / 640, param:"width", targetID:"_app"}}
					]
				}
			]
		},
		genericDiamond_medium: {
			vLayout: [
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
						assetId: "uiGeneral/diamond_medium",
						parentID: "rootVisual"
					},
					initParams: [
						{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
						{prop: "_widthHeight", val: {percent:98 / 640, param:"width", targetID:"_app"}}
					]
				}
			]
		}
	};
	
	cls.fxTemplates = {
		simpleBlockOutBoardClearing: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "simpleBlockNoFaceParticle",
						visualLayer: ["game", "layers", "vLayer_gridFxMain"],
						igParticlesIFlagIDs: ["iVisualFXComponentActive"],
						initParams: null,
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 300, "rootVisual", "alpha", 1, 0, false, null),
								DTVisualFxKf.newAnimation(0, 300, "rootVisual", ["scale", "x"], 1, 0.4, false, null),
								DTVisualFxKf.newAnimation(0, 300, "rootVisual", ["scale", "y"], 1, 0.4, false, null)
							]
						]
					}
				)
			]
		},
		specialBlockOutBoardClearing: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "specialBlockParticle",
						visualLayer: ["game", "layers", "vLayer_gridFxMain"],
						igParticlesIFlagIDs: ["iVisualFXComponentActive"],
						initParams: null,
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 300, "rootVisual", "alpha", 1, 0, false, null),
								DTVisualFxKf.newAnimation(0, 300, "rootVisual", ["scale", "x"], 1, 0.4, false, null),
								DTVisualFxKf.newAnimation(0, 300, "rootVisual", ["scale", "y"], 1, 0.4, false, null)
							]
						]
					}
				)
			]
		},
		simpleMatchedBlockOut: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "basicFaceBlockParticle",
						visualLayer: ["game", "layers", "vLayer_gridFxMain"],
						igParticlesIFlagIDs: ["iVisualFXComponentActive", "iBoardSettleBlocked"],
						initParams: [
							{objAddress: "blockFace", objChange: {prop: "alpha", val: 0}}
						],
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 200, "blockFace", "alpha", 0, 1, false, null),
								DTVisualFxKf.newAnimation(100, 300, "rootVisual", "alpha", 1, 0, false, null),
								DTVisualFxKf.newAnimation(100, 300, "rootVisual", ["scale", "x"], 1, 0.4, false, null),
								DTVisualFxKf.newAnimation(100, 300, "rootVisual", ["scale", "y"], 1, 0.4, false, null)
							]
						]
					}
				)
			]
		},
		itemBlockCollectedUp: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "specialBlockParticle",
						visualLayer: ["game", "layers", "vLayer_gridFxOver"],
						igParticlesIFlagIDs: ["iVisualFXComponentActive"],
						initParams: null,
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", "y", 0, -350, true, null),
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", "alpha", 1, 0.1, false, null),
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", ["scale", "x"], 1, 1.4, false, null),
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", ["scale", "y"], 1, 1.4, false, null)
							]
						]
					}
				)
			]
		},
		itemBlockCollectedZoom: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "specialBlockParticle",
						visualLayer: ["game", "layers", "vLayer_gridFxOver"],
						igParticlesIFlagIDs: ["iVisualFXComponentActive"],
						initParams: null,
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", "alpha", 1, 0.1, false, null),
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", ["scale", "x"], 1, 1.6, false, null),
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", ["scale", "y"], 1, 1.6, false, null)
							]
						]
					}
				)
			]
		},
		bombBlockExplosion: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "specialBlockParticle",
						visualLayer: ["game", "layers", "vLayer_gridFxMain"],
						igParticlesIFlagIDs: ["iVisualFXComponentActive", "iBoardSettleBlocked"],
						initParams: null,
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", "alpha", 1, 0.1, false, null),
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", ["scale", "x"], 1, 0.4, false, null),
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", ["scale", "y"], 1, 0.4, false, null),
								DTVisualFxKf.newDelay(0, {
									type: VisualKfFinishEventType.CREATE_GENERATOR,
									delayMS: 0,
									generatorTemplate: new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
										{
											particlesPerInterval: 1
										},
										{
											particleTemplateId: "boomParticle",
											visualLayer: ["game", "layers", "vLayer_gridFxOver"],
											igParticlesIFlagIDs: ["iVisualFXComponentActive"],
											initParams: null,
											kfItemGroups: [
												[
													DTVisualFxKf.newValueDetector(0, "mainVisual", "playing", false, null)
												]
											]
										}
									)
								})
							]
						]
					}
				)
			]
		},
		simpleBlockCrushed: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "simpleBlockNoFaceParticle",
						visualLayer: ["game", "layers", "vLayer_gridFxMain"],
						igParticlesIFlagIDs: ["iVisualFXComponentActive", "iBoardSettleBlocked"],
						initParams: null,
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 200, "rootVisual", "alpha", 1, 0.1, false, null),
								DTVisualFxKf.newAnimation(0, 200, "rootVisual", ["scale", "x"], 1, 0.4, false, null),
								DTVisualFxKf.newAnimation(0, 200, "rootVisual", ["scale", "y"], 1, 0.4, false, null),
								DTVisualFxKf.newDelay(0, {
									type: VisualKfFinishEventType.CREATE_GENERATOR,
									delayMS: 0,
									generatorTemplate: new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
										{
											particlesPerInterval: 1
										},
										{
											particleTemplateId: "splatParticle",
											visualLayer: ["game", "layers", "vLayer_gridFxOver"],
											igParticlesIFlagIDs: ["iVisualFXComponentActive"],
											initParams: null,
											kfItemGroups: [
												[
													DTVisualFxKf.newValueDetector(0, "dropsVisual", "playing", false, null)
												]
											]
										}
									)
								})
							]
						]
					}
				)
			]
		},
		cementBlockCrushed: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "specialBlockParticle",
						visualLayer: ["game", "layers", "vLayer_gridFxMain"],
						igParticlesIFlagIDs: ["iVisualFXComponentActive", "iBoardSettleBlocked"],
						initParams: null,
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 200, "rootVisual", "alpha", 1, 0.1, false, null),
								DTVisualFxKf.newAnimation(0, 200, "rootVisual", ["scale", "x"], 1, 0.4, false, null),
								DTVisualFxKf.newAnimation(0, 200, "rootVisual", ["scale", "y"], 1, 0.4, false, null),
								DTVisualFxKf.newDelay(0, {
									type: VisualKfFinishEventType.CREATE_GENERATOR,
									delayMS: 0,
									generatorTemplate: new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
										{
											particlesPerInterval: 1
										},
										{
											particleTemplateId: "splatParticle",
											visualLayer: ["game", "layers", "vLayer_gridFxOver"],
											igParticlesIFlagIDs: ["iVisualFXComponentActive"],
											initParams: null,
											kfItemGroups: [
												[
													DTVisualFxKf.newValueDetector(0, "dropsVisual", "playing", false, null)
												]
											]
										}
									)
								})
							]
						]
					}
				)
			]
		},
		lineDestroyerExplosion: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "specialBlockParticle",
						visualLayer: ["game", "layers", "vLayer_gridFxOver"],
						igParticlesIFlagIDs: ["iVisualFXComponentActive", "iBoardSettleBlocked"],
						initParams: null,
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", "alpha", 1, 0.1, false, null),
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", ["scale", "x"], 1, 1.6, false, null),
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", ["scale", "y"], 1, 1.6, false, null)
							]
						]
					}
				)
			]
		},
		columnDestroyerExplosion: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "specialBlockParticle",
						visualLayer: ["game", "layers", "vLayer_gridFxOver"],
						igParticlesIFlagIDs: ["iVisualFXComponentActive", "iBoardSettleBlocked"],
						initParams: null,
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", "alpha", 1, 0.1, false, null),
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", ["scale", "x"], 1, 1.6, false, null),
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", ["scale", "y"], 1, 1.6, false, null)
							]
						]
					}
				)
			]
		},
		keyBlockCollectedToLock: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "specialBlockParticle",
						visualLayer: ["game", "layers", "vLayer_gridFxOver"],
						igParticlesIFlagIDs: ["iVisualFXComponentActive"],
						initParams: null,
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", "x", 0, {targetID: "_customValues", param: "END_OFFSET_X"}, true, null),
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", "y", 0, {targetID: "_customValues", param: "END_OFFSET_Y"}, true, null),
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", "alpha", 1, 0.1, false, null),
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", ["scale", "x"], 1, 1.4, false, null),
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", ["scale", "y"], 1, 1.4, false, null)
							]
						]
					}
				)
			]
		},
		lockBlockUnlocked: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "specialBlockParticle",
						visualLayer: ["game", "layers", "vLayer_gridFxOver"],
						igParticlesIFlagIDs: ["iVisualFXComponentActive", "iBoardSettleVelocityChangeBlocked"],
						initParams: null,
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", "alpha", 1, 0.1, false, null),
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", ["scale", "x"], 1, 1.6, false, null),
								DTVisualFxKf.newAnimation(0, 400, "rootVisual", ["scale", "y"], 1, 1.6, false, null)
							]
						]
					}
				)
			]
		},
		simpleBlockColorPainting: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "simpleBlockNoFaceParticle",
						visualLayer: ["game", "layers", "vLayer_gridFxMain"],
						igParticlesIFlagIDs: ["iVisualFXComponentActive"],
						initParams: null,
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(100, 300, "rootVisual", "alpha", 1, 0, false, null),
								DTVisualFxKf.newAnimation(100, 300, "rootVisual", ["scale", "x"], 1, 0.3, false, null),
								DTVisualFxKf.newAnimation(100, 300, "rootVisual", ["scale", "y"], 1, 0.3, false, null)
							]
						]
					}
				)
			]
		},
		diamondSpentOnUserInterface: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "diamondSpentParticleOnUserInterface",
						visualLayer: ["game", "layers", "vLayer_interfaceFx"],
						igParticlesIFlagIDs: ["iVisualFXComponentActive"],
						initParams: null,
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 600, "rootVisual", "y", 0, {percent:-240 / 960, param:"height", targetID:"_app"}, true, null),
								DTVisualFxKf.newAnimation(300, 300, "rootVisual", "alpha", 1, 0.1, false, null),
								DTVisualFxKf.newAnimation(0, 600, "rootVisual", ["scale", "x"], 0.8, 1.4, false, null),
								DTVisualFxKf.newAnimation(0, 600, "rootVisual", ["scale", "y"], 0.8, 1.4, false, null)
							]
						]
					}
				)
			]
		},
		diamondSpentInGame: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, ["iVisualFXComponentActive"], 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "diamondSpentParticleInGame",
						visualLayer: ["game", "layers", "vLayer_gridFxOver"],
						igParticlesIFlagIDs: ["iVisualFXComponentActive"],
						initParams: null,
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 600, "rootVisual", "y", 0, -120, true, null),
								DTVisualFxKf.newAnimation(300, 300, "rootVisual", "alpha", 1, 0.1, false, null),
								DTVisualFxKf.newAnimation(0, 600, "rootVisual", ["scale", "x"], 0.5, 1, false, null),
								DTVisualFxKf.newAnimation(0, 600, "rootVisual", ["scale", "y"], 0.5, 1, false, null)
							]
						]
					}
				)
			]
		},
		viewLevelClear_collectedStarToSlot: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, null, 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "genericStar_large",
						visualLayer: ["layers", "visualFx"],
						igParticlesIFlagIDs: null,
						isTracked: true,
						initParams: [
							{objAddress: "blockVisual", objChange: {prop: "_widthHeight", val: {percent:150 / 640, param:"width", targetID:"_app"}}},
							{objAddress: "rootVisual", objChange: {prop: "rotation", val: {targetID: "_customValues", param: "STAR_ROTATION"}}}
						],
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", "x", 0, {targetID: "_customValues", param: "END_OFFSET_X"}, true, null),
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", "y", 0, {targetID: "_customValues", param: "END_OFFSET_Y"}, true, null),
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", "alpha", 0, 1, false, null),
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", ["scale", "x"], 0.4, 1, false, null),
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", ["scale", "y"], 0.4, 1, false, null)
							]
						]
					}
				)
			]
		},
		viewLevelClear_smallStarTransfer: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, null, 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "genericStar_large",
						visualLayer: ["layers", "visualFx"],
						igParticlesIFlagIDs: null,
						isTracked: true,
						initParams: [
							{objAddress: "blockVisual", objChange: {prop: "_widthHeight", val: {percent:150 / 640, param:"width", targetID:"_app"}}},
							{objAddress: "rootVisual", objChange: {prop: "rotation", val: {targetID: "_customValues", param: "STAR_ROTATION"}}}
						],
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", "x", 0, {targetID: "_customValues", param: "END_OFFSET_X"}, true, null),
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", "y", 0, {targetID: "_customValues", param: "END_OFFSET_Y"}, true, null),
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", "alpha", 0, 1, false, null),
								DTVisualFxKf.newAnimation(0, 450, "blockVisual", "width", {percent:150 / 640, param:"width", targetID:"_app"}, {percent:80 / 640, param:"width", targetID:"_app"}, false, null),
								DTVisualFxKf.newAnimation(0, 450, "blockVisual", "height", {percent:140 / 640, param:"width", targetID:"_app"}, {percent:75 / 640, param:"width", targetID:"_app"}, false, null)
							]
						]
					}
				)
			]
		},
		viewLevelClear_smallDiamondTransfer: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, null, 
					{
						particlesPerInterval: 1
					},
					{
						particleTemplateId: "genericDiamond_large",
						visualLayer: ["layers", "visualFx"],
						igParticlesIFlagIDs: null,
						isTracked: true,
						initParams: [
							{objAddress: "blockVisual", objChange: {prop: "_widthHeight", val: {percent:150 / 640, param:"width", targetID:"_app"}}},
							{objAddress: "rootVisual", objChange: {prop: "rotation", val: {targetID: "_customValues", param: "STAR_ROTATION"}}}
						],
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", "x", 0, {targetID: "_customValues", param: "END_OFFSET_X"}, true, null),
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", "y", 0, {targetID: "_customValues", param: "END_OFFSET_Y"}, true, null),
								DTVisualFxKf.newAnimation(0, 450, "rootVisual", "alpha", 0, 1, false, null),
								DTVisualFxKf.newAnimation(0, 450, "blockVisual", "width", {percent:150 / 640, param:"width", targetID:"_app"}, {percent:80 / 640, param:"width", targetID:"_app"}, false, null),
								DTVisualFxKf.newAnimation(0, 450, "blockVisual", "height", {percent:150 / 640, param:"width", targetID:"_app"}, {percent:80 / 640, param:"width", targetID:"_app"}, false, null)
							]
						]
					}
				)
			]
		},
		viewShop_bonusCollected: {
			generators: [
				new DTVisualFXGeneratorTemplate(0, null, 
					{
						particlesPerInterval: 6
					},
					{
						particleTemplateId: "genericDiamond_medium",
						visualLayer: ["layers", "visualFx"],
						igParticlesIFlagIDs: null,
						isTracked: true,
						initParams: null,
						kfItemGroups: [
							[
								DTVisualFxKf.newAnimation(200, 400, "rootVisual", "alpha", 1, 0.1, false, null, {
									easeFunctioName: "easeOutCirc"
								}),
								DTVisualFxKf.newAnimation(0, 600, "rootVisual", ["scale", "x"], 0.2, 0.9, false, null, {
									easeFunctioName: "easeOutCirc"
								}),
								DTVisualFxKf.newAnimation(0, 600, "rootVisual", ["scale", "y"], 0.2, 0.9, false, null, {
									easeFunctioName: "easeOutCirc"
								}),
								DTVisualFxKf.newAnimation(0, 600, null, ["vElems", "rootVisual", "x"], 0, {multiply: [
									{percent:150 / 960, param:"width", targetID:"_app"},
									{multiply: [
										{value:(Math.PI * 2) / 6},
										{percent:1, param:["data", "intervalParticleIndice"], targetID:"_self"}
									], mathOperation: "cos"}
								]}, true, null, {
									isLogicKf: true,
									easeFunctioName: "easeOutCirc"
								}),
								DTVisualFxKf.newAnimation(0, 600, null, ["vElems", "rootVisual", "y"], 0, {multiply: [
									{percent:150 / 960, param:"width", targetID:"_app"},
									{multiply: [
										{value:(Math.PI * 2) / 6},
										{percent:1, param:["data", "intervalParticleIndice"], targetID:"_self"}
									], mathOperation: "sin"}
								]}, true, null, {
									isLogicKf: true,
									easeFunctioName: "easeOutCirc"
								})
							]
						]
					}
				)
			]
		}
	};
	
	return cls;
	
});
