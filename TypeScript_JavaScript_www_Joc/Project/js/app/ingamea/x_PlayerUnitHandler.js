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
	return declare("app.ingamea.PlayerUnitHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gCreatePlayerUnit", cbName:"onGCreatePlayerUnit", isUnique:false}
		],
		publishMsg: {
			gPlayerMinerUnitCreated:{channel:"gPlayerMinerUnitCreated", listenerType:"list", mustExist:false},
			gCreateEntity:{channel:"gCreateEntity", listenerType:"unique", mustExist:true},
			
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game: null,
		
		constructor: function() {
			//void
		},
		
		onGCreatePlayerUnit: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			// Create the player unit.
			var unitInfo = {
				type: EntityType.UNIT,
				id: this.game.extraData.playerUnitID,
				pos: {x: this.game.worldArea.width / 2, y: 0},
				visualLayerOverride: this.game.layers.playerUnits,
				logicProcessorOverride: new LogicProcessorPlayerInputA(this.game.playerInput)
			};
			this.game.playerEntity = this.publisher.publish(this.publishMsg.gCreateEntity, unitInfo);
			
			this.publisher.publish(this.publishMsg.gPlayerMinerUnitCreated, this.game.playerEntity);
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
