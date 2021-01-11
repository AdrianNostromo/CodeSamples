/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/EntityType",
	"app/ingamea/util/BuildingID"
], function(declare, ModuleBase,
	EntityType, BuildingID
){
	"use strict";
	return declare("app.ingamea.BuildingsHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitPersistentContent", cbName:"onGInitPersistentContent", isUnique:false}
		],
		publishMsg: {
			gCreateEntity:{channel:"gCreateEntity", listenerType:"unique", mustExist:true},
			
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game: null,
		
		constructor: function() {
			//void
		},
		
		onGInitPersistentContent: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			var unitInfo = {
				type: EntityType.BUILDING,
				id: BuildingID.UpgradesA,
				pos: {x: 300, y: 0}
			};
			var entity = this.publisher.publish(this.publishMsg.gCreateEntity, unitInfo);
			
			unitInfo = {
				type: EntityType.BUILDING,
				id: BuildingID.RefineryA,
				pos: {x: 600, y: 0}
			};
			entity = this.publisher.publish(this.publishMsg.gCreateEntity, unitInfo);
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
