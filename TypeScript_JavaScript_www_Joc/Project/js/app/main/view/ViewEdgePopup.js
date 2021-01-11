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
	return declare("app.main.view.ViewEdgePopup", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"initializeAppDependantComponents", cbName:"onInitializeAppDependantComponents", isUnique:false},
			{channel:"initializeView", cbName:"onInitializeView", isUnique:false},
			{channel:"appLoopUserInterface", cbName:"onAppLoopUserInterface", isUnique:false},
			{channel:"activatePopup", cbName:"onActivatePopup", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp",  listenerType:"unique", mustExist:true},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false},
			activateInGame:{channel:"activateInGame", listenerType:"list", mustExist:false}
		},
		
		app: null,
		
		data: null,
		
		messagesExtraDataStack: null,
		
		cfg: {
			autoCloseTimeMS: 2500
		},
		
		messagesText: {
			INSUFFICIENT_DIAMONDS_FOR_ACTION: {
				text: "Need more diamonds.",
				sInState: "sIn"
			},
			NOT_AVAILABLE: {
				text: "Not available.",
				sInState: "sIn"
			}
		},
		
		viewID: ViewType.EDGE_POPUP,
		viewCompound: null,
		
		vElems: null,
		
		viewConfig: {
		},
		
		viewLayout: [
			{
				id:"container",
				params:[
					{prop:"x", val:{percent:320 / 640, param:"width", targetID:"_app"}},
					{prop:"y", val:{hasStates: true,
						defaultEaseName: "easeOutSine",
						sIn:{percent:890 / 960, param:"height", targetID:"_app"},
						sOut:{add: [
							{percent:960 / 960, param:"height", targetID:"_app"},
							{percent:80 / 960, param:"height", targetID:"_app"}
						]}
					}}
				]
			},
			{
				id:"mainPanel",
				list:[
					{
						id:"container"
					},
					{
						id:"bg",
						params:[
							{prop:"width", val:{percent:520 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:80 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"lblMsg",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:44 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}}
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
					parentLayer: this.app.layers.popupViews,
					dependantViews: null,
					cbcViewLayoutUpdateFinished: null,
					cbcViewActivatedPre: {callback: this.onViewActivatedPre, context: this}
				};
				
				this.viewCompound;
			}
		},
		
		onAppLoopUserInterface:function(appTime, deltaMS, deltaS) {
			if(this.data && this.data.timerLifeMS < this.cfg.autoCloseTimeMS) {
				this.data.timerLifeMS = Math.min(this.cfg.autoCloseTimeMS, this.data.timerLifeMS + deltaMS);
				
				if(this.data.timerLifeMS >= this.cfg.autoCloseTimeMS) {
					this.closeView();
				}
			}
			
			if(this.messagesExtraDataStack && this.messagesExtraDataStack.length > 0 && this.viewCompound.state.stateEnd.indexOf("sOut") == 0 && this.viewCompound.state.completionPercent >= 1) {
				var nextPopupExtraData = this.messagesExtraDataStack.shift();
				if(nextPopupExtraData) {
					this.publisher.publish(this.publishMsg.changeViewState, ViewType.EDGE_POPUP, "sIn", true, nextPopupExtraData, 0, null, null);
				}
			}
		},
		
		closeView: function() {
			if(this.data) {
				this.data = null;
				
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.EDGE_POPUP, "sOut", true, null, 0, null, null);
			}
		},
		
		onInitializeAppDependantComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.messagesExtraDataStack = [];
		},
		
		createVisuals:function() {
			this.vElems = {};
			
			this.createVisuals_base(this.vElems);
			this.createVisuals_mainPanel(this.vElems);
		},
		
		onActivatePopup: function(msgID, ignoreDuplicate) {
			if(this.messagesExtraDataStack.length > 0 || (this.viewCompound && (this.viewCompound.state.stateEnd.indexOf("sIn") == 0 || this.viewCompound.state.completionPercent < 1))) {
				if(ignoreDuplicate) {
					if(this.messagesExtraDataStack.indexOf(msgID) >= 0) {
						return ;
					}
					
					if(this.viewCompound.state.stateEnd.indexOf("sIn") == 0 && this.data && this.data.msgID === msgID) {
						return ;
					}
				}
				
				this.messagesExtraDataStack.push({
					msgID: msgID, 
					ignoreDuplicate: ignoreDuplicate
				});
				
				return;
			}
			
			this.publisher.publish(this.publishMsg.changeViewState, ViewType.EDGE_POPUP, "sIn", true, {msgID: msgID, ignoreDuplicate: ignoreDuplicate}, 0, null, null);
		},
		
		onViewActivatedPre: function(extraData) {
			if(!extraData || !(extraData.msgID in this.messagesText)) {
				this.postError("Err 6.");
				
				return null;
			}
			
			this.data = {
				extraData: extraData,
				msgID: extraData.msgID,
				msgConfig: this.messagesText[extraData.msgID],
				timerLifeMS: 0
			};
			
			if(this.viewCompound.state.stateEnd != this.data.msgConfig.sInState) {
				this.viewCompound.state.stateEnd = this.data.msgConfig.sInState;
				
				this.viewCompound.state.fDoUpdate = true;
			}
			
			this.updateData(extraData);
		},
		
		createVisuals_base:function(parentVElems) {
			var vElemsLocal = parentVElems;
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
		},
		
		createVisuals_mainPanel:function(parentVElems) {
			var vElemsLocal = parentVElems.mainPanel = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			vElemsLocal.bg = this.app.popAsset(this.moduleBranch, "sprite", "viewEdgePopup/bg");
			vElemsLocal.bg.alpha = 0.8;
			vElemsLocal.bg.anchor.set(0.5, 0.5);
			vElemsLocal.bg.interactive = true;
			vElemsLocal.bg.on('touchstart', this.onBgTouchStart.bind(this));
			vElemsLocal.container.addChild(vElemsLocal.bg);
			
			vElemsLocal.lblMsg = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "Aaaaaa",
				style: {
					font: "800 44px 'WFDosis'",
					fill: "#FFFFFF",
					align: "center"
				}
			});
			vElemsLocal.lblMsg.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblMsg);
		},
		
		onBgTouchStart: function(event) {
			if(this.viewCompound.state.stateEnd.indexOf("sOut") != 0) {
				this.closeView();
			}
		},
		
		updateData: function(extraData) {
			this.vElems.mainPanel.lblMsg.text = this.data.msgConfig.text;
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
