/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/main/view/util/ViewType",
	"app/util/SafeNumber",
	"app/util/VisualUtil",
	"app/util/MathUtil"
], function(declare, ModuleBase, 
		ViewType, SafeNumber, VisualUtil, MathUtil
	){
	"use strict";
	return declare("app.main.view.ViewLevelClear", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"initializeView", cbName:"onInitializeView", isUnique:false},
			{channel:"appLoopUserInterface", cbName:"onAppLoopUserInterface", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp",  listenerType:"unique", mustExist:true},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false},
			activateInGame:{channel:"activateInGame", listenerType:"list", mustExist:false}
		},
		
		app: null,
		
		viewID: ViewType.LEVEL_CLEAR,
		viewCompound: null,
		
		extraData: null,
		
		data: null,
		animation: null,
		
		vElems: null,
		
		animationPhase: {
			VIEW_IN_WAITING: "VIEW_IN_WAITING",
			STARS_IN: "STARS_IN",
			TRANSFER_POINTS: "TRANSFER_POINTS",
			FINISHED: "FINISHED"
		},
		
		cfg: {
			iconsFadeInSpeedS: 2,
			multiplierGrowDelayMS: 450,
			stashStarItemTransferDelayMS: 300,
			starRotations: [
				-15 * (Math.PI/180),
				0 * (Math.PI/180),
				15 * (Math.PI/180)
			]
		},
		
		viewConfig: {
		},
		
		viewLayout: [
			{
				id:"container",
				visual: {
					type: "container"
				},
				params:[
					{prop:"y", val:{hasStates: true,
						sIn:{percent:0 / 960, param:"height", targetID:"_app"},
						sInEaseName: "easeOutSine",
						sOut:{percent:960 / 960, param:"height", targetID:"_app"},
						sOutEaseName: "easeOutSine"
					}}
				]
			},
			{
				id:"titlePanel",
				list:[
					{
						id:"container",
						visual: {
							type: "container",
							parentID: ["_parent", "container"]
						},
						params:[
							{prop:"y", val:{percent:30 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"bg",
						visual: {
							type: "sprite",
							assetId: "uiGeneral/plank2X",
							parentID: "container"
						},
						params:[
							{prop:"width", val:{percent:640 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:212 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"starSlot0",
						visual: {
							type: "sprite",
							assetId: "viewLevelClear/starSlot",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
							{prop: "rotation", val: -15 * (Math.PI/180)}
						],
						params:[
							{prop:"_widthHeight", val:{percent:150 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:145 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:114 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"starSlot1",
						visual: {
							type: "sprite",
							assetId: "viewLevelClear/starSlot",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
							{prop: "rotation", val: 0 * (Math.PI/180)}
						],
						params:[
							{prop:"_widthHeight", val:{percent:150 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:320 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:95 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"starSlot2",
						visual: {
							type: "sprite",
							assetId: "viewLevelClear/starSlot",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
							{prop: "rotation", val: 15 * (Math.PI/180)}
						],
						params:[
							{prop:"_widthHeight", val:{percent:150 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:495 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:114 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"star0",
						visual: {
							type: "sprite",
							assetId: "uiGeneral/star_large",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
							{prop: "rotation", val: -15 * (Math.PI/180)}
						],
						params:[
							{prop:"_widthHeight", val:{percent:150 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:145 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:114 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"star1",
						visual: {
							type: "sprite",
							assetId: "uiGeneral/star_large",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
							{prop: "rotation", val: 0 * (Math.PI/180)}
						],
						params:[
							{prop:"_widthHeight", val:{percent:150 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:320 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:95 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"star2",
						visual: {
							type: "sprite",
							assetId: "uiGeneral/star_large",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
							{prop: "rotation", val: 15 * (Math.PI/180)}
						],
						params:[
							{prop:"_widthHeight", val:{percent:150 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:495 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:114 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"subtitlePanel",
						list:[
							{
								id:"container",
								visual: {
									type: "container",
									parentID: ["_parent", "container"]
								},
								params:[
									{prop:"x", val:{percent:320 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:251 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"bg",
								visual: {
									type: "sprite",
									assetId: "uiGeneral/subtitleBg",
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5}
								],
								params:[
									{prop:"_widthHeight", val:{percent:504 / 640, param:"width", targetID:"_app"}}
								]
							},
							{
								id:"flblLevel",
								visual: {
									type: "text",
									extraData: {
										text: "Level",
										style: {
											font: "800 77px 'WFDosis'",
											fill: "#FFFFFF",
											align: "right"
										}
									},
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 1}, {prop: ["anchor", "y"], val: 0.5}
								],
								_refreshTextStyle: true,
								params:[
									{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:77 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
									{prop:"x", val:{percent:34 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:0 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"lblLevelIndice",
								visual: {
									type: "text",
									extraData: {
										text: "00",
										style: {
											font: "800 77px 'WFDosis'",
											fill: "#A3DA1C",
											align: "left"
										}
									},
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 0}, {prop: ["anchor", "y"], val: 0.5}
								],
								_refreshTextStyle: true,
								params:[
									{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:77 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
									{prop:"x", val:{percent:46 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:0 / 960, param:"height", targetID:"_app"}}
								]
							}
						]
					}
				]
			},
			{
				id:"levelBonusPanel",
				list:[
					{
						id:"container",
						visual: {
							type: "container",
							parentID: ["_parent", "container"]
						},
						params:[
							{prop:"y", val:{percent:367 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"bg",
						visual: {
							type: "sprite",
							assetId: "uiGeneral/plank1X",
							parentID: "container"
						},
						params:[
							{prop:"width", val:{percent:640 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"flblLevelBonus",
						visual: {
							type: "text",
							extraData: {
								text: "Bonus",
								style: {
									font: "800 90px 'WFOpenSans'",
									fill: "#BE8642",
									align: "right",
									stroke: "#3A2914",
									strokeThickness: 8
								}
							},
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 1}, {prop: ["anchor", "y"], val: 0.5}
						],
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:90 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFOpenSans'"}]}},
							{prop:["style", "strokeThickness"], val:{percent:8 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:390 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:56 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"flblLevelMultiplierSymbol",
						visual: {
							type: "text",
							extraData: {
								text: "x",
								style: {
									font: "800 88px 'WFDosis'",
									fill: "#FFFFFF",
									align: "center"
								}
							},
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5}
						],
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:88 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
							{prop:"x", val:{percent:424 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:50 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"lblStarsMuliplier",
						visual: {
							type: "text",
							extraData: {
								text: "00",
								style: {
									font: "800 88px 'WFDosis'",
									fill: "#A3DA1C",
									align: "left"
								}
							},
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0}, {prop: ["anchor", "y"], val: 0.5}
						],
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:88 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
							{prop:"x", val:{percent:451 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:52 / 960, param:"height", targetID:"_app"}}
						]
					}
				]
			},
			{
				id:"infoPanel",
				list:[
					{
						id:"container",
						visual: {
							type: "container",
							parentID: ["_parent", "container"]
						},
						params:[
							{prop:"y", val:{percent:473 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"bg",
						visual: {
							type: "sprite",
							assetId: "uiGeneral/plank2X",
							parentID: "container"
						},
						params:[
							{prop:"width", val:{percent:640 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:212 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"iconStars",
						visual: {
							type: "sprite",
							assetId: "uiGeneral/star_medium",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5}
						],
						params:[
							{prop:"_widthHeight", val:{percent:80 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:128 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:61 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"iconDiamonds",
						visual: {
							type: "sprite",
							assetId: "uiGeneral/diamond_medium",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5}
						],
						params:[
							{prop:"_widthHeight", val:{percent:80 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:128 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:155 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"lblCurrentStars",
						visual: {
							type: "text",
							extraData: {
								text: "0000",
								style: {
									font: "800 88px 'WFDosis'",
									fill: "#FFFFFF",
									align: "left"
								}
							},
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0}, {prop: ["anchor", "y"], val: 0.5}
						],
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:88 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
							{prop:"x", val:{percent:190 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:61 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"lblCurrentDiamonds",
						visual: {
							type: "text",
							extraData: {
								text: "0000",
								style: {
									font: "800 88px 'WFDosis'",
									fill: "#FFFFFF",
									align: "left"
								}
							},
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0}, {prop: ["anchor", "y"], val: 0.5}
						],
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:88 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
							{prop:"x", val:{percent:190 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:155 / 960, param:"height", targetID:"_app"}}
						]
					}
				]
			},
			{
				id:"actionPanel",
				list:[
					{
						id:"container",
						visual: {
							type: "container",
							parentID: ["_parent", "container"]
						},
						params:[
							{prop:"y", val:{percent:717 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"bg",
						visual: {
							type: "sprite",
							assetId: "uiGeneral/plank2X",
							parentID: "container"
						},
						params:[
							{prop:"width", val:{percent:640 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:212 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"btnHome",
						visual: {
							type: "spriteButton",
							assetId: "viewLevelClear/btnHome_",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
							{prop: "cbcClick", val:{param:"CBC_CLICK", targetID:"_customValues"}}
						],
						params:[
							{prop:"_widthHeight", val:{percent:211 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:203 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:106 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"btnNextLevel",
						visual: {
							type: "spriteButton",
							assetId: "viewLevelClear/btnNextLevel_",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
							{prop: "cbcClick", val:{param:"CBC_CLICK", targetID:"_customValues"}}
						],
						params:[
							{prop:"_widthHeight", val:{percent:211 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:437 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:106 / 960, param:"height", targetID:"_app"}}
						]
					},
				]
			}
		],
		
		constructor: function() {
			//void
		},
		
		onInitializeView: function(viewID, ret) {
			if(viewID === this.viewID) {
				this.createVisuals();
				
				ret.viewCompound = this.viewCompound = {
					config: this.viewConfig,
					layout: this.viewLayout,
					vElems: this.vElems,
					parentLayer: this.app.layers.views,
					dependantViews: [
						ViewType.BACKGROUND,
						ViewType.SIDE_BUSHES
					],
					cbcViewLayoutUpdateFinished: null,
					cbcViewActivated: {callback: this.onViewActivated, context: this}
				};
			}
		},
		
		createVisuals:function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.vElems = {isVContainer: true};
			
			VisualUtil.createVLayoutVisuals(this.vElems, this.viewLayout, this.app, this.moduleBranch, null, null, {CBC_CLICK: {callback: this.onBtnClick, context: this}});
		},
		
		onViewActivated: function(extraData) {
			this.extraData = extraData;
			
			this.updateData(extraData);
		},
		
		onAppLoopUserInterface:function(appTime, deltaMS, deltaS) {
			if(!this.animation) {
				return ;
			}
			
			if(this.animation.phase == this.animationPhase.VIEW_IN_WAITING) {
				if(this.viewCompound && this.viewCompound.state.stateEnd.indexOf("sIn") == 0 && this.viewCompound.state.completionPercent >= 1) {
					// Initialize the animation steps.
					this.activatePhase_STARS_IN();
				}
			}else if(this.animation.phase == this.animationPhase.STARS_IN) {
				//void
			}else if(this.animation.phase == this.animationPhase.TRANSFER_POINTS) {
				//void
			}else if(this.animation.phase == this.animationPhase.FINISHED) {
				//void
			}else {
				this.postError("Err 6.");
				
				return null;
			}
		},
		
		updateData: function(extraData) {
			var levelIndice = extraData.snLevelIndice.getValue();
			var levelStarsCollected = extraData.snLevelStarsCollected.getValue();
			
			var rewardStars = levelStarsCollected * levelIndice;
			var rewardDiamonds = levelStarsCollected * 2;
			
			this.data = {
				levelID: extraData.levelID,
				levelIndice: levelIndice,
				levelStarsCollected: levelStarsCollected,
				levelMultiplier: levelIndice,
				rewardStars: rewardStars,
				rewardDiamonds: rewardDiamonds,
				infoLblStarsCount: this.app.profileData.getStars(),
				infoLblDiamondsCount: this.app.profileData.getDiamonds()
			};
			
			// Set the stars and diamonds labels.
			this.vElems.titlePanel.subtitlePanel.lblLevelIndice.text = this.data.levelMultiplier;
			this.vElems.levelBonusPanel.lblStarsMuliplier.text = this.data.levelMultiplier;
			
			this.vElems.infoPanel.lblCurrentStars.text = this.data.infoLblStarsCount;
			this.vElems.infoPanel.lblCurrentDiamonds.text = this.data.infoLblDiamondsCount;
			
			// Hide all stars;
			this.vElems.titlePanel.star0.visible = false;
			this.vElems.titlePanel.star1.visible = false;
			this.vElems.titlePanel.star2.visible = false;
			
			// Give the bonus to the player (stars and diamonds).
			this.app.profileData.addStars(rewardStars);
			this.app.profileData.addDiamonds(rewardDiamonds);
			
			this.animation = {
				phase: this.animationPhase.VIEW_IN_WAITING
			};
		},
		
		activatePhase_STARS_IN: function() {
			this.animation = {
				phase: this.animationPhase.STARS_IN,
				animatingStarsCount: 0
			};
			
			// Create the fx and activate.
			var starFxInitialOffsets = [
				{x: -20, y: -70},
				{x: 0, y: -80},
				{x: 20, y: -70}
			];
			for(var i=0;i<this.data.levelStarsCollected;i++) {
				var visualEntry = this.vElems.titlePanel["star" + i];
				
				this.animation.animatingStarsCount++;
				
				var fxPos = {
					x: this.vElems.titlePanel.container.x + visualEntry.x + starFxInitialOffsets[i].x,
					y: this.vElems.titlePanel.container.y + visualEntry.y + starFxInitialOffsets[i].y
				};
				
				var extraData = {
					customValues: {
						STAR_ROTATION: this.cfg.starRotations[i],
						END_OFFSET_X: -starFxInitialOffsets[i].x, 
						END_OFFSET_Y: -starFxInitialOffsets[i].y
					},
					cbcTrackedParticleLifeEnd: {callback: this.onStarVisualFxTrackedParticleLifeEnd, context: this},
					userData: {
						starIndice: i
					},
					parentLayer: this.vElems.container
				};
				
				this.app.visualFx.activateFX("viewLevelClear_collectedStarToSlot", 400 * i, fxPos, null, this.moduleBranch, extraData, false, this.viewID);
			}
		},
		
		onStarVisualFxTrackedParticleLifeEnd: function(visualFxParticle) {
			this.vElems.titlePanel["star" + visualFxParticle.extraData.userData.starIndice].visible = true;
			
			this.animation.animatingStarsCount--;
			
			if(this.animation.animatingStarsCount === 0) {
				this.activatePhase_TRANSFER_POINTS();
			}
		},
		
		activatePhase_TRANSFER_POINTS: function() {
			this.animation = {
				phase: this.animationPhase.TRANSFER_POINTS,
				activeTransferPoints: 0
			};
			
			var itemsList = [];
			for(var i=0;i<this.data.rewardStars;i++) {
				itemsList.push("STAR");
			}
			for(var i=0;i<this.data.rewardDiamonds;i++) {
				itemsList.push("DIAMOND");
			}
			
			// Shuffle the list.
			MathUtil.shuffleFisherYates(itemsList);
			
			var interPointFxDelay = 250 - Math.min(100, (this.data.rewardStars + this.data.rewardDiamonds) * 3);
			
			var sourceStarIndice = 0;
			for(var i=0;i<itemsList.length;i++) {
				var entry = itemsList[i];
				
				this.animation.activeTransferPoints++;
				
				var visualEntry = this.vElems.titlePanel["star" + sourceStarIndice];
				var fxPos = {
					x: this.vElems.titlePanel.container.x + visualEntry.x,
					y: this.vElems.titlePanel.container.y + visualEntry.y
				};
				
				sourceStarIndice++;
				if(sourceStarIndice >= this.data.levelStarsCollected) {
					sourceStarIndice = 0;
				}
				
				var targetPos;
				
				var visualFxId;
				if(entry === "STAR") {
					visualFxId = "viewLevelClear_smallStarTransfer";
					
					targetPos = {
						x: this.vElems.infoPanel.container.x + this.vElems.infoPanel.iconStars.x,
						y: this.vElems.infoPanel.container.y + this.vElems.infoPanel.iconStars.y
					};
				}else if(entry === "DIAMOND") {
					visualFxId = "viewLevelClear_smallDiamondTransfer";
					
					targetPos = {
						x: this.vElems.infoPanel.container.x + this.vElems.infoPanel.iconDiamonds.x,
						y: this.vElems.infoPanel.container.y + this.vElems.infoPanel.iconDiamonds.y
					};
				}else {
					this.postError("Err 6.");
					
					return null;
				}
				
				var extraData = {
					customValues: {
						STAR_ROTATION: this.cfg.starRotations[i],
						END_OFFSET_X: targetPos.x - fxPos.x,
						END_OFFSET_Y: targetPos.y - fxPos.y
					},
					cbcTrackedParticleLifeEnd: {callback: this.onSmallPointsTransferVisualFxTrackedParticleLifeEnd, context: this},
					userData: {
						pointType: entry
					},
					parentLayer: this.vElems.container
				};
				
				this.app.visualFx.activateFX(visualFxId, 250 * i, fxPos, null, this.moduleBranch, extraData, false, this.viewID);
			}
		},
		
		onSmallPointsTransferVisualFxTrackedParticleLifeEnd: function(visualFxParticle) {
			if(visualFxParticle.extraData.userData.pointType === "STAR") {
				this.data.infoLblStarsCount++;
				
				this.vElems.infoPanel.lblCurrentStars.text = this.data.infoLblStarsCount;
			}else if(visualFxParticle.extraData.userData.pointType === "DIAMOND") {
				this.data.infoLblDiamondsCount++;
				
				this.vElems.infoPanel.lblCurrentDiamonds.text = this.data.infoLblDiamondsCount;
			}else {
				this.postError("Err 6.");
				
				return null;
			}
			
			this.animation.activeTransferPoints--;
			
			if(this.animation.activeTransferPoints === 0) {
				this.activatePhase_FINISHED();
			}
		},
		
		activatePhase_FINISHED: function() {
			this.animation = {
				phase: this.animationPhase.FINISHED
			};
		},
		
		onBtnClick:function(event) {
			if(!this.viewCompound || this.viewCompound.state.stateEnd.indexOf("sIn") != 0) {
				return ;
			}
			
			if(event.target === this.vElems.actionPanel.btnHome) {
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.LEVEL_CLEAR, "sOut", true, null, 0, null, {
					viewID: ViewType.MAIN,
					stateEnd: "sIn",
					doTransition: true,
					extraData: null,
					delayMS: 0,
					cbcAnimationFinish: null,
					finishCommand: null
				});
			}else if(event.target === this.vElems.actionPanel.btnNextLevel) {
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.LEVEL_CLEAR, "sOut", true, null, 0, null, {
					viewID: ViewType.IN_GAME,
					stateEnd: "sIn",
					doTransition: false,
					extraData: {
						levelID: this.extraData.levelID,
						snLevelIndice: new SafeNumber(this.extraData.snLevelIndice.getValue() + 1)
					},
					delayMS: 0,
					cbcAnimationFinish: null,
					finishCommand: null
				});
			}else {
				this.postError("Err 6.");
				
				return null;
			}
		},
		
		destroy: function() {
			if(this.vElems) {
				this.app.cache.releaseVElems(this.moduleBranch, this.vElems);
				
				this.vElems = null;
			}
			
			this.__proto__.__proto__.destroy.call(this);
		}

	});
});
