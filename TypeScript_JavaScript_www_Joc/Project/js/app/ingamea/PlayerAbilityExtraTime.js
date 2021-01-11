/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/BlockType",
	"app/ingamea/util/SimpleBlockVersion",
	"app/ingamea/util/EntityVisualType",
	"app/util/VisualUtil",
	"app/util/MathUtil",
	
	"app/ingamea/util/PlayerAbilityType",
	"app/main/view/util/ViewType",
	"app/main/util/PopupMessageType"
], function(declare, ModuleBase,
	BlockType, SimpleBlockVersion, EntityVisualType, VisualUtil, MathUtil, 
	PlayerAbilityType, ViewType, PopupMessageType
){
	"use strict";
	return declare("app.ingamea.PlayerAbilityExtraTime", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitDependantComponents", cbName:"onGInitDependantComponents", isUnique:false},
			{channel:"gSetActivePlayerAbility", cbName:"onGSetActivePlayerAbility", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false},
			gAddExtraPlayTime:{channel:"gAddExtraPlayTime", listenerType:"any", mustExist:false},
			gIncrementUnpauseState:{channel:"gIncrementUnpauseState", listenerType:"any", mustExist:true},
			activatePopup:{channel:"activatePopup", listenerType:"any", mustExist:false}
		},
		
		app: null,
		game: null,
		
		cfg: {
		},
		
		constructor: function() {
			//void
		},
		
		onGInitDependantComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
		},
		
		onGSetActivePlayerAbility: function(playerAbilityType) {
			if(playerAbilityType != PlayerAbilityType.EXTRA_TIME) {
				return ;
			}
			
			if(this.app.profileData.getDiamonds() < 1) {
				// Not enough diamonds.
				
				this.publisher.publish(this.publishMsg.activatePopup, PopupMessageType.INSUFFICIENT_DIAMONDS_FOR_ACTION, true);
				
				this.publisher.publish(this.publishMsg.gIncrementUnpauseState, false);
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.SHOP, "sIn", true, {
					showExtraBackground: true,
					cbcViewCloseAnimationFinish: {callback: this.onShopViewClose,  context: this}
				}, 0, null, null);
			}else {
				// Remove 1 diamond from the player.
				this.app.profileData.addDiamonds(-1);
				
				// Show a fx for "-1 Diamond" lost.
				var fxPos = {
					x: this.app.appSize.width * (68 / 640),
					y: this.app.appSize.height * ((907 - 60) / 960)
				};
				this.app.visualFx.activateFxTemplate("diamondSpentOnUserInterface", 0, fxPos, this.moduleBranch, null, ViewType.IN_GAME);
				
				// Give the player a extra 15 seconds and pause the timer.
				var tm = this.game.data.playTimeMS + 15 * 1000;
				tm = Math.ceil(tm / 1000) * 1000 + 1;
				this.publisher.publish(this.publishMsg.gAddExtraPlayTime, tm - this.game.data.playTimeMS, true);
			}
		},
		
		onShopViewClose: function() {
			this.publisher.publish(this.publishMsg.gIncrementUnpauseState, true);
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
