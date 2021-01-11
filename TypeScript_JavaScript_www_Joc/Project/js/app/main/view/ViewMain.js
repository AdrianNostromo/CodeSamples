/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/main/view/util/ViewType",
	"app/util/TimeUltil",
	"lib/pixijs/pixi",
	"app/util/VisualUtil",
	"app/util/tween/EaseFunctions",
	
	"app/ingamea/util/LevelID",
	"app/util/SafeNumber",
	"app/main/util/PopupMessageType"
], function(declare, ModuleBase, 
		ViewType, TimeUltil, PIXI, VisualUtil, EaseFunctions,
		LevelID, SafeNumber, PopupMessageType
	){
	"use strict";
	return declare("app.main.view.ViewMain", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"initializeView", cbName:"onInitializeView", isUnique:false},
			{channel:"profileDataChange", cbName:"onProfileDataChange", isUnique:false},
			{channel:"audioUnmuteStateChanged", cbName:"onAudioUnmuteStateChanged", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp",  listenerType:"unique", mustExist:true},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false},
			activateInGame:{channel:"activateInGame", listenerType:"list", mustExist:false},
			toggleAudioUnmuteState:{channel:"toggleAudioUnmuteState", listenerType:"any", mustExist:false}
		},
		
		app: null,
		
		viewID: ViewType.MAIN,
		viewCompound: null,
		
		vElems: null,
		
		cfg: {
		},
		
		viewConfig: {
		},
		
		viewLayout: [
			{
				id:"container",
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
						params:[
							{prop:"y", val:{percent:87 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"bg",
						params:[
							{prop:"width", val:{percent:640 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:212 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"iconStars",
						params:[
							{prop:"_widthHeight", val:{percent:58 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:107 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:-45 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"lblStars",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:61 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
							{prop:["style", "strokeThickness"], val:{percent:8 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:150 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:-45 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"subtitlePanel",
						list:[
							{
								id:"container",
								params:[
									{prop:"x", val:{percent:320 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:208 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"bg",
								params:[
									{prop:"_widthHeight", val:{percent:504 / 640, param:"width", targetID:"_app"}}
								]
							},
							{
								id:"iconDiamonds",
								params:[
									{prop:"_widthHeight", val:{percent:98 / 640, param:"width", targetID:"_app"}},
									{prop:"x", val:{percent:-101 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:2 / 960, param:"height", targetID:"_app"}}
								]
							},
							// ASD_002;// add templates for elements as: subtitlePanel_lblDiamonds, for easy update.
							// ASD_001;// allow for local params objects that are applied after the template ones.
							/*
							{
								templateId:"subtitlePanel",
								_refreshTextStyle: true,
								initParams: [
								]
								params:[
								]
							}
							*/
							
							{
								id:"lblDiamonds",
								_refreshTextStyle: true,
								params:[
									{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:77 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
									{prop:"x", val:{percent:49 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:0 / 960, param:"height", targetID:"_app"}}
								]
							}
						]
					},
					{
						id:"imgTitle",
						params:[
							{prop:"width", val:{percent:520 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:133 / 960, param:"height", targetID:"_app"}},
							{prop:"x", val:{percent:323 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:81 / 960, param:"height", targetID:"_app"}}
						]
					}
				]
			},
			{
				id:"infoPanel",
				list:[
					{
						id:"container",
						params:[
							{prop:"y", val:{percent:435 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"bg",
						params:[
							{prop:"width", val:{percent:640 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"lblGamesPlayedGroup",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:40 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
							{prop:["style", "strokeThickness"], val:{percent:8 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:213 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"lblGamesPlayed",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:56 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
							{prop:"x", val:{percent:210 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"lblMaxLevelGroup",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:40 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
							{prop:["style", "strokeThickness"], val:{percent:8 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:498 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"lblMaxLevel",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:56 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
							{prop:"x", val:{percent:495 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					}
				]
			},
			{
				id:"playPanel",
				list:[
					{
						id:"container",
						params:[
							{prop:"y", val:{percent:563 / 960, param:"height", targetID:"_app"}}
						],
					},
					{
						id:"bg",
						params:[
							{prop:"width", val:{percent:640 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:212 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"btnPlay",
						params:[
							{prop:"_widthHeight", val:{percent:211 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:320 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:106 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"btnRateGame",
						params:[
							{prop:"_widthHeight", val:{percent:80 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:134 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:106 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"btnAchievements",
						params:[
							{prop:"_widthHeight", val:{percent:80 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:506 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:106 / 960, param:"height", targetID:"_app"}}
						]
					}
				]
			},
			{
				id:"extrasPanel",
				list:[
					{
						id:"container",
						params:[
							{prop:"y", val:{percent:861 / 960, param:"height", targetID:"_app"}}
						],
					},
					{
						id:"btnShop",
						params:[
							{prop:"_widthHeight", val:{percent:80 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:134 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:0 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"btnShareGame",
						params:[
							{prop:"_widthHeight", val:{percent:80 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:258 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:0 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"btnLeaderboard",
						params:[
							{prop:"_widthHeight", val:{percent:80 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:382 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:0 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"tbtnSound",
						params:[
							{prop:"_widthHeight", val:{percent:80 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:506 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:0 / 960, param:"height", targetID:"_app"}}
						]
					}
				]
			}
		],
		
		constructor: function() {
			//void
		},
		
		onInitializeView: function(viewID, ret) {
			if(viewID === this.viewID) {
				this.createVisuals();
				
				this.viewCompound = {
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
				
				ret.viewCompound = this.viewCompound;
			}
		},
		
		createVisuals:function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.vElems = {};
			
			this.createVisuals_base(this.vElems);
			this.createVisuals_titlePanel(this.vElems);
			this.createVisuals_infoPanel(this.vElems);
			this.createVisuals_playPanel(this.vElems);
			this.createVisuals_extrasPanel(this.vElems);
		},
		
		onViewActivated: function(extraData) {
			this.updateViewData(extraData);
		},
		
		createVisuals_base:function(parentVElems) {
			var vElemsLocal = parentVElems;
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
		},
		
		createVisuals_titlePanel:function(parentVElems) {
			var vElemsLocal = parentVElems.titlePanel = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			vElemsLocal.bg = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plank2X");
			vElemsLocal.bg.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bg);
			
			this.createVisuals_titlePanel_subtitlePanel(parentVElems.titlePanel);
			
			vElemsLocal.iconStars = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/star_medium");
			vElemsLocal.iconStars.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.iconStars);
			
			vElemsLocal.lblStars = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "0000",
				style: {
					font: "800 61px 'WFDosis'",
					fill: "#FFFFFF",
					align: "left",
					stroke: "#3A2914",
					strokeThickness: 8
				}
			});
			vElemsLocal.lblStars.anchor.set(0, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblStars);
			
			vElemsLocal.imgTitle = this.app.popAsset(this.moduleBranch, "sprite", "viewMain/title");
			vElemsLocal.imgTitle.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.imgTitle);
		},
		
		createVisuals_titlePanel_subtitlePanel: function(parentVElems) {
			var vElemsLocal = parentVElems.subtitlePanel = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			vElemsLocal.bg = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/subtitleBg");
			vElemsLocal.bg.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.bg);
			
			vElemsLocal.iconDiamonds = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/diamond_medium");
			vElemsLocal.iconDiamonds.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.iconDiamonds);
			
			vElemsLocal.lblDiamonds = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "0000",
				style: {
					font: "800 77px 'WFDosis'",
					fill: "#FFFFFF",
					align: "center"
				}
			});
			vElemsLocal.lblDiamonds.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblDiamonds);
		},
		
		updateViewData: function(extraData) {
			this.updateGeneralData();
		},
		
		updateGeneralData: function() {
			this.vElems.titlePanel.lblStars.text = this.app.profileData.getStars();
			this.vElems.titlePanel.subtitlePanel.lblDiamonds.text = this.app.profileData.getDiamonds();
			this.vElems.infoPanel.lblGamesPlayed.text = this.app.profileData.getGamesPlayed();
			this.vElems.infoPanel.lblMaxLevel.text = this.app.profileData.getMaxLevel();
			
			this.updateAudioButtonState();
		},
		
		onAudioUnmuteStateChanged: function(audioUnmuteState) {
			if(!this.app || (!this.viewCompound || this.viewCompound.state.stateEnd.indexOf("sIn") != 0)) {
				return ;
			}
			
			this.updateAudioButtonState();
		},
		
		updateAudioButtonState: function() {
			if(!this.app.audio.isUnmuted) {
				this.vElems.extrasPanel.tbtnSound.setIsSelected(false);
			}else {
				this.vElems.extrasPanel.tbtnSound.setIsSelected(true);
			}
		},
		
		onProfileDataChange: function() {
			if(!this.app || (!this.viewCompound || this.viewCompound.state.stateEnd.indexOf("sIn") != 0)) {
				return ;
			}
			
			this.updateGeneralData();
		},
		
		createVisuals_infoPanel:function(parentVElems) {
			var vElemsLocal = parentVElems.infoPanel = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			vElemsLocal.bg = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plank1X");
			vElemsLocal.bg.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bg);
			
			vElemsLocal.lblGamesPlayedGroup = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "Games:",
				style: {
					font: "800 40px 'WFDosis'",
					fill: "#BE8642",
					align: "right",
					stroke: "#3A2914",
					strokeThickness: 8
				}
			});
			vElemsLocal.lblGamesPlayedGroup.anchor.set(1, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblGamesPlayedGroup);
			
			vElemsLocal.lblGamesPlayed = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "00",
				style: {
					font: "800 56px 'WFDosis'",
					fill: "#FFFFFF",
					align: "center"
				}
			});
			vElemsLocal.lblGamesPlayed.anchor.set(0, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblGamesPlayed);
			
			vElemsLocal.lblMaxLevelGroup = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "Max lvl:",
				style: {
					font: "800 40px 'WFDosis'",
					fill: "#BE8642",
					align: "right",
					stroke: "#3A2914",
					strokeThickness: 8
				}
			});
			vElemsLocal.lblMaxLevelGroup.anchor.set(1, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblMaxLevelGroup);
			
			vElemsLocal.lblMaxLevel = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "00",
				style: {
					font: "800 56px 'WFDosis'",
					fill: "#FFFFFF",
					align: "center"
				}
			});
			vElemsLocal.lblMaxLevel.anchor.set(0, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblMaxLevel);
		},
		
		createVisuals_playPanel:function(parentVElems) {
			var vElemsLocal = parentVElems.playPanel = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			vElemsLocal.bg = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plank2X");
			vElemsLocal.bg.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bg);
			
			vElemsLocal.btnPlay = this.app.popAsset(this.moduleBranch, "spriteButton", "viewMain/btnPlay_");
			vElemsLocal.btnPlay.anchor.set(0.5, 0.5);
			vElemsLocal.btnPlay.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnPlay);
			
			vElemsLocal.btnRateGame = this.app.popAsset(this.moduleBranch, "spriteButton", "viewMain/btnRateGame_");
			vElemsLocal.btnRateGame.anchor.set(0.5, 0.5);
			vElemsLocal.btnRateGame.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnRateGame);
			
			vElemsLocal.btnAchievements = this.app.popAsset(this.moduleBranch, "spriteButton", "viewMain/btnAchievements_");
			vElemsLocal.btnAchievements.anchor.set(0.5, 0.5);
			vElemsLocal.btnAchievements.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnAchievements);
		},
		
		createVisuals_extrasPanel:function(parentVElems) {
			var vElemsLocal = parentVElems.extrasPanel = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			vElemsLocal.btnShop = this.app.popAsset(this.moduleBranch, "spriteButton", "viewMain/btnShop_");
			vElemsLocal.btnShop.anchor.set(0.5, 0.5);
			vElemsLocal.btnShop.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnShop);
			
			vElemsLocal.btnShareGame = this.app.popAsset(this.moduleBranch, "spriteButton", "viewMain/btnShareGame_");
			vElemsLocal.btnShareGame.anchor.set(0.5, 0.5);
			vElemsLocal.btnShareGame.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnShareGame);
			
			vElemsLocal.btnLeaderboard = this.app.popAsset(this.moduleBranch, "spriteButton", "viewMain/btnLeaderboard_");
			vElemsLocal.btnLeaderboard.anchor.set(0.5, 0.5);
			vElemsLocal.btnLeaderboard.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnLeaderboard);
			
			vElemsLocal.tbtnSound = this.app.popAsset(this.moduleBranch, "spriteButton", "viewMain/tbtnSound_");
			vElemsLocal.tbtnSound.anchor.set(0.5, 0.5);
			vElemsLocal.tbtnSound.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.tbtnSound);
		},
		
		onBtnClick:function(event) {
			if(!this.viewCompound || this.viewCompound.state.stateEnd.indexOf("sIn") != 0) {
				return ;
			}
			
			if(event.target === this.vElems.playPanel.btnPlay) {
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.MAIN, "sOut", true, null, 0, null, {
					viewID: ViewType.IN_GAME,
					stateEnd: "sIn",
					doTransition: false,
					extraData: {
						levelID: LevelID.Terra,
						snLevelIndice: new SafeNumber(1)
					},
					delayMS: 0,
					cbcAnimationFinish: null,
					finishCommand: null
				});
			}else if(event.target === this.vElems.playPanel.btnRateGame) {
				console.log("asd dev btnClick btnRateGame");
				
				this.postDevLog("Not implemented.");
			}else if(event.target === this.vElems.playPanel.btnAchievements) {
				console.log("asd dev btnClick btnAchievements");
				
				this.postDevLog("Not implemented.");
			}else if(event.target === this.vElems.extrasPanel.btnShop) {
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.MAIN, "sOut", true, null, 0, null, {
					viewID: ViewType.SHOP,
					stateEnd: "sIn",
					doTransition: true,
					extraData: {
						closeViewCommand: {
							viewID: ViewType.MAIN,
							stateEnd: "sIn",
							doTransition: true,
							extraData: null,
							delayMS: 0,
							cbcAnimationFinish: null,
							finishCommand: null
						}
					},
					delayMS: 0,
					cbcAnimationFinish: null,
					finishCommand: null
				});
			}else if(event.target === this.vElems.extrasPanel.btnShareGame) {
				console.log("asd dev btnClick btnShareGame");
				
				this.postDevLog("Not implemented.");
			}else if(event.target === this.vElems.extrasPanel.btnLeaderboard) {
				console.log("asd dev btnClick btnLeaderboard");
				
				this.postDevLog("Not implemented.");
			}else if(event.target === this.vElems.extrasPanel.tbtnSound) {
				this.publisher.publish(this.publishMsg.toggleAudioUnmuteState);
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
