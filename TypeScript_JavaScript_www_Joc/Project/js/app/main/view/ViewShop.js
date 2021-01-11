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
	"app/main/util/ShopItemType",
	"app/main/util/PopupMessageType"
], function(declare, ModuleBase, 
		ViewType, TimeUltil, PIXI, VisualUtil, EaseFunctions,
		LevelID, ShopItemType, PopupMessageType
	){
	"use strict";
	return declare("app.main.view.ViewShop", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"initializeView", cbName:"onInitializeView", isUnique:false},
			{channel:"profileDataDiamondsChange", cbName:"onProfileDataDiamondsChange", isUnique:false},
			{channel:"appLoopUserInterface", cbName:"onAppLoopUserInterface", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp",  listenerType:"unique", mustExist:true},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false},
			activateInGame:{channel:"activateInGame", listenerType:"list", mustExist:false},
			activatePopup:{channel:"activatePopup", listenerType:"any", mustExist:false}
		},
		
		app: null,
		
		viewID: ViewType.SHOP,
		viewCompound: null,
		
		vElems: null,
		
		cfg: {
			freeDiamondsAmount: 20,
			// 19:59:59
			freeBonusDelay: 20 * 60 * 60 * 1000 - 1
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
				id:"extraBg",
				visual: {
					type: "sprite",
					assetId: "viewBackground/bgA",
					parentID: "container"
				},
				params:[
					{prop:"width", val:{percent:640 / 640, param:"width", targetID:"_app"}},
					{prop:"height", val:{percent:960 / 960, param:"height", targetID:"_app"}},
					{prop:"alpha", val:{hasStates: true,
						defaultEaseName: "easeOutSine",
						sIn:1,
						sOut:0
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
							{prop:"y", val:{percent:23 / 960, param:"height", targetID:"_app"}}
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
						id:"imgTitle",
						visual: {
							type: "sprite",
							assetId: "viewShop/title",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5}
						],
						params:[
							{prop:"_widthHeight", val:{percent:285 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:323 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:80 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"btnClose",
						visual: {
							type: "spriteButton",
							assetId: "uiGeneral/btnClose_",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
							{prop: "cbcClick", val:{param:"CBC_CLICK", targetID:"_customValues"}}
						],
						params:[
							{prop:"_heightWidth", val:{percent:80 / 960, param:"height", targetID:"_app"}},
							{prop:"x", val:{percent:535 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:58 / 960, param:"height", targetID:"_app"}}
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
									{prop:"y", val:{percent:188 / 960, param:"height", targetID:"_app"}}
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
									{prop:"_widthHeight", val:{percent:98 / 640, param:"width", targetID:"_app"}},
									{prop:"x", val:{percent:-101 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:2 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"lblDiamonds",
								visual: {
									type: "text",
									extraData: {
										text: "0000",
										style: {
											font: "800 77px 'WFDosis'",
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
									{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:77 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
									{prop:"x", val:{percent:47 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:0 / 960, param:"height", targetID:"_app"}}
								]
							}
						]
					}
				]
			},
			{
				id:"shopItemsPanel",
				list:[
					{
						id:"container",
						visual: {
							type: "container",
							parentID: ["_parent", "container"]
						},
						params:[
							{prop:"y", val:{percent:320 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"bg",
						visual: {
							type: "sprite",
							assetId: "uiGeneral/plankLarge",
							parentID: "container"
						},
						params:[
							{prop:"width", val:{percent:640 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:610 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"panelA",
						list:[
							{
								id:"container",
								visual: {
									type: "container",
									parentID: ["_parent", "container"]
								},
								params:[
									{prop:"x", val:{percent:72 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:34 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"bg",
								visual: {
									type: "sprite",
									assetId: "viewShop/panelA",
									parentID: "container"
								},
								params:[
									{prop:"width", val:{percent:189 / 640, param:"width", targetID:"_app"}},
									{prop:"height", val:{percent:189 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"imgDiamonds",
								visual: {
									type: "sprite",
									assetId: "viewShop/diamondsPileA",
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5}
								],
								params:[
									{prop:"_widthHeight", val:{percent:170 / 640, param:"width", targetID:"_app"}},
									{prop:"x", val:{percent:89 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:80 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"transparentOverlay",
								visual: {
									type: "sprite",
									assetId: "viewShop/panelA_transparentOverlay",
									parentID: "container"
								},
								initParams: [
									{prop: "alpha", val: 0.6}
								],
								params:[
									{prop:"width", val:{percent:189 / 640, param:"width", targetID:"_app"}},
									{prop:"height", val:{percent:189 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"bonusBg",
								visual: {
									type: "sprite",
									assetId: "viewShop/iconCorner",
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 1}, {prop: ["anchor", "y"], val: 0}
								],
								params:[
									{prop:"width", val:{percent:95 / 640, param:"width", targetID:"_app"}},
									{prop:"height", val:{percent:94 / 960, param:"height", targetID:"_app"}},
									{prop:"x", val:{percent:180 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:0 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"flblBonus",
								visual: {
									type: "text",
									extraData: {
										text: "Free",
										style: {
											font: "800 36px 'WFDosis'",
											fill: "#FFFFFF",
											align: "center"
										}
									},
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0}
								],
								_refreshTextStyle: true,
								params:[
									{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:36 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
									{prop:"rotation", val:45 * (Math.PI/180)},
									{prop:"x", val:{percent:160 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:15 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"flblDiamonds",
								visual: {
									type: "text",
									extraData: {
										text: "",
										style: {
											font: "800 46px 'WFDosis'",
											fill: "#44BFFB",
											align: "center"
										}
									},
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 1}
								],
								_refreshTextStyle: true,
								params:[
									{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:46 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
									{prop:"x", val:{percent:90 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:186 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"lblBonusTime",
								visual: {
									type: "text",
									extraData: {
										text: "20:00:00",
										style: {
											font: "800 44px 'WFDosis'",
											fill: "#FFFFFF",
											align: "center",
											stroke: "#3A2914",
											strokeThickness: 8
										}
									},
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
									{prop:"rotation", val:45 * (Math.PI/180)}
								],
								_refreshTextStyle: true,
								params:[
									{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:44 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
									{prop:["style", "strokeThickness"], val:{percent:8 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}},
									{prop:"x", val:{percent:94 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:94 / 960, param:"height", targetID:"_app"}}
								]
							}
						]
					},
					{
						id:"panelB",
						list:[
							{
								id:"container",
								visual: {
									type: "container",
									parentID: ["_parent", "container"]
								},
								params:[
									{prop:"x", val:{percent:284 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:34 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"bg",
								visual: {
									type: "sprite",
									assetId: "viewShop/panelB",
									parentID: "container"
								},
								params:[
									{prop:"width", val:{percent:289 / 640, param:"width", targetID:"_app"}},
									{prop:"height", val:{percent:189 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"imgDiamonds",
								visual: {
									type: "sprite",
									assetId: "viewShop/diamondsPileB",
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5}
								],
								params:[
									{prop:"_widthHeight", val:{percent:245 / 640, param:"width", targetID:"_app"}},
									{prop:"x", val:{percent:140 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:74 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"flblDiamonds",
								visual: {
									type: "text",
									extraData: {
										text: "+400",
										style: {
											font: "800 46px 'WFDosis'",
											fill: "#44BFFB",
											align: "center"
										}
									},
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 1}
								],
								_refreshTextStyle: true,
								params:[
									{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:46 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
									{prop:"x", val:{percent:140 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:186 / 960, param:"height", targetID:"_app"}}
								]
							}
						]
					},
					{
						id:"panelC",
						list:[
							{
								id:"container",
								visual: {
									type: "container",
									parentID: ["_parent", "container"]
								},
								params:[
									{prop:"x", val:{percent:72 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:251 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"bg",
								visual: {
									type: "sprite",
									assetId: "viewShop/panelC",
									parentID: "container"
								},
								params:[
									{prop:"width", val:{percent:189 / 640, param:"width", targetID:"_app"}},
									{prop:"height", val:{percent:339 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"bonusBg",
								visual: {
									type: "sprite",
									assetId: "viewShop/iconCorner",
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 1}, {prop: ["anchor", "y"], val: 0}
								],
								params:[
									{prop:"width", val:{percent:95 / 640, param:"width", targetID:"_app"}},
									{prop:"height", val:{percent:94 / 960, param:"height", targetID:"_app"}},
									{prop:"x", val:{percent:180 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:0 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"flblBonus",
								visual: {
									type: "text",
									extraData: {
										text: "+50%",
										style: {
											font: "800 36px 'WFDosis'",
											fill: "#FFFFFF",
											align: "center"
										}
									},
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0}
								],
								_refreshTextStyle: true,
								params:[
									{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:36 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
									{prop:"rotation", val:45 * (Math.PI/180)},
									{prop:"x", val:{percent:160 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:15 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"imgDiamonds",
								visual: {
									type: "sprite",
									assetId: "viewShop/diamondsPileC",
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5}
								],
								params:[
									{prop:"_widthHeight", val:{percent:158 / 640, param:"width", targetID:"_app"}},
									{prop:"x", val:{percent:91 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:148 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"flblDiamonds",
								visual: {
									type: "text",
									extraData: {
										text: "+1200",
										style: {
											font: "800 46px 'WFDosis'",
											fill: "#44BFFB",
											align: "center"
										}
									},
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 1}
								],
								_refreshTextStyle: true,
								params:[
									{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:46 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
									{prop:"x", val:{percent:90 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:336 / 960, param:"height", targetID:"_app"}}
								]
							}
						]
					},
					{
						id:"panelD",
						list:[
							{
								id:"container",
								visual: {
									type: "container",
									parentID: ["_parent", "container"]
								},
								params:[
									{prop:"x", val:{percent:284 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:251 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"bg",
								visual: {
									type: "sprite",
									assetId: "viewShop/panelD",
									parentID: "container"
								},
								params:[
									{prop:"width", val:{percent:289 / 640, param:"width", targetID:"_app"}},
									{prop:"height", val:{percent:339 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"bonusBg",
								visual: {
									type: "sprite",
									assetId: "viewShop/iconCorner",
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 1}, {prop: ["anchor", "y"], val: 0}
								],
								params:[
									{prop:"width", val:{percent:95 / 640, param:"width", targetID:"_app"}},
									{prop:"height", val:{percent:94 / 960, param:"height", targetID:"_app"}},
									{prop:"x", val:{percent:280 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:0 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"flblBonus",
								visual: {
									type: "text",
									extraData: {
										text: "+150%",
										style: {
											font: "800 36px 'WFDosis'",
											fill: "#FFFFFF",
											align: "center"
										}
									},
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0}
								],
								_refreshTextStyle: true,
								params:[
									{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:36 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
									{prop:"rotation", val:45 * (Math.PI/180)},
									{prop:"x", val:{percent:260 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:15 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"imgDiamonds",
								visual: {
									type: "sprite",
									assetId: "viewShop/diamondsPileD",
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5}
								],
								params:[
									{prop:"_widthHeight", val:{percent:238 / 640, param:"width", targetID:"_app"}},
									{prop:"x", val:{percent:141 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:157 / 960, param:"height", targetID:"_app"}}
								]
							},
							{
								id:"flblDiamonds",
								visual: {
									type: "text",
									extraData: {
										text: "+4000",
										style: {
											font: "800 46px 'WFDosis'",
											fill: "#44BFFB",
											align: "center"
										}
									},
									parentID: "container"
								},
								initParams: [
									{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 1}
								],
								_refreshTextStyle: true,
								params:[
									{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:46 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
									{prop:"x", val:{percent:140 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:336 / 960, param:"height", targetID:"_app"}}
								]
							}
						]
					},
					{
						id:"iconMostPopular",
						visual: {
							type: "sprite",
							assetId: "viewShop/iconMostPopular",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 1}
						],
						params:[
							{prop:"_widthHeight", val:{percent:143 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:408 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:307 / 960, param:"height", targetID:"_app"}}
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
					cbcViewActivatedPre: {callback: this.onViewActivatedPre, context: this},
					cbcViewActivated: {callback: this.onViewActivated, context: this}
				};
			}
		},
		
		onViewActivatedPre: function(extraData) {
			if(extraData && extraData.showExtraBackground) {
				this.vElems.extraBg.visible = true;
			}else {
				this.vElems.extraBg.visible = false;
			}
		},
		
		createVisuals:function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.vElems = {isVContainer: true};
			
			VisualUtil.createVLayoutVisuals(this.vElems, this.viewLayout, this.app, this.moduleBranch, null, null, {CBC_CLICK: {callback: this.onBtnClick, context: this}});
			
			this.vElems.shopItemsPanel.panelA.flblDiamonds.text = "+" + this.cfg.freeDiamondsAmount;
			
			this.vElems.shopItemsPanel.panelA.container.interactive = true;
			this.vElems.shopItemsPanel.panelA.container.on('touchstart', this.onShopItemTouchStartA.bind(this));
			
			this.vElems.shopItemsPanel.panelB.container.interactive = true;
			this.vElems.shopItemsPanel.panelB.container.on('touchstart', this.onShopItemTouchStartB.bind(this));
			
			this.vElems.shopItemsPanel.panelC.container.interactive = true;
			this.vElems.shopItemsPanel.panelC.container.on('touchstart', this.onShopItemTouchStartC.bind(this));
			
			this.vElems.shopItemsPanel.panelD.container.interactive = true;
			this.vElems.shopItemsPanel.panelD.container.on('touchstart', this.onShopItemTouchStartD.bind(this));
		},
		
		onViewActivated: function(extraData) {
			var closeViewCommand = null;
			if(extraData && "closeViewCommand" in extraData) {
				closeViewCommand = extraData.closeViewCommand;
			}
			this.data = {
				closeViewCommand: closeViewCommand,
				
				lastDiamondsLblUpdateValue: -1,
				targetPlayerDiamonds: 0,
				
				lastFreeDiamondsTimestamp: 0,
				freeDiamondsDelayActive: false,
				lastFreeDiamondsTimerSeconds: 0
			}
			
			this.initFreeDiamondsTimer();
			
			this.updateViewData(extraData);
		},
		
		initFreeDiamondsTimer: function() {
			this.data.lastFreeDiamondsTimestamp = this.app.profileData.getLastFreeDiamondsTimestamp();
			var tm = new Date().getTime();
			if(this.data.lastFreeDiamondsTimestamp + this.cfg.freeBonusDelay > tm) {
				// Delay is active.
				
				this.data.freeDiamondsDelayActive = true;
				
				this.vElems.shopItemsPanel.panelA.transparentOverlay.visible = true;
				this.vElems.shopItemsPanel.panelA.lblBonusTime.visible = true;
				
				var timerMS = (this.data.lastFreeDiamondsTimestamp + this.cfg.freeBonusDelay) - tm;
				var timerSeconds = Math.ceil(timerMS / 1000);
				
				var timerText = this.prepareTimerText(timerSeconds * 1000);
				this.vElems.shopItemsPanel.panelA.lblBonusTime.text = timerText;
				
				this.data.lastFreeDiamondsTimerSeconds = timerSeconds;
			}else {
				// Available.
				
				this.data.freeDiamondsDelayActive = false;
				
				this.vElems.shopItemsPanel.panelA.transparentOverlay.visible = false;
				this.vElems.shopItemsPanel.panelA.lblBonusTime.visible = false;
			}
		},
		
		prepareTimerText: function(timeMS) {
			var seconds = Math.floor((timeMS / 1000) % 60);
			var minutes = Math.floor((timeMS / (1000*60)) % 60);
			var hours = Math.floor((timeMS / (1000*60*60)) % 24);
			
			var ret = "";
			if(hours > 0) {
				ret = ret + ((hours < 10) ? "0" : "") + hours + ":";
			}
			if(hours > 0 || minutes > 0) {
				ret = ret + ((minutes < 10) ? "0" : "") + minutes + ":";
			}
			if(hours > 0 || minutes > 0 || seconds > 0) {
				ret = ret + ((seconds < 10) ? "0" : "") + seconds;
			}
			
			return ret;
		},
		
		updateViewData: function(extraData) {
			this.updatePlayerDiamonds(false);
		},
		
		updatePlayerDiamonds: function(doAnimation) {
			this.data.targetPlayerDiamonds = this.app.profileData.getDiamonds();
			
			if(!doAnimation) {
				this.vElems.titlePanel.subtitlePanel.lblDiamonds.text = this.data.targetPlayerDiamonds;
				
				this.data.lastDiamondsLblUpdateValue = this.data.targetPlayerDiamonds;
			}
		},
		
		onProfileDataDiamondsChange: function() {
			if(!this.app || (!this.viewCompound || this.viewCompound.state.stateEnd.indexOf("sIn") != 0)) {
				return ;
			}
			
			this.updatePlayerDiamonds(true);
		},
		
		onAppLoopUserInterface:function(appTime, deltaMS, deltaS) {
			if(this.viewCompound && this.viewCompound.state.completionPercent > 0) {
				if(this.data.freeDiamondsDelayActive) {
					var tm = new Date().getTime();
					
					if(this.data.lastFreeDiamondsTimestamp + this.cfg.freeBonusDelay > tm) {
						// Delay is active.
						var timerMS = (this.data.lastFreeDiamondsTimestamp + this.cfg.freeBonusDelay) - tm;
						var timerSeconds = Math.ceil(timerMS / 1000);
						
						if(this.data.lastFreeDiamondsTimerSeconds != timerSeconds) {
							var timerText = this.prepareTimerText(timerSeconds * 1000);
							this.vElems.shopItemsPanel.panelA.lblBonusTime.text = timerText;
							
							this.data.lastFreeDiamondsTimerSeconds = timerSeconds;
						}
					}else {
						// Available.
						
						this.initFreeDiamondsTimer();
					}
				}
				
				if(this.data.lastDiamondsLblUpdateValue != this.data.targetPlayerDiamonds) {
					var diff = Math.abs(this.data.targetPlayerDiamonds - this.data.lastDiamondsLblUpdateValue);
					var direction = diff / (this.data.targetPlayerDiamonds - this.data.lastDiamondsLblUpdateValue);
					var finalDelta = Math.min(diff, Math.max(0.5, diff / 20));
					
					var newVal = this.data.lastDiamondsLblUpdateValue + finalDelta * direction;
					
					this.vElems.titlePanel.subtitlePanel.lblDiamonds.text = Math.round(newVal);
					
					this.data.lastDiamondsLblUpdateValue = newVal;
				}
			}
		},
		
		onShopItemTouchStartA: function(event) {
			this.buyShopItemAtempt(ShopItemType.DIAMONDS_PILE_A);
		},
		
		onShopItemTouchStartB: function(event) {
			this.buyShopItemAtempt(ShopItemType.DIAMONDS_PILE_B);
		},
		
		onShopItemTouchStartC: function(event) {
			this.buyShopItemAtempt(ShopItemType.DIAMONDS_PILE_C);
		},
		
		onShopItemTouchStartD: function(event) {
			this.buyShopItemAtempt(ShopItemType.DIAMONDS_PILE_D);
		},
		
		buyShopItemAtempt: function(shopItemType) {
			if(shopItemType === ShopItemType.DIAMONDS_PILE_A) {
				// If timer active, show a popup.
				var tm = new Date().getTime();
				this.cfg.freeBonusDelay;
				
				if(this.data.lastFreeDiamondsTimestamp + this.cfg.freeBonusDelay <= tm) {
					// Give diamonds to the player.
					
					this.app.profileData.addDiamonds(this.cfg.freeDiamondsAmount);
					
					this.app.profileData.setLastFreeDiamondsTimestamp(tm);
					
					this.initFreeDiamondsTimer();
					
					this.vElems.shopItemsPanel.container.y
					
					this.vElems.shopItemsPanel.panelA.container.y
					
					this.vElems.shopItemsPanel.panelA.bg.height / 2
					var fxPos = {
						x: this.vElems.shopItemsPanel.container.x + this.vElems.shopItemsPanel.panelA.container.x + this.vElems.shopItemsPanel.panelA.bg.width / 2,
						y: this.vElems.shopItemsPanel.container.y + this.vElems.shopItemsPanel.panelA.container.y + this.vElems.shopItemsPanel.panelA.bg.height / 2
					};
					this.app.visualFx.activateFX("viewShop_bonusCollected", 0, fxPos, null, this.moduleBranch, null, false, this.viewID);
				}else {
					this.publisher.publish(this.publishMsg.activatePopup, PopupMessageType.NOT_AVAILABLE, true);
				}
			}else {
				// asd_002; // initialize buy command;
				
				console.log("dev buyItem: " + shopItemType);
			}
		},
		
		onBtnClick:function(event) {
			if(!this.viewCompound || this.viewCompound.state.stateEnd.indexOf("sIn") != 0) {
				return ;
			}
			
			if(event.target === this.vElems.titlePanel.btnClose) {
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.SHOP, "sOut", true, null, 0, null, this.data.closeViewCommand);
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
