/**
 * Created by Adrian on 11/21/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase"
], function(declare, ModuleBase){
	"use strict";
	return declare("app.main.ScreenSizeMonitor", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"initializeScreenMonitor", cbName:"onInitializeScreenMonitor", isUnique:false}
		],
		publishMsg: {
			screenSizeChange:{channel:"screenSizeChange", listenerType:"list", mustExist:false},
			
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},
		
		app:null,
		
		constructor: function() {
			//void
		},
		
		onInitializeScreenMonitor: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.app.screenSize = {
				width:Math.max(10, Number(window.innerWidth)),
            	height:Math.max(10, Number(window.innerHeight))
			};
			
			window.addEventListener("resize", this.onWindowResize.bind(this), false);
		},
		
		onWindowResize:function(event) {
			var newContainerWidth = Math.max(10, Number(window.innerWidth)),
            	newContainerHeight = Math.max(10, Number(window.innerHeight));
            	
            if(this.app.screenSize.width != newContainerWidth || this.app.screenSize.height != newContainerHeight) {
            	this.app.screenSize.width = newContainerWidth;
            	this.app.screenSize.height = newContainerHeight;
            	
            	this.publisher.publish(this.publishMsg.screenSizeChange);
            }
		},

		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}

	});
});
