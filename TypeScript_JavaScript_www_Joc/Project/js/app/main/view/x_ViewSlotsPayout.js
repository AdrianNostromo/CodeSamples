/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/main/view/util/ViewType"
], function(declare, ModuleBase, 
		ViewType
	){
	"use strict";
	return declare("app.main.view.ViewSlotsPayout", ModuleBase, {
		moduleBranch:"global",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"initializeView", cbName:"onInitializeView", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp",  listenerType:"unique", mustExist:true},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false},
			activateInGame:{channel:"activateInGame", listenerType:"list", mustExist:false}
		},
		
		app: null,
		
		viewID: ViewType.SLOTS_PAYOUT,
		viewCompound: null,
		
		vElems: null,
		
		cfg: {
		},
		
		viewConfig: {
		},
		
		viewLayout: [
			{
				id:"container",
			},
			{
				id:"panelBg",
				params:[
					{prop:"width", val:{wPercent:640 / 640, targetID:"_app"}},
					{prop:"height", val:{hPercent:960 / 960, targetID:"_app"}},
					{prop:"alpha", val:{hasStates: true,
						sIn:0.9,
						sInEaseName: "easeOutSine",
						sOut:0,
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
							{prop:"y", val:{hasStates: true,
								sIn:{hPercent:91 / 960, targetID:"_app"},
								sInEaseName: "easeOutSine",
								sOut:{hPercent:(91 - 960) / 960, targetID:"_app"},
								sOutEaseName: "easeOutSine"
							}}
						]
					},
					{
						id:"bg",
						params:[
							{prop:"width", val:{wPercent:640 / 640, targetID:"_app"}},
							{prop:"height", val:{hPercent:106 / 960, targetID:"_app"}}
						]
					},
					{
						id:"imgTitle",
						params:[
							{prop:"width", val:{wPercent:328 / 640, targetID:"_app"}},
							{prop:"height", val:{hPercent:134 / 960, targetID:"_app"}},
							{prop:"x", val:{wPercent:278 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:40 / 960, targetID:"_app"}}
						]
					},
					{
						id:"btnClose",
						params:[
							{prop:"_heightWidth", val:{hPercent:80 / 960, targetID:"_app"}},
							{prop:"x", val:{wPercent:537 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:53 / 960, targetID:"_app"}}
						]
					}
				]
			},
			{
				id:"actionPanel",
				list:[
					{
						id:"container",
						params:[
							{prop:"y", val:{hasStates: true,
								sIn:{hPercent:302 / 960, targetID:"_app"},
								sInEaseName: "easeOutSine",
								sOut:{hPercent:(302 - 960) / 960, targetID:"_app"},
								sOutEaseName: "easeOutSine"
							}}
						]
					},
					{
						id:"bgA",
						params:[
							{prop:"width", val:{wPercent:544 / 640, targetID:"_app"}},
							{prop:"height", val:{hPercent:106 / 960, targetID:"_app"}},
							{prop:"y", val:{hPercent:0 / 960, targetID:"_app"}}
						]
					},
					{
						id:"symbolIconA",
						params:[
							{prop:"_heightWidth", val:{hPercent:(109 * 0.5) / 960, targetID:"_app"}},
							{prop:"x", val:{wPercent:110 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:(0 + 53) / 960, targetID:"_app"}}
						]
					},
					{
						id:"lblPayoutSymbolA",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {wPercent:53 / 640, roundType: "ceil", targetID:"_app"}, {value:"px 'Dosis-ExtraBold'"}]}},
							{prop:"x", val:{wPercent:339 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:(0 + 53) / 960, targetID:"_app"}}
						]
					},
					{
						id:"bgB",
						params:[
							{prop:"width", val:{wPercent:544 / 640, targetID:"_app"}},
							{prop:"height", val:{hPercent:106 / 960, targetID:"_app"}},
							{prop:"y", val:{hPercent:118 / 960, targetID:"_app"}}
						]
					},
					{
						id:"symbolIconB",
						params:[
							{prop:"_heightWidth", val:{hPercent:(109 * 0.5) / 960, targetID:"_app"}},
							{prop:"x", val:{wPercent:110 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:(118 + 53) / 960, targetID:"_app"}}
						]
					},
					{
						id:"lblPayoutSymbolB",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {wPercent:53 / 640, roundType: "ceil", targetID:"_app"}, {value:"px 'Dosis-ExtraBold'"}]}},
							{prop:"x", val:{wPercent:339 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:(118 + 53) / 960, targetID:"_app"}}
						]
					},
					{
						id:"bgC",
						params:[
							{prop:"width", val:{wPercent:544 / 640, targetID:"_app"}},
							{prop:"height", val:{hPercent:106 / 960, targetID:"_app"}},
							{prop:"y", val:{hPercent:236 / 960, targetID:"_app"}}
						]
					},
					{
						id:"symbolIconC",
						params:[
							{prop:"_heightWidth", val:{hPercent:(113 * 0.5) / 960, targetID:"_app"}},
							{prop:"x", val:{wPercent:110 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:(236 + 53) / 960, targetID:"_app"}}
						]
					},
					{
						id:"lblPayoutSymbolC",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {wPercent:53 / 640, roundType: "ceil", targetID:"_app"}, {value:"px 'Dosis-ExtraBold'"}]}},
							{prop:"x", val:{wPercent:339 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:(236 + 53) / 960, targetID:"_app"}}
						]
					},
					{
						id:"bgD",
						params:[
							{prop:"width", val:{wPercent:544 / 640, targetID:"_app"}},
							{prop:"height", val:{hPercent:106 / 960, targetID:"_app"}},
							{prop:"y", val:{hPercent:354 / 960, targetID:"_app"}}
						]
					},
					{
						id:"symbolIconD",
						params:[
							{prop:"_heightWidth", val:{hPercent:(109 * 0.5) / 960, targetID:"_app"}},
							{prop:"x", val:{wPercent:110 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:(354 + 53) / 960, targetID:"_app"}}
						]
					},
					{
						id:"lblPayoutSymbolD",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {wPercent:53 / 640, roundType: "ceil", targetID:"_app"}, {value:"px 'Dosis-ExtraBold'"}]}},
							{prop:"x", val:{wPercent:339 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:(354 + 53) / 960, targetID:"_app"}}
						]
					},
					{
						id:"bgE",
						params:[
							{prop:"width", val:{wPercent:544 / 640, targetID:"_app"}},
							{prop:"height", val:{hPercent:106 / 960, targetID:"_app"}},
							{prop:"y", val:{hPercent:472 / 960, targetID:"_app"}}
						]
					},
					{
						id:"symbolIconE",
						params:[
							{prop:"_heightWidth", val:{hPercent:(109 * 0.5) / 960, targetID:"_app"}},
							{prop:"x", val:{wPercent:110 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:(472 + 53) / 960, targetID:"_app"}}
						]
					},
					{
						id:"lblPayoutSymbolE",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {wPercent:53 / 640, roundType: "ceil", targetID:"_app"}, {value:"px 'Dosis-ExtraBold'"}]}},
							{prop:"x", val:{wPercent:339 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:(472 + 53) / 960, targetID:"_app"}}
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
			this.createVisuals_actionPanel(this.vElems);
		},
		
		onViewActivated: function(extraData) {
			this.updateData(extraData);
		},
		
		createVisuals_base:function(parentVElems) {
			var vElemsLocal = parentVElems;
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			
			vElemsLocal.panelBg = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/panelBg_white");
			vElemsLocal.panelBg.anchor.set(0, 0);
			vElemsLocal.panelBg.interactive = true;
			vElemsLocal.container.addChild(vElemsLocal.panelBg);
		},
		
		createVisuals_titlePanel:function(parentVElems) {
			var vElemsLocal = parentVElems.titlePanel = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			vElemsLocal.bg = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plank1X");
			vElemsLocal.bg.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bg);
			
			vElemsLocal.imgTitle = this.app.popAsset(this.moduleBranch, "sprite", "viewSlotsPayout/title");
			vElemsLocal.imgTitle.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.imgTitle);
			
			vElemsLocal.btnClose = this.app.popAsset(this.moduleBranch, "spriteButton", "uiGeneral/btnClose_");
			vElemsLocal.btnClose.anchor.set(0.5, 0.5);
			vElemsLocal.btnClose.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnClose);
		},
		
		updateData: function(extraData) {
			//asd_001;// update the games played and max lvl;
		},
		
		createVisuals_actionPanel:function(parentVElems) {
			var vElemsLocal = parentVElems.actionPanel = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			// Next plank;
			
			vElemsLocal.bgA = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plank1X");
			vElemsLocal.bgA.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bgA);
			
			vElemsLocal.symbolIconA = this.app.popAsset(this.moduleBranch, "sprite", "uiSlots/slotsSymbol_A");
			vElemsLocal.symbolIconA.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.symbolIconA);
			
			vElemsLocal.lblPayoutSymbolA = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "10% | 20% | 40%",
				style: {
					font: "800 53px 'Dosis-ExtraBold'",
					fill: "#FFFFFF",
					align: "center"
				}
			});
			vElemsLocal.lblPayoutSymbolA.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblPayoutSymbolA);
			
			// Next plank;
			
			vElemsLocal.bgB = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plank1X");
			vElemsLocal.bgB.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bgB);
			
			vElemsLocal.symbolIconB = this.app.popAsset(this.moduleBranch, "sprite", "uiSlots/slotsSymbol_B");
			vElemsLocal.symbolIconB.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.symbolIconB);
			
			vElemsLocal.lblPayoutSymbolB = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "10% | 30% | 35%",
				style: {
					font: "800 53px 'Dosis-ExtraBold'",
					fill: "#FFFFFF",
					align: "center"
				}
			});
			vElemsLocal.lblPayoutSymbolB.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblPayoutSymbolB);
			
			// Next plank;
			
			vElemsLocal.bgC = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plank1X");
			vElemsLocal.bgC.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bgC);
			
			vElemsLocal.symbolIconC = this.app.popAsset(this.moduleBranch, "sprite", "uiSlots/slotsSymbol_C");
			vElemsLocal.symbolIconC.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.symbolIconC);
			
			vElemsLocal.lblPayoutSymbolC = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "30% | 35% | 40%",
				style: {
					font: "800 53px 'Dosis-ExtraBold'",
					fill: "#FFFFFF",
					align: "center"
				}
			});
			vElemsLocal.lblPayoutSymbolC.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblPayoutSymbolC);
			
			// Next plank;
			
			vElemsLocal.bgD = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plank1X");
			vElemsLocal.bgD.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bgD);
			
			vElemsLocal.symbolIconD = this.app.popAsset(this.moduleBranch, "sprite", "uiSlots/slotsSymbol_D");
			vElemsLocal.symbolIconD.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.symbolIconD);
			
			vElemsLocal.lblPayoutSymbolD = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "10% | 20% | 50%",
				style: {
					font: "800 53px 'Dosis-ExtraBold'",
					fill: "#FFFFFF",
					align: "center"
				}
			});
			vElemsLocal.lblPayoutSymbolD.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblPayoutSymbolD);
			
			// Next plank;
			
			vElemsLocal.bgE = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plank1X");
			vElemsLocal.bgE.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bgE);
			
			vElemsLocal.symbolIconE = this.app.popAsset(this.moduleBranch, "sprite", "uiSlots/slotsSymbol_E");
			vElemsLocal.symbolIconE.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.symbolIconE);
			
			vElemsLocal.lblPayoutSymbolE = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "10% | 15% | 90%",
				style: {
					font: "800 53px 'Dosis-ExtraBold'",
					fill: "#FFFFFF",
					align: "center"
				}
			});
			vElemsLocal.lblPayoutSymbolE.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblPayoutSymbolE);
		},
		
		onBtnClick:function(event) {
			if(!this.viewCompound || this.viewCompound.state.stateEnd.indexOf("sIn") != 0) {
				return ;
			}
			
			if(event.target === this.vElems.titlePanel.btnClose) {
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.SLOTS_PAYOUT, "sOut", true, null, 0, null, null);
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
