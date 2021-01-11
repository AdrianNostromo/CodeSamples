/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/util/VisualUtil"
], function(declare, ModuleBase,
	VisualUtil
){
	"use strict";
	return declare("app.horizontalhackandslash.TouchControllsUI", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitUserInterface", cbName:"onGInitUserInterface", isUnique:false},
			{channel:"gUIShowStageChange", cbName:"onGUIShowStageChange", isUnique:false},
			{channel:"appSizeChanged",  cbName:"onAppSizeChanged",  isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game: null,
		
		vElems: null,
		
		vState: null,
		
		vLayout: [
			{
				id:"imgBg",
				params:[
					{prop:"width", val:{wPercent:1, targetID:"_app"}},
					{prop:"height", val:{hPercent:1, targetID:"_app"}}
				]
			}/*asd_001,
			{
				id:"imgLogo",
				params:[
					{prop:"_wh", val:{wPercent:200 / 480, targetID:"_app"}},
					{prop:"x", val:{wPercent:240 / 480, targetID:"_app"}},
					{prop:"y", val:{hasStates: true,
						sIn:{hPercent:100 / 320, targetID:"_app"},
						sInEaseName: "easeOutSine",
						sOut:{subtract:[
							{hPercent:0 / 320, targetID:"_app"},
							{hPercent:0.5, targetID:"_self"}
						]},
						sOutEaseName: "easeOutSine"
					}}
				]
			},
			{
				id:"loaderBg",
				params:[
					{prop: "_wh", val:{wPercent:260 / 480, targetID:"_app"}},
					{prop: "x", val:{wPercent:240 / 480, targetID:"_app"}},
					{prop:"y", val:{hasStates: true,
						sIn:{hPercent:220 / 320, targetID:"_app"},
						sInEaseName: "easeOutSine",
						sOut:{add:[
							{hPercent:320 / 320, targetID:"_app"},
							{hPercent:0.5, targetID:"_self"}
						]},
						sOutEaseName: "easeOutSine"
					}}
				]
			}*/
		],
		
		constructor: function() {
			//void
		},
		
		onGInitUserInterface: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.createVisual();
			
			this.vState = {stateStart: "sOut", stateEnd: "sIn", completionPercent: 0};
			VisualUtil.updateLayout(this.vElems, this.vLayout, this.app.appSize, this.game.layers.userInterface, true, this.vState);
		},
		
		createVisual: function() {
			this.vElems = {};
			
			this.createVisuals_base();
			this.createVisuals_bottomLeft();
		},

		createVisuals_base:function() {
			var vElemsLocal = this.vElems;
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			
			vElemsLocal.touchIndicatorArrowLeft = this.app.popAsset(this.moduleBranch, "sprite", "touchControl/arrow_left");
			vElemsLocal.touchIndicatorArrowLeft.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.touchIndicatorArrowLeft);
			
			vElemsLocal.touchIndicatorArrowRight = this.app.popAsset(this.moduleBranch, "sprite", "touchControl/arrow_right");
			vElemsLocal.touchIndicatorArrowRight.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.touchIndicatorArrowRight);
			
			vElemsLocal.touchIndicatorHeroAvatarBase = this.app.popAsset(this.moduleBranch, "sprite", "touchControl/heroAvatar");
			vElemsLocal.touchIndicatorHeroAvatarBase.anchor.set(1, 1);
			vElemsLocal.container.addChild(vElemsLocal.touchIndicatorHeroAvatarBase);
		},
		
		onAppSizeChanged:function() {
			if(this.vElems && this.vElems.container.parent) {
				VisualUtil.updateLayout(this.vElems, this.vLayout, this.app.appSize, this.game.layers.userInterface, true, this.vState);
			}
		},
		
		onGUIShowStageChange: function(stateStart, stateEnd, completionPercent) {
			this.vState = {stateStart: stateStart, stateEnd: stateEnd, completionPercent: completionPercent};
			VisualUtil.updateLayout(this.vElems, this.vLayout, this.app.appSize, this.game.layers.userInterface, false, this.vState);
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
