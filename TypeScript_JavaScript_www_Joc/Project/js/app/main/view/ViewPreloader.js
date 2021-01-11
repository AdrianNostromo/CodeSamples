/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/main/view/util/ViewType",
	"app/util/TimeUltil",
	"lib/pixijs/pixi",
	"app/util/tween/EaseFunctions"
], function(declare, ModuleBase, 
		ViewType, TimeUltil, PIXI, EaseFunctions
	){
	"use strict";
	return declare("app.main.view.ViewPreloader", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"initializeView", cbName:"onInitializeView", isUnique:false},
			{channel:"appLoopUserInterface", cbName:"onAppLoopUserInterface", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp",  listenerType:"unique", mustExist:true}
		},
		
		app: null,
		
		knobDirectionX: 1,
		knobPercentPosX: 0.5,
		
		viewID: ViewType.PRELOADER,
		viewCompound: null,
		
		vElems: null,
		
		cfg: {
			knobPercentSpeedH: 0.268,
			knobLeftEdgeP: 134 / 640,
			knobRightEdgeP: 506 / 640,
		},
		
		viewConfig: {
		},
		
		viewLayout: [
			{
				id:"bg",
				params:[
					{prop:"width", val:{percent:1, param:"width", targetID:"_app"}},
					{prop:"width", val:{percent:1, param:"width", targetID:"_app"}},
					{prop:"height", val:{percent:1, param:"height", targetID:"_app"}},
					{prop:"alpha", val:{hasStates: true,
						defaultEaseName: "easeOutSine",
						sIn:1,
						sOut:0
					}}
				]
			},
			{
				id:"imgTitle",
				params:[
					{prop:"_widthHeight", val:{percent:520 / 640, param:"width", targetID:"_app"}},
					{prop:"x", val:{percent:323 / 640, param:"width", targetID:"_app"}},
					{prop:"y", val:{hasStates: true,
						defaultEaseName: "easeOutSine",
						sIn:{percent:300 / 960, param:"height", targetID:"_app"},
						sOut:{subtract:[
							{percent:0 / 960, param:"height", targetID:"_app"},
							{percent:0.5, param:"height", targetID:"_self"}
						]}
					}}
				]
			},
			{
				id:"loaderBg",
				params:[
					{prop: "_widthHeight", val:{percent:500 / 640, param:"width", targetID:"_app"}},
					{prop: "x", val:{percent:320 / 640, param:"width", targetID:"_app"}},
					{prop:"y", val:{hasStates: true,
						defaultEaseName: "easeOutSine",
						sIn:{percent:635 / 960, param:"height", targetID:"_app"},
						sOut:{add:[
							{percent:960 / 960, param:"height", targetID:"_app"},
							{percent:0.5, param:"height", targetID:"_self"}
						]}
					}}
				]
			},
			{
				id:"loaderKnob",
				params:[
					{prop: "_widthHeight", val:{percent:76 / 640, param:"width", targetID:"_app"}},
					{prop:"y", val:{hasStates: true,
						defaultEaseName: "easeOutSine",
						sIn:{percent:653 / 960, param:"height", targetID:"_app"},
						sOut:{add:[
							{percent:960 / 960, param:"height", targetID:"_app"},
							{percent:0.5, param:"height", targetID:"loaderBg"}
						]}
					}}
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
					cbcViewLayoutUpdateFinished: {callback: this.onViewLayoutUpdateFinished, context: this},
				};
				
				ret.viewCompound = this.viewCompound;
			}
		},
		
		onViewLayoutUpdateFinished: function() {
			this.syncKnobPosition();
		},
		
		createVisuals:function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.vElems = {};
			
			this.createVisuals_base(this.vElems);
		},

		createVisuals_base:function(parentVElems) {
			var vElemsLocal = parentVElems;
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			
			vElemsLocal.bg = this.app.popAsset(this.moduleBranch, "sprite", "viewPreloader/bg");
			vElemsLocal.container.addChild(vElemsLocal.bg);
			
			vElemsLocal.imgTitle = this.app.popAsset(this.moduleBranch, "sprite", "viewPreloader/title");
			vElemsLocal.imgTitle.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.imgTitle);
			
			vElemsLocal.loaderBg = this.app.popAsset(this.moduleBranch, "sprite", "viewPreloader/loaderBg");
			vElemsLocal.loaderBg.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.loaderBg);
			
			vElemsLocal.loaderKnob = this.app.popAsset(this.moduleBranch, "sprite", "viewPreloader/loaderKnob");
			vElemsLocal.loaderKnob.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.loaderKnob);
		},
		
		onAppLoopUserInterface:function(appTime, deltaMS, deltaS) {
			if(this.viewCompound && this.viewCompound.state.stateEnd.indexOf("sIn") == 0) {
				if(this.knobDirectionX > 0) {
					this.knobPercentPosX = Math.min(1, this.knobPercentPosX + this.cfg.knobPercentSpeedH * deltaS);
					
					if(this.knobPercentPosX >= 1) {
						this.knobDirectionX = -1;
					}
				}else {
					this.knobPercentPosX = Math.max(0, this.knobPercentPosX - this.cfg.knobPercentSpeedH * deltaS);
					
					if(this.knobPercentPosX <= 0) {
						this.knobDirectionX = 1;
					}
				}
				
				this.syncKnobPosition();
			}
		},
		
		syncKnobPosition: function() {
			var posLeft = this.app.appSize.width * this.cfg.knobLeftEdgeP;
			var posRight = this.app.appSize.width * this.cfg.knobRightEdgeP;
			
			var newX = posLeft + (posRight - posLeft) * this.knobPercentPosX;
			
			if(this.vElems.loaderKnob.x != newX) {
				this.vElems.loaderKnob.x = newX;
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
