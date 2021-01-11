/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/util/VisualUtil",
	"app/main/view/util/ViewType",
	"app/ingamea/util/PlayerAbilityType"
], function(declare, ModuleBase,
	VisualUtil, ViewType, PlayerAbilityType
){
	"use strict";
	return declare("app.ingamea.UserInterfaceBottomBar", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitUserInterface", cbName:"onGInitUserInterface", isUnique:false},
			{channel:"appSizeChanged",  cbName:"onAppSizeChanged", isUnique:false},
			{channel:"gPersistentGameLoopUserInterface", cbName:"onGPersistentGameLoopUserInterface", isUnique:false},
			{channel:"gHideUserInterface", cbName:"onGHideUserInterface", isUnique:false},
			{channel:"gUnpausedStatusChanged", cbName:"onGUnpausedStatusChanged", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gIncrementUnpauseState:{channel:"gIncrementUnpauseState", listenerType:"any", mustExist:true},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false},
			gSetActivePlayerAbility:{channel:"gSetActivePlayerAbility", listenerType:"any", mustExist:false}
		},
		
		app: null,
		game: null,
		
		vElems: null,
		
		isLevelEnded: false,
		
		cfg: {
			uiTransitionSpeedS: 2,
			leftBarLayoutLocalizedWidth: 242
		},
		
		vState: null,
		viewLayout: [
			{
				id:"container",
				visual: {
					type: "container"
				}
			},
			{
				id:"bottomBar",
				list:[
					{
						id:"container",
						visual: {
							type: "container",
							parentID: ["_parent", "container"]
						},
						params:[
							{prop:"y", val:{hasStates: true,
								defaultEaseName: "easeOutSine",
								sIn:{percent:854 / 960, param:"height", targetID:"_app"},
								sOut:{percent:960 / 960, param:"height", targetID:"_app"}
							}}
						],
					},
					{
						id:"bg",
						visual: {
							type: "sprite",
							assetId: "inGame/ui/bottomBar/bg",
							parentID: "container"
						},
						params:[
							{prop:"width", val:{percent:640 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"btnPause",
						visual: {
							type: "spriteButton",
							assetId: "inGame/ui/bottomBar/btnPause_",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
							{prop: "cbcClick", val:{param:"CBC_CLICK", targetID:"_customValues"}}
						],
						params:[
							{prop:"_heightWidth", val:{percent:74 / 960, param:"height", targetID:"_app"}},
							{prop:"x", val:{percent:570 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:53 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"touch_extraTime",
						visual: {
							type: "sprite",
							assetId: "uiGeneral/touchDetector",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
							{prop: "cbcClick", val:{param:"CBC_CLICK", targetID:"_customValues"}}
						],
						params:[
							{prop:"width", val:{percent:126 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}},
							{prop:"x", val:{percent:(68 + 126 * 0) / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"icon_extraTime",
						visual: {
							type: "sprite",
							assetId: "inGame/ui/ability/extraTime",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5}
						],
						params:[
							{prop:"_widthHeight", val:{percent:74 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:(68 + 126 * 0) / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2 + 5) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"touch_randomizeColors",
						visual: {
							type: "sprite",
							assetId: "uiGeneral/touchDetector",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
							{prop: "cbcClick", val:{param:"CBC_CLICK", targetID:"_customValues"}}
						],
						params:[
							{prop:"width", val:{percent:126 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}},
							{prop:"x", val:{percent:(68 + 126 * 1) / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"icon_randomizeColors",
						visual: {
							type: "sprite",
							assetId: "inGame/ui/ability/randomizeColors",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5}
						],
						params:[
							{prop:"_widthHeight", val:{percent:74 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:(68 + 126 * 1) / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2 + 5) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"touch_destroyBlock",
						visual: {
							type: "sprite",
							assetId: "uiGeneral/touchDetector",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
							{prop: "cbcClick", val:{param:"CBC_CLICK", targetID:"_customValues"}}
						],
						params:[
							{prop:"width", val:{percent:126 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}},
							{prop:"x", val:{percent:(68 + 126 * 2) / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"icon_destroyBlock",
						visual: {
							type: "sprite",
							assetId: "inGame/ui/ability/destroyBlock",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5}
						],
						params:[
							{prop:"_widthHeight", val:{percent:74 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:(68 + 126 * 2) / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2 + 5) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"touch_paintBlock",
						visual: {
							type: "sprite",
							assetId: "uiGeneral/touchDetector",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5},
							{prop: "cbcClick", val:{param:"CBC_CLICK", targetID:"_customValues"}}
						],
						params:[
							{prop:"width", val:{percent:126 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}},
							{prop:"x", val:{percent:(68 + 126 * 3) / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"icon_paintBlock",
						visual: {
							type: "sprite",
							assetId: "inGame/ui/ability/paintBlock",
							parentID: "container"
						},
						initParams: [
							{prop: ["anchor", "x"], val: 0.5}, {prop: ["anchor", "y"], val: 0.5}
						],
						params:[
							{prop:"_widthHeight", val:{percent:74 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:(68 + 126 * 3) / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2 + 5) / 960, param:"height", targetID:"_app"}}
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
			
			this.updateLayout();
		},
		
		onAppSizeChanged: function() {
			if(this.vElems && this.vElems.container.parent) {
				this.updateLayout();
			}
		},
		
		onGPersistentGameLoopUserInterface: function(gameTime, deltaMS, deltaS) {
			// Check if animation increment is needed.
			
			if(this.vState && this.vState.completionPercent < 1) {
				this.vState.completionPercent = Math.min(1, this.vState.completionPercent + this.cfg.uiTransitionSpeedS * deltaS);
				
				this.updateLayout();
			}
		},
		
		updateLayout: function() {
			VisualUtil.updateLayout(this.vElems, this.viewLayout, this.app.appSize, this.game.layers.userInterface, true, this.vState, null);
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
			
			this.vElems = {isVContainer: true};
			
			VisualUtil.createVLayoutVisuals(this.vElems, this.viewLayout, this.app, this.moduleBranch, null, null, {CBC_CLICK: {callback: this.onBtnClick, context: this}});
			
			this.vElems.bottomBar.touch_extraTime.interactive = true;
			this.vElems.bottomBar.touch_extraTime.on('tap', this.onBtnClick.bind(this));
			
			this.vElems.bottomBar.touch_randomizeColors.interactive = true;
			this.vElems.bottomBar.touch_randomizeColors.on('tap', this.onBtnClick.bind(this));
			
			this.vElems.bottomBar.touch_destroyBlock.interactive = true;
			this.vElems.bottomBar.touch_destroyBlock.on('tap', this.onBtnClick.bind(this));
			
			this.vElems.bottomBar.touch_paintBlock.interactive = true;
			this.vElems.bottomBar.touch_paintBlock.on('tap', this.onBtnClick.bind(this));
		},
		
		onBtnClick:function(event) {
			if(this.game.flags.iEnableHudUIInteraction < 1 || this.game.flags.levelEnded) {
				return ;
			}
			
			if(event.target === this.vElems.bottomBar.touch_extraTime) {
				this.publisher.publish(this.publishMsg.gSetActivePlayerAbility, PlayerAbilityType.EXTRA_TIME);
			}else if(event.target === this.vElems.bottomBar.touch_randomizeColors) {
				this.publisher.publish(this.publishMsg.gSetActivePlayerAbility, PlayerAbilityType.RANDOMIZE_COLORS);
			}else if(event.target === this.vElems.bottomBar.touch_destroyBlock) {
				this.publisher.publish(this.publishMsg.gSetActivePlayerAbility, PlayerAbilityType.DESTROY_BLOCK);
			}else if(event.target === this.vElems.bottomBar.touch_paintBlock) {
				this.publisher.publish(this.publishMsg.gSetActivePlayerAbility, PlayerAbilityType.PAINT_BLOCK);
			}else if(event.target === this.vElems.bottomBar.btnPause) {
				if(this.game.isUnpaused) {
					this.publisher.publish(this.publishMsg.gIncrementUnpauseState, false);
					
					this.publisher.publish(this.publishMsg.changeViewState, ViewType.IN_GAME_PAUSED, "sIn", true, null, 0, null, null);
				}
			}else {
				this.postError("Err 6.");
				
				return null;
			}
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
