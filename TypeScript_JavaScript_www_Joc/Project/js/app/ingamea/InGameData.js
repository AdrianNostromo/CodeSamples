/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/PlayPhaseType",
	"lib/pixijs/pixi",
	"app/util/SafeNumber"
], function(declare, ModuleBase,
	PlayPhaseType, PIXI, SafeNumber
){
	"use strict";
	return declare("app.ingamea.InGameData", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gStarBlockCollected", cbName:"onGStarBlockCollected", isUnique:false},
			
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false}
		],
		publishMsg: {
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gCollectedStarsCountChanged:{channel:"gCollectedStarsCountChanged", listenerType:"any", mustExist:false}
		},
		
		game: null,
		
		constructor: function() {
			//void
		},
		
		onGInitBaseComponents: function() {
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.game.data = {
				snStarsCollected: new SafeNumber(0)
			};
		},
		
		onGStarBlockCollected: function(collectedStarGridPos) {
			this.game.data.snStarsCollected.setValue(this.game.data.snStarsCollected.getValue() + 1);
			
			this.publisher.publish(this.publishMsg.gCollectedStarsCountChanged, collectedStarGridPos);
		},
		
		destroy: function() {
			//void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
