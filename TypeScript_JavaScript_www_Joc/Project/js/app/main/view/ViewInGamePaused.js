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
	return declare("app.main.view.ViewInGamePaused", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"initializeView", cbName:"onInitializeView", isUnique:false},
			{channel:"audioUnmuteStateChanged", cbName:"onAudioUnmuteStateChanged", isUnique:false},
			{channel:"profileDataDiamondsChange", cbName:"onProfileDataDiamondsChange", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp",  listenerType:"unique", mustExist:true},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false},
			toggleAudioUnmuteState:{channel:"toggleAudioUnmuteState", listenerType:"any", mustExist:false},
			activateInGame:{channel:"activateInGame", listenerType:"list", mustExist:false},
			gIncrementUnpauseState:{channel:"gIncrementUnpauseState", listenerType:"any", mustExist:true},
			gUserInterfaceLevelEndRequest:{channel:"gUserInterfaceLevelEndRequest", listenerType:"any", mustExist:false}
		},
		
		app: null,
		
		viewID: ViewType.IN_GAME_PAUSED,
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
					{prop:"width", val:{percent:640 / 640, param:"width", targetID:"_app"}},
					{prop:"height", val:{percent:960 / 960, param:"height", targetID:"_app"}},
					{prop:"alpha", val:{hasStates: true,
						sIn:0.7,
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
								sIn:{percent:134 / 960, param:"height", targetID:"_app"},
								sInEaseName: "easeOutSine",
								sOut:{percent:(-106) / 960, param:"height", targetID:"_app"},
								sOutEaseName: "easeOutSine"
							}}
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
						id:"imgTitle",
						params:[
							{prop:"_widthHeight", val:{percent:336 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:278 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:47 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"btnClose",
						params:[
							{prop:"_heightWidth", val:{percent:80 / 960, param:"height", targetID:"_app"}},
							{prop:"x", val:{percent:537 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:53 / 960, param:"height", targetID:"_app"}}
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
							{prop:"y", val:{percent:359 / 960, param:"height", targetID:"_app"}},
							{prop:"x", val:{hasStates: true,
								sIn:{percent:0 / 640, param:"width", targetID:"_app"},
								sInEaseName: "easeOutSine",
								sOut:{percent:-502 / 640, param:"width", targetID:"_app"},
								sOutEaseName: "easeOutSine"
							}}
						]
					},
					{
						id:"bgDiamonds",
						params:[
							{prop:"width", val:{percent:477 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}},
							{prop:"y", val:{percent:(125 * 0) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"btnDiamonds",
						params:[
							{prop:"width", val:{percent:463 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}},
							{prop:"x", val:{percent:288 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(125 * 0 + 106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"lblDiamonds",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:77 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
							{prop:"x", val:{percent:185 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(125 * 0 + 106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"bg",
						params:[
							{prop:"width", val:{percent:477 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}},
							{prop:"y", val:{percent:(125 * 1) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"tbtnSound",
						params:[
							{prop:"width", val:{percent:427 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}},
							{prop:"x", val:{percent:288 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(125 * 1 + 106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"bgB",
						params:[
							{prop:"width", val:{percent:477 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}},
							{prop:"y", val:{percent:(125 * 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"btnQuitLevel",
						params:[
							{prop:"width", val:{percent:427 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}},
							{prop:"x", val:{percent:288 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(125 * 2 + 106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"bgC",
						params:[
							{prop:"width", val:{percent:477 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}},
							{prop:"y", val:{percent:(125 * 3) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"btnResume",
						params:[
							{prop:"width", val:{percent:427 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}},
							{prop:"x", val:{percent:288 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(125 * 3 + 106 / 2) / 960, param:"height", targetID:"_app"}}
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
		
		onAudioUnmuteStateChanged: function(audioUnmuteState) {
			if(!this.app || (!this.viewCompound || this.viewCompound.state.stateEnd.indexOf("sIn") != 0)) {
				return ;
			}
			
			this.updateAudioButtonState();
		},
		
		updatePlayerDiamonds: function() {
			this.vElems.actionPanel.lblDiamonds.text = this.app.profileData.getDiamonds();
		},
		
		onProfileDataDiamondsChange: function() {
			if(!this.app || (!this.viewCompound || this.viewCompound.state.stateEnd.indexOf("sIn") != 0)) {
				return ;
			}
			
			this.updatePlayerDiamonds();
		},
		
		updateAudioButtonState: function() {
			if(!this.app.audio.isUnmuted) {
				this.vElems.actionPanel.tbtnSound.setIsSelected(false);
			}else {
				this.vElems.actionPanel.tbtnSound.setIsSelected(true);
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
			this.updateViewData(extraData);
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
			
			vElemsLocal.imgTitle = this.app.popAsset(this.moduleBranch, "sprite", "viewInGamePaused/title");
			vElemsLocal.imgTitle.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.imgTitle);
			
			vElemsLocal.btnClose = this.app.popAsset(this.moduleBranch, "spriteButton", "uiGeneral/btnClose_");
			vElemsLocal.btnClose.anchor.set(0.5, 0.5);
			vElemsLocal.btnClose.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnClose);
		},
		
		updateViewData: function(extraData) {
			this.updateAudioButtonState();
			
			this.updatePlayerDiamonds();
		},
		
		createVisuals_actionPanel:function(parentVElems) {
			var vElemsLocal = parentVElems.actionPanel = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			vElemsLocal.bgDiamonds = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plank1X");
			vElemsLocal.bgDiamonds.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bgDiamonds);
			
			vElemsLocal.btnDiamonds = this.app.popAsset(this.moduleBranch, "spriteButton", "viewInGamePaused/btnPlayerDiamonds_");
			vElemsLocal.btnDiamonds.anchor.set(0.5, 0.5);
			vElemsLocal.btnDiamonds.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnDiamonds);
			
			vElemsLocal.lblDiamonds = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "0000",
				style: {
					font: "800 77px 'WFDosis'",
					fill: "#FFFFFF",
					align: "left"
				}
			});
			vElemsLocal.lblDiamonds.anchor.set(0, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblDiamonds);
			
			vElemsLocal.bg = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plank1X");
			vElemsLocal.bg.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bg);
			
			vElemsLocal.tbtnSound = this.app.popAsset(this.moduleBranch, "spriteButton", "viewInGamePaused/tbtnSound_");
			vElemsLocal.tbtnSound.anchor.set(0.5, 0.5);
			vElemsLocal.tbtnSound.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.tbtnSound);
			
			vElemsLocal.bgB = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plank1X");
			vElemsLocal.bgB.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bgB);
			
			vElemsLocal.btnQuitLevel = this.app.popAsset(this.moduleBranch, "spriteButton", "viewInGamePaused/btnQuitLevel_");
			vElemsLocal.btnQuitLevel.anchor.set(0.5, 0.5);
			vElemsLocal.btnQuitLevel.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnQuitLevel);
			
			vElemsLocal.bgC = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plank1X");
			vElemsLocal.bgC.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bgC);
			
			vElemsLocal.btnResume = this.app.popAsset(this.moduleBranch, "spriteButton", "viewInGamePaused/btnResume_");
			vElemsLocal.btnResume.anchor.set(0.5, 0.5);
			vElemsLocal.btnResume.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnResume);
		},
		
		onBtnClick:function(event) {
			if(!this.viewCompound || this.viewCompound.state.stateEnd.indexOf("sIn") != 0) {
				return ;
			}
			
			if(event.target === this.vElems.titlePanel.btnClose) {
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.IN_GAME_PAUSED, "sOut", true, null, 0, {callback: this.onViewHideFinishForPlayResume, context: this}, null);
			}else if(event.target === this.vElems.actionPanel.btnDiamonds) {
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.SHOP, "sIn", true, {showExtraBackground: true}, 0, null, null);
			}else if(event.target === this.vElems.actionPanel.tbtnSound) {
				this.publisher.publish(this.publishMsg.toggleAudioUnmuteState);
			}else if(event.target === this.vElems.actionPanel.btnQuitLevel) {
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.IN_GAME_PAUSED, "sOut", true, null, 0, {callback: this.onViewHideFinishForLevelEnd, context: this}, null);
			}else if(event.target === this.vElems.actionPanel.btnResume) {
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.IN_GAME_PAUSED, "sOut", true, null, 0, {callback: this.onViewHideFinishForPlayResume, context: this}, null);
			}else {
				this.postError("Err 6.");
				
				return null;
			}
		},
		
		onViewHideFinishForPlayResume: function() {
			this.publisher.publish(this.publishMsg.gIncrementUnpauseState, true);
		},
		
		onViewHideFinishForLevelEnd: function() {
			this.publisher.publish(this.publishMsg.gUserInterfaceLevelEndRequest);
			
			this.publisher.publish(this.publishMsg.gIncrementUnpauseState, true);
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
