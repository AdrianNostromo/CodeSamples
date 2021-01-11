/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/EntityType",
	"app/ingamea/util/UnitID",
	"app/ingamea/entity/worldentity/entitymodulecontroller/LogicProcessorPlayerInputA"
], function(declare, ModuleBase,
	EntityType, UnitID, LogicProcessorPlayerInputA
){
	"use strict";
	return declare("app.ingamea.PlayerDataHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gPlayerCollectDiamonds", cbName:"onGPlayerCollectDiamonds", isUnique:true},
			
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false}
		],
		publishMsg: {
			gPlayerDataChanged:{channel:"gPlayerDataChanged", listenerType:"list", mustExist:false},
			
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		game: null,
		
		constructor: function() {
			//void
		},
		
		onGInitBaseComponents: function() {
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.game.playerData = {
				diamonds: 0
			};
		},
		
		onGPlayerCollectDiamonds: function(diamondsCount) {
			this.game.playerData.diamonds += diamondsCount;
			
			//asd_001;// the ui listens for this;
			this.publisher.publish(this.publishMsg.gPlayerDataChanged, this.game.playerData);
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
