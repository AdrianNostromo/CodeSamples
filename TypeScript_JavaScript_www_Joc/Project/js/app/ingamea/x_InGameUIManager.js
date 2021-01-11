/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase"
], function(declare, ModuleBase
){
	"use strict";
	return declare("app.horizontalhackandslash.InGameUIManager", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gUnpausedStatusChanged", cbName:"onGUnpausedStatusChanged", isUnique:false},
			
			{channel:"appLoopInGameUI", cbName:"onAppLoopInGameUI", isUnique:true}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gUIShowStageChange:{channel:"gUIShowStageChange", listenerType:"list", mustExist:false}
		},
		
		app: null,
		game: null,
		
		isUIActive: false,
		stateChangeCompletionPercent: 1,
		
		cfg: {
			uiStateChangeSpeedS: 1
		},
		
		constructor: function() {
			//void
		},
		
		onGUnpausedStatusChanged: function(isUnpausedCounter) {
			if(!this.app) {
				this.app = this.publisher.publish(this.publishMsg.getApp);
				this.game = this.publisher.publish(this.publishMsg.gGetGame);
			}
			
			if(isUnpausedCounter >= 1 && !this.isUIActive) {
				this.isUIActive = true;
				
				this.stateChangeCompletionPercent = 0;
			}else if(isUnpausedCounter < 1 && this.isUIActive) {
				this.isUIActive = false;
				
				this.stateChangeCompletionPercent = 0;
			}
		},
		
		onAppLoopInGameUI: function(appTime, deltaMS, deltaS) {
			if(this.stateChangeCompletionPercent < 1) {
				this.stateChangeCompletionPercent = Math.min(1, this.stateChangeCompletionPercent + this.cfg.uiStateChangeSpeedS * deltaS);
				
				var stateStart;
				var stateEnd;
				if(this.isUIActive) {
					stateStart = "sOut";
					stateEnd = "sIn";
				}else {
					stateStart = "sIn";
					stateEnd = "sOut";
				}
				
				this.publisher.publish(this.publishMsg.gUIShowStageChange, stateStart, stateEnd, this.stateChangeCompletionPercent);
			}
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
