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
	return declare("app.ingamea.UserInterfaceTimerBar", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitUserInterface", cbName:"onGInitUserInterface", isUnique:false},
			{channel:"appSizeChanged",  cbName:"onAppSizeChanged", isUnique:false},
			{channel:"gPersistentGameLoopUserInterface", cbName:"onGPersistentGameLoopUserInterface", isUnique:false},
			{channel:"gUnpausedStatusChanged", cbName:"onGUnpausedStatusChanged", isUnique:false},
			{channel:"gHideUserInterface", cbName:"onGHideUserInterface", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gIncrementUnpauseState:{channel:"gIncrementUnpauseState", listenerType:"any", mustExist:true},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false}
		},
		
		app: null,
		game: null,
		
		lastLblTimerMS: 0,
		lastLblTimerS: 0,
		
		lastBarCompletionPercent: 0,
		lastArcCompletionPercent: 0,
		
		vElems: null,
		
		isLevelEnded: false,
		
		cfg: {
			uiTransitionSpeedS: 2,
			leftBarLayoutLocalizedWidth: 242
		},
		
		vState: null,
		viewLayout: [
			{
				id:"topBar",
				list:[
					{
						id:"container",
						params:[
							{prop:"y", val:{hasStates: true,
								defaultEaseName: "easeOutSine",
								sIn:{percent:0 / 960, param:"height", targetID:"_app"},
								sOut:{percent:-87 / 960, param:"height", targetID:"_app"},
								sInPaused:{percent:-87 / 960, param:"height", targetID:"_app"}
							}}
						],
					},
					{
						id:"bg",
						params:[
							{prop:"width", val:{percent:640 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:87 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"timerComposedFill",
						list:[
							{
								id:"container",
								params:[
									// Scale can be used because the object is a PIXI.Container, not a variable size image.
									{prop:["scale", "x"], val:{percent:1 / 640, param:"width", targetID:"_app"}},
									{prop:["scale", "y"], val:{percent:1 / 960, param:"height", targetID:"_app"}}
								],
							},
							{
								id:"leftBar",
								params:[
									{prop:"height", val:36}
								],
							},
							{
								id:"rightBar",
								params:[
									{prop:"height", val:36}
								],
							},
							{
								id:"leftArc",
								params:[
									{prop:"width", val:87},
									{prop:"height", val:87}
								],
							},
							{
								id:"rightArc",
								params:[
									{prop:"width", val:-87},
									{prop:"height", val:87}
								],
							}
						]
					},
					{
						id:"bottomShadow",
						params:[
							{prop:"width", val:{percent:640 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:87 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"lblTimeRemaining",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:77 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
							{prop:["style", "strokeThickness"], val:{percent:8 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:320 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:36 / 960, param:"height", targetID:"_app"}}
						]
					}
				]
			}
		],
		
		constructor: function() {
			//void
		},

		onGInitUserInterface: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.createVisuals();
			
			this.vState = {stateStart: "sIn", stateEnd: "sOut", completionPercent: 1};
			
			VisualUtil.updateLayout(this.vElems, this.viewLayout, this.app.appSize, this.game.layers.userInterface, true, this.vState, null);
			
			this.updateTimerVisuals(false);
		},
		
		onAppSizeChanged: function() {
			if(this.vElems && this.vElems.container.parent) {
				VisualUtil.updateLayout(this.vElems, this.viewLayout, this.app.appSize, this.game.layers.userInterface, true, this.vState, null);
			}
		},
		
		updateTimerVisuals: function(doAnimation) {
			var playTimeMS = this.game.data.playTimeMS;
			
			if(doAnimation && this.game.data.playTimeMS != this.lastLblTimerMS) {
				var diff = Math.abs(playTimeMS - this.lastLblTimerMS);
				var direction = diff / (playTimeMS - this.lastLblTimerMS);
				var finalDelta = Math.min(diff, Math.max(700, diff / 3));
				
				playTimeMS = this.lastLblTimerMS + finalDelta * direction;
			}
			this.lastLblTimerMS = playTimeMS;
			
			var playTimeSeconds = Math.ceil(playTimeMS / 1000);
			if(playTimeSeconds != this.lastLblTimerS) {
				this.vElems.topBar.lblTimeRemaining.text = playTimeSeconds;
				
				this.lastLblTimerS = playTimeSeconds;
			}
			
			var leftBar = this.vElems.topBar.timerComposedFill.leftBar;
			var rightBar = this.vElems.topBar.timerComposedFill.rightBar;
			var leftArc = this.vElems.topBar.timerComposedFill.leftArc;
			var rightArc = this.vElems.topBar.timerComposedFill.rightArc;
			
			var sideBarShowPercent;
			if(this.game.data.totalPlayTimeMS <= 5000) {
				sideBarShowPercent = 1;
			}else {
				sideBarShowPercent = 1 - Math.max(0, (this.game.data.playTimeMS - 5000) / (this.game.data.totalPlayTimeMS - 5000));
			}
			var circleMaskPercent = 1 - Math.min(this.game.data.playTimeMS, 5000) / 5000;
			
			if(doAnimation) {
				if(circleMaskPercent != this.lastArcCompletionPercent) {
					var diff = Math.abs(circleMaskPercent - this.lastArcCompletionPercent);
					var direction = diff / (circleMaskPercent - this.lastArcCompletionPercent);
					var finalDelta = Math.min(diff, 0.2);
					
					circleMaskPercent = this.lastArcCompletionPercent + finalDelta * direction;
				}
				
				if(circleMaskPercent < this.lastArcCompletionPercent) {
					sideBarShowPercent = this.lastBarCompletionPercent;
				}else if(sideBarShowPercent != this.lastBarCompletionPercent) {
					var diff = Math.abs(sideBarShowPercent - this.lastBarCompletionPercent);
					var direction = diff / (sideBarShowPercent - this.lastBarCompletionPercent);
					var finalDelta = Math.min(diff, Math.max(0.01, diff / 2));
					
					sideBarShowPercent = this.lastBarCompletionPercent + finalDelta * direction;
				}
			}
			
			this.lastBarCompletionPercent = sideBarShowPercent;
			this.lastArcCompletionPercent = circleMaskPercent;
			
			leftBar.width = this.cfg.leftBarLayoutLocalizedWidth * sideBarShowPercent;
			rightBar.width = -this.cfg.leftBarLayoutLocalizedWidth * sideBarShowPercent;
			
			leftArc.rotation = Math.PI / 2 - (Math.PI / 2) * circleMaskPercent;
			rightArc.rotation = - Math.PI / 2 + (Math.PI / 2) * circleMaskPercent;
		},
		
		onGPersistentGameLoopUserInterface: function(gameTime, deltaMS, deltaS) {
			// Check if animation increment is needed.
			
			if(this.vState && this.vState.completionPercent < 1) {
				this.vState.completionPercent = Math.min(1, this.vState.completionPercent + this.cfg.uiTransitionSpeedS * deltaS);
				
				VisualUtil.updateLayout(this.vElems, this.viewLayout, this.app.appSize, this.game.layers.userInterface, true, this.vState, null);
			}
			
			this.updateTimerVisuals(true);
		},
		
		onGUnpausedStatusChanged: function(isUnpausedCounter) {
			if(this.isLevelEnded || this.game.levelEndReason) {
				return ;
			}
			// Update the vState if needed.
			
			if(this.game.isUnpaused) {
				// Animate in.
				
				if(this.vState.stateEnd != "sIn") {
					this.vState.completionPercent = 1 - this.vState.completionPercent;
					
					this.vState.stateStart = this.vState.stateEnd;
					this.vState.stateEnd = "sIn";
				}
			}else {
				// Animate out to sInPaused.
				
				if(this.vState.stateEnd != "sInPaused") {
					this.vState.completionPercent = 1 - this.vState.completionPercent;
					
					this.vState.stateStart = this.vState.stateEnd;
					this.vState.stateEnd = "sInPaused";
				}
			}
		},
		
		onGHideUserInterface: function() {
			this.isLevelEnded = true;
			 
			if(this.vState.stateEnd != "sOut") {
				this.vState.stateStart = this.vState.stateEnd;
				this.vState.stateEnd = "sOut";
				
				this.vState.completionPercent = 1 - this.vState.completionPercent;
			}
		},
		
		createVisuals:function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.vElems = {};
			
			this.createVisuals_base(this.vElems);
			this.createVisuals_topBar(this.vElems);
		},
		
		createVisuals_base:function(parentVElems) {
			var vElemsLocal = parentVElems;
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
		},
		
		createVisuals_topBar: function(parentVElems) {
			var vElemsLocal = parentVElems.topBar = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			vElemsLocal.bg = this.app.popAsset(this.moduleBranch, "sprite", "inGame/ui/topBar/bg");
			vElemsLocal.bg.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bg);
			
			this.createVisuals_topBar_timerComposedFill(vElemsLocal);
			
			vElemsLocal.bottomShadow = this.app.popAsset(this.moduleBranch, "sprite", "inGame/ui/topBar/bottomShadow");
			vElemsLocal.bottomShadow.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bottomShadow);
			
			vElemsLocal.lblTimeRemaining = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "000",
				style: {
					font: "800 77px 'WFDosis'",
					fill: "#FFFFFF",
					align: "center",
					stroke: "#7F5B2B",
					strokeThickness: 8
				}
			});
			vElemsLocal.lblTimeRemaining.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblTimeRemaining);
		},
		
		createVisuals_topBar_timerComposedFill: function(parentVElems) {
			var vElemsLocal = parentVElems.timerComposedFill = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			vElemsLocal.leftBar = this.app.popAsset(this.moduleBranch, "sprite", "inGame/ui/topBar/fillLeftBar");
			vElemsLocal.leftBar.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.leftBar);
			
			vElemsLocal.rightBar = this.app.popAsset(this.moduleBranch, "sprite", "inGame/ui/topBar/fillLeftBar");
			vElemsLocal.rightBar.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.rightBar);
			
			vElemsLocal.leftArc = this.app.popAsset(this.moduleBranch, "sprite", "inGame/ui/topBar/fillLeftArc");
			vElemsLocal.leftArc.anchor.set(1, 0);
			vElemsLocal.container.addChild(vElemsLocal.leftArc);
			
			vElemsLocal.rightArc = this.app.popAsset(this.moduleBranch, "sprite", "inGame/ui/topBar/fillLeftArc");
			vElemsLocal.rightArc.anchor.set(1, 0);
			vElemsLocal.container.addChild(vElemsLocal.rightArc);
			
			// NOTE : The bar has limited vStyles applied, it needs some pre-initialization.
			vElemsLocal.rightBar.x = 640;
			vElemsLocal.leftArc.x = 320;
			vElemsLocal.rightArc.x = 320;
		},
		
		onBtnClick:function(event) {
			this.postError("Err 6.");
			
			return null;
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
