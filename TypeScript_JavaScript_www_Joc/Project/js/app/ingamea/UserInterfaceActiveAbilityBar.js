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
	return declare("app.ingamea.UserInterfaceActiveAbilityBar", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gShowPlayerAbilityBar", cbName:"onGShowPlayerAbilityBar", isUnique:false},
			{channel:"gHidePlayerAbilityBar", cbName:"onGHidePlayerAbilityBar", isUnique:false},
			
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
		
		extraData: null,
		reShowExtraData: null,
		
		vElems: null,
		
		isLevelEnded: false,
		
		cfg: {
			uiTransitionSpeedS: 2,
			leftBarLayoutLocalizedWidth: 242
		},
		
		vState: null,
		viewLayout: [
			{
				id:"activeAbilityBar",
				list:[
					{
						id:"container",
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
						params:[
							{prop:"width", val:{percent:640 / 640, param:"width", targetID:"_app"}},
							{prop:"height", val:{percent:106 / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"btnStopAbility",
						params:[
							{prop:"_widthHeight", val:{percent:200 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:144 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"icon_destroyBlock",
						params:[
							{prop:"_widthHeight", val:{percent:74 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:320 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"icon_paintBlock",
						params:[
							{prop:"_widthHeight", val:{percent:74 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:320 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"iconDiamonds",
						params:[
							{prop:"_widthHeight", val:{percent:74 / 640, param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:469 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2) / 960, param:"height", targetID:"_app"}}
						]
					},
					{
						id:"lblDiamondsCost",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {percent:96 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'WFDosis'"}]}},
							{prop:"x", val:{percent:528 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:(106 / 2) / 960, param:"height", targetID:"_app"}}
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
		},
		
		onGShowPlayerAbilityBar: function(extraData) {
			if(this.vState && this.vState.stateEnd.indexOf("sIn") == 0) {
				// Hide with a reShow command.
				this.onGHidePlayerAbilityBar(null, extraData);
				
				return ;
			}else if(this.vState && this.vState.stateEnd.indexOf("sOut") == 0 && this.vState.completionPercent < 1) {
				// Save the end command.
				this.reShowExtraData = extraData;
				
				return ;
			}
			
			this.extraData = extraData;
			
			if(this.vState.stateEnd != "sIn") {
				this.vState.stateStart = this.vState.stateEnd;
				this.vState.stateEnd = "sIn";
				
				this.vState.completionPercent = 1 - this.vState.completionPercent;
			}
			
			this.updateUIContent();
		},
		
		onGHidePlayerAbilityBar: function(playerAbilityTypeCheck, reShowExtraData) {
			if(!this.extraData) {
				this.postError("Err 6.");
				
				return null;
			}
			
			if(!playerAbilityTypeCheck || playerAbilityTypeCheck === this.extraData.playerAbilityID) {
				this.extraData = null;
				this.reShowExtraData = reShowExtraData;
				
				if(this.vState.stateEnd != "sOut") {
					this.vState.stateStart = this.vState.stateEnd;
					this.vState.stateEnd = "sOut";
					
					this.vState.completionPercent = 1 - this.vState.completionPercent;
				}
			}
		},
		
		updateUIContent: function() {
			// Activate the corect icon only.
			var iconsList = [
				this.vElems.activeAbilityBar.icon_destroyBlock,
				this.vElems.activeAbilityBar.icon_paintBlock
			];
			var activeIcon = null;
			
			if(this.extraData) {
				if(this.extraData.playerAbilityID == PlayerAbilityType.DESTROY_BLOCK) {
					activeIcon = this.vElems.activeAbilityBar.icon_destroyBlock;
				}else if(this.extraData.playerAbilityID == PlayerAbilityType.PAINT_BLOCK) {
					activeIcon = this.vElems.activeAbilityBar.icon_paintBlock;
				}else {
					this.postError("Err 6.");
					
					return null;
				}
			}
			
			for(var i=0;i<iconsList.length;i++) {
				var entry = iconsList[i];
				if(entry) {
					if(entry === activeIcon) {
						// Make it visible.
						
						if(!entry.visible) {
							entry.visible = true;
						}
					}else {
						// Make it hidden.
						
						if(entry.visible) {
							entry.visible = false;
						}
					}
				}
			}
		},
		
		onAppSizeChanged: function() {
			if(this.vElems && this.vElems.container.parent) {
				VisualUtil.updateLayout(this.vElems, this.viewLayout, this.app.appSize, this.game.layers.userInterface, true, this.vState, null);
			}
		},
		
		onGPersistentGameLoopUserInterface: function(gameTime, deltaMS, deltaS) {
			// Check if animation increment is needed.
			
			if(this.vState && this.vState.completionPercent < 1) {
				this.vState.completionPercent = Math.min(1, this.vState.completionPercent + this.cfg.uiTransitionSpeedS * deltaS);
				if(this.vState.stateEnd == "sOut" && this.vState.completionPercent >= 1 && this.reShowExtraData) {
					this.onGShowPlayerAbilityBar(this.reShowExtraData);
				}
				
				VisualUtil.updateLayout(this.vElems, this.viewLayout, this.app.appSize, this.game.layers.userInterface, true, this.vState, null);
			}
		},
		
		onGUnpausedStatusChanged: function(isUnpausedCounter) {
			if(this.isLevelEnded || this.game.levelEndReason) {
				return ;
			}
			
			if(!this.game.isUnpaused) {
				if(this.vState.stateEnd != "sOut") {
					this.vState.stateStart = this.vState.stateEnd;
					this.vState.stateEnd = "sOut";
					
					this.vState.completionPercent = 1 - this.vState.completionPercent;
				}
			}else if(this.extraData && this.extraData.playerAbilityID) {
				if(this.vState.stateEnd != "sIn") {
					this.vState.stateStart = this.vState.stateEnd;
					this.vState.stateEnd = "sIn";
					
					this.vState.completionPercent = 1 - this.vState.completionPercent;
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
			this.createVisuals_activeAbilityBar(this.vElems);
		},
		
		createVisuals_base:function(parentVElems) {
			var vElemsLocal = parentVElems;
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
		},
		
		createVisuals_activeAbilityBar: function(parentVElems) {
			var vElemsLocal = parentVElems.activeAbilityBar = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			vElemsLocal.bg = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plank1X");
			vElemsLocal.bg.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bg);
			
			vElemsLocal.btnStopAbility = this.app.popAsset(this.moduleBranch, "spriteButton", "inGame/ui/abilityBar/btnCancel_");
			vElemsLocal.btnStopAbility.anchor.set(0.5, 0.5);
			vElemsLocal.btnStopAbility.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnStopAbility);
			
			vElemsLocal.icon_destroyBlock = this.app.popAsset(this.moduleBranch, "sprite", "inGame/ui/ability/destroyBlock");
			vElemsLocal.icon_destroyBlock.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.icon_destroyBlock);
			
			vElemsLocal.icon_paintBlock = this.app.popAsset(this.moduleBranch, "sprite", "inGame/ui/ability/paintBlock");
			vElemsLocal.icon_paintBlock.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.icon_paintBlock);
			
			vElemsLocal.iconDiamonds = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/diamond_medium");
			vElemsLocal.iconDiamonds.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.iconDiamonds);
			
			vElemsLocal.lblDiamondsCost = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "1",
				style: {
					font: "800 96px 'WFDosis'",
					fill: "#FFFFFF",
					align: "left"
				}
			});
			vElemsLocal.lblDiamondsCost.anchor.set(0, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblDiamondsCost);
		},
		
		onBtnClick:function(event) {
			if(!this.game.isUnpaused || this.game.flags.iEnableHudUIInteraction < 1 || this.game.flags.levelEnded) {
				return ;
			}
			
			if(!this.vState || this.vState.stateEnd.indexOf("sIn") != 0 || this.vState.completionPercent < 1) {
				return ;
			}
			
			if(event.target === this.vElems.activeAbilityBar.btnStopAbility) {
				if(this.extraData && this.extraData.cbcStopAbility) {
					this.extraData.cbcStopAbility.callback.call(this.extraData.cbcStopAbility.context);
				}
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
