/**
 * Created by Adrian on 11/21/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase"
], function(declare, ModuleBase){
	"use strict";
	return declare("app.main.AppSizeHandler", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"initializeAppSize", cbName:"onInitializeAppSize", isUnique:false},
			{channel:"screenSizeChange", cbName:"onScreenSizeChange", isUnique:false}
		],
		publishMsg: {
			appSizeChanged:{channel:"appSizeChanged", listenerType:"list", mustExist:false},
			
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},
		
		app:null,
		
		constructor: function() {
			//void
		},
		
		onInitializeAppSize: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			var currentArea = this.app.screenSize.width * this.app.screenSize.height;
			var standardDefinitionArea = 640 * 960;
			
			var areaRatio = currentArea / standardDefinitionArea;
			if(areaRatio <= 0.25) {
				this.app.assetsVariantName = "_LD";
			}else {
				this.app.assetsVariantName = "";
			}
			
			this.app.appSize = {
				width:this.app.screenSize.width,
            	height:this.app.screenSize.height
			};
			this.app.appRect = {
				x: 0,
				y: 0,
				width:this.app.screenSize.width,
            	height:this.app.screenSize.height
			};
		},
		
		onScreenSizeChange:function() {
			this.app.appSize.width = this.app.screenSize.width;
			this.app.appSize.height = this.app.screenSize.height;
			
			this.app.appRect.width = this.app.screenSize.width;
			this.app.appRect.height = this.app.screenSize.height;
			
			this.publisher.publish(this.publishMsg.appSizeChanged);
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}

	});
});
