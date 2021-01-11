/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase"
], function(declare, ModuleBase
    ){
	"use strict";
	return declare("app.horizontalhackandslash.GroundHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false},
			{channel:"gViewAreaChanged", cbName:"onGViewAreaChanged", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game: null,
		
		constructor: function() {
			//void
		},
		
		onGInitBaseComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
		},
		
		onGViewAreaChanged: function(worldArea) {
			//asd_002;
		},
		
		destroy: function() {
			//void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
