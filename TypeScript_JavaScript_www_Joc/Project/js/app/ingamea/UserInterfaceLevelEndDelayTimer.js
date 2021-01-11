/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/util/VisualUtil",
	"app/main/view/util/ViewType"
], function(declare, ModuleBase,
	VisualUtil, ViewType
){
	"use strict";
	return declare("app.ingamea.UserInterfaceLevelEndDelayTimer", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gShowLevelEndDelayPanel", cbName:"onGShowLevelEndDelayPanel", isUnique:false},
			{channel:"gHideLevelEndDelayPanel", cbName:"onGHideLevelEndDelayPanel", isUnique:false},
			{channel:"appSizeChanged",  cbName:"onAppSizeChanged", isUnique:false},
			{channel:"gPersistentGameLoopUserInterface", cbName:"onGPersistentGameLoopUserInterface", isUnique:false},
			{channel:"gGameLoopGameElements", cbName:"onGGameLoopGameElements", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gLevelEndDelayTimerCloseRequest:{channel:"gLevelEndDelayTimerCloseRequest", listenerType:"any", mustExist:false}
		},
		
		app: null,
		game: null,
		
		timePassedMS: 0,
		timeTotalMS: 0,
		
		vElems: null,
		
		cfg: {
			uiTransitionSpeedS: 2
		},
		
		vState: null,
		viewLayout: [
			{
				id:"container",
				params:[
					{prop:"x", val:{percent:320 / 640, param:"width", targetID:"_app"}},
					{prop:"y", val:{hasStates: true,
						defaultEaseName: "easeOutSine",
						sIn:{percent:240 / 960, param:"height", targetID:"_app"},
						sOut:{percent:140 / 960, param:"height", targetID:"_app"}
					}},
					{prop:"alpha", val:{hasStates: true,
						defaultEaseName: "easeOutSine",
						sIn:1,
						sOut:0
					}}
				]
			},
			{
				id:"bg",
				params:[
					{prop:"_widthHeight", val:{percent:433 / 640, param:"width", targetID:"_app"}}
				]
			},
			{
				id:"basicOutline",
				params:[
					{prop:"_widthHeight", val:{percent:180 / 640, param:"width", targetID:"_app"}}
				]
			},
			{
				id:"timerFill",
				_refreshTextStyle: true,
				params:[
					// Scale can be used here because timerFill is a PIXI.Graphic.
					{prop:["scale", "x"], val:{percent:1 / 640, param:"width", targetID:"_app"}},
					{prop:["scale", "y"], val:{percent:1 / 960, param:"height", targetID:"_app"}}
				]
			},
			{
				id:"btnTheEnd",
				params:[
					{prop:"_widthHeight", val:{percent:155 / 640, param:"width", targetID:"_app"}}
				]
			}
		],
		
		constructor: function() {
			//void
		},
		
		onGShowLevelEndDelayPanel: function() {
			if(this.vState && this.vState.stateEnd.indexOf("sIn") == 0) {
				return ;
			}
			
			if(!this.app) {
				this.app = this.publisher.publish(this.publishMsg.getApp);
				this.game = this.publisher.publish(this.publishMsg.gGetGame);
			}
			
			if(!this.vElems) {
				this.createVisuals();
			}
			
			if(!this.vState) {
				this.vState = {stateStart: "sOut", stateEnd: "sIn", completionPercent: 0};
			}else {
				var s = this.vState.stateStart;
				this.vState.stateStart = this.vState.stateEnd;
				this.vState.stateEnd = s;
				this.vState.completionPercent = 1 - this.vState.completionPercent;
			}
			
			VisualUtil.updateLayout(this.vElems, this.viewLayout, this.app.appSize, this.game.layers.userInterface, true, this.vState, null);
			
			this.timePassedMS = 0;
			this.timeTotalMS = 3 * 1000;
			
			this.updateTimerBar();
		},
		
		onGHideLevelEndDelayPanel: function() {
			if(!this.vState || this.vState.stateEnd.indexOf("sOut") == 0) {
				return ;
			}
			
			var s = this.vState.stateStart;
			this.vState.stateStart = this.vState.stateEnd;
			this.vState.stateEnd = s;
			this.vState.completionPercent = 1 - this.vState.completionPercent;
		},
		
		onAppSizeChanged: function() {
			if(this.vElems && this.vElems.container.parent) {
				VisualUtil.updateLayout(this.vElems, this.viewLayout, this.app.appSize, this.game.layers.userInterface, true, this.vState, null);
			}
		},
		
		updateTimerBar: function() {
			var n = this.timePassedMS / this.timeTotalMS;
			
			var visual = this.vElems.timerFill;
			
			visual.clear();
			
			visual.beginFill(0xF9CA06, 1);
			visual.moveTo(0, 0);
			visual.drawCircle(0, 0, 77);
			visual.endFill();
			
			visual.beginFill(0xF9CA06, 1);
			visual.moveTo(0, 0);
			visual.arc(0, 0, 90, -Math.PI / 2 - Math.PI * n, -Math.PI / 2 + Math.PI * n, false);
			visual.endFill();
		},
		
		onGPersistentGameLoopUserInterface: function(gameTime, deltaMS, deltaS) {
			// Check if animation increment is needed.
			
			if(this.vState && this.vState.completionPercent < 1) {
				this.vState.completionPercent = Math.min(1, this.vState.completionPercent + this.cfg.uiTransitionSpeedS * deltaS);
				
				VisualUtil.updateLayout(this.vElems, this.viewLayout, this.app.appSize, this.game.layers.userInterface, true, this.vState, null);
			}
		},
		
		onGGameLoopGameElements: function(gameTime, deltaMS, deltaS) {
			if(this.vState && this.vState.stateEnd.indexOf("sIn") == 0) {
				this.timePassedMS = Math.min(this.timeTotalMS, this.timePassedMS + deltaMS);
				
				this.updateTimerBar();
				
				if(this.timePassedMS >= this.timeTotalMS) {
					this.publisher.publish(this.publishMsg.gLevelEndDelayTimerCloseRequest);
				}
			}
		},
		
		createVisuals:function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.vElems = {};
			
			this.createVisuals_base(this.vElems);
		},
		
		createVisuals_base:function(parentVElems) {
			var vElemsLocal = parentVElems;
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			
			vElemsLocal.bg = this.app.popAsset(this.moduleBranch, "sprite", "inGame/ui/endDelay/theEndGlow");
			vElemsLocal.bg.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.bg);
			
			vElemsLocal.basicOutline = this.app.popAsset(this.moduleBranch, "sprite", "inGame/ui/endDelay/basicOutline");
			vElemsLocal.basicOutline.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.basicOutline);
			
			vElemsLocal.timerFill = this.app.popAsset(this.moduleBranch, "graphics");
			vElemsLocal.container.addChild(vElemsLocal.timerFill);
			
			vElemsLocal.btnTheEnd = this.app.popAsset(this.moduleBranch, "spriteButton", "inGame/ui/endDelay/btnTheEnd_");
			vElemsLocal.btnTheEnd.anchor.set(0.5, 0.5);
			vElemsLocal.btnTheEnd.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnTheEnd);
		},
		
		onBtnClick:function(event) {
			if(event.target === this.vElems.btnTheEnd) {
				this.publisher.publish(this.publishMsg.gLevelEndDelayTimerCloseRequest);
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
