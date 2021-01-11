/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/main/view/util/ViewType",
	"app/util/SafeNumber",
	"app/main/util/PopupMessageType",
	"app/util/VisualUtil"
], function(declare, ModuleBase, 
		ViewType, SafeNumber, PopupMessageType, VisualUtil
	){
	"use strict";
	return declare("app.main.view.ViewLevelDefeat", ModuleBase, {
		moduleBranch: "main",
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
		
		viewID: ViewType.LEVEL_DEFEAT,
		viewCompound: null,
		
		extraData: null,
		
		vElems: null,
		
		cfg: {
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
						defaultEaseName: "easeOutSine",
						sIn:{percent:0 / 960, param:"height", targetID:"_app"},
						sOut:{percent:960 / 960, param:"height", targetID:"_app"}
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
							{prop:"y", val:{percent:78 / 960, param:"height", targetID:"_app"}}
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
									{prop:"y", val:{percent:212 / 960, param:"height", targetID:"_app"}}
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
									{prop:"x", val:{percent:46 / 640, param:"width", targetID:"_app"}},
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
									{prop:"x", val:{percent:60 / 640, param:"width", targetID:"_app"}},
									{prop:"y", val:{percent:0 / 960, param:"height", targetID:"_app"}}
								]
							}
						]
					},
					{
						id:"imgTitle",
						visual: {
							type: "sprite",
							assetId: "viewLevelDefeat/title",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5}
						],
						params:[
							{prop:"_widthHeight", val:{percent:400 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:323 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:82 / 960, param:"height", targetID:"_app"}}
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
							{prop:"y", val:{percent:596 / 960, param:"height", targetID:"_app"}}
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
							assetId: "viewLevelDefeat/btnHome_",
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
						id:"btnRestartLevel",
						visual: {
							type: "spriteButton",
							assetId: "viewLevelDefeat/btnRestartLevel_",
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
					cbcViewActivatedPre: {callback: this.onViewActivatedPre, context: this}
				};
			}
		},
		
		createVisuals:function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.vElems = {isVContainer: true};
			
			VisualUtil.createVLayoutVisuals(this.vElems, this.viewLayout, this.app, this.moduleBranch, null, null, {CBC_CLICK: {callback: this.onBtnClick, context: this}});
		},
		
		onViewActivatedPre: function(extraData) {
			this.updateData(extraData);
		},
		
		updateData: function(extraData) {
			this.extraData = extraData;
			
			var levelIndice = extraData.snLevelIndice.getValue();
			
			this.vElems.titlePanel.subtitlePanel.lblLevelIndice.text = levelIndice;
		},
		
		onBtnClick:function(event) {
			if(!this.viewCompound || this.viewCompound.state.stateEnd.indexOf("sIn") != 0) {
				return ;
			}
			
			if(event.target === this.vElems.actionPanel.btnHome) {
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.LEVEL_DEFEAT, "sOut", true, null, 0, null, {
					viewID: ViewType.MAIN,
					stateEnd: "sIn",
					doTransition: true,
					extraData: null,
					delayMS: 0,
					cbcAnimationFinish: null,
					finishCommand: null
				});
			}else if(event.target === this.vElems.actionPanel.btnRestartLevel) {
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.LEVEL_CLEAR, "sOut", true, null, 0, null, {
					viewID: ViewType.IN_GAME,
					stateEnd: "sIn",
					doTransition: false,
					extraData: {
						levelID: this.extraData.levelID,
						snLevelIndice: new SafeNumber(this.extraData.snLevelIndice.getValue())
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
