/**
 * Created by Adrian on 11/21/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase"
], function(declare, ModuleBase){
	"use strict";
	return declare("app.main.PreloaderHandler", ModuleBase, {
		moduleBranch:"global",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"initializePreloaderHandler", cbName:"onInitializePreloaderHandler", isUnique:true},
			{channel:"appAssetsLoaded", cbName:"onAppAssetsLoaded", isUnique:false}
		],
		publishMsg: {
		},
		
		preloaderControl:null,
		
		constructor: function() {
			//void
		},
		
		onInitializePreloaderHandler: function(preloaderControl) {
			this.preloaderControl = preloaderControl;
			
			this.preloaderControl.setCompletionPercent(0);
		},
		
		onAppAssetsLoaded:function() {
			if(this.preloaderControl) {
				this.preloaderControl.setCompletionPercent(1);
				
				this.preloaderControl = null;
			}
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}

	});
});
