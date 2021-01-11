/**
 * Created by Adrian on 11/21/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	"lib/pixijs/pixi"
], function(declare, ModuleBase, PIXI){
	"use strict";
	return declare("app.main.AppLayers", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"initializeAppDependantComponents", cbName:"onInitializeAppDependantComponents", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		
		constructor: function() {
			//void
		},
		
		onInitializeAppDependantComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.app.layers = {};
			
			this.app.stage.addChild(this.app.layers.viewBackground = this.app.popAsset(this.moduleBranch, "container"));
			this.app.stage.addChild(this.app.layers.inGame = this.app.popAsset(this.moduleBranch, "container"));
			this.app.stage.addChild(this.app.layers.views = this.app.popAsset(this.moduleBranch, "container"));
			this.app.stage.addChild(this.app.layers.popupViews = this.app.popAsset(this.moduleBranch, "container"));
			this.app.stage.addChild(this.app.layers.viewSideBushes = this.app.popAsset(this.moduleBranch, "container"));
			this.app.stage.addChild(this.app.layers.visualFx = this.app.popAsset(this.moduleBranch, "container"));
			this.app.stage.addChild(this.app.layers.debugControlView = this.app.popAsset(this.moduleBranch, "container"));
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}

	});
});
