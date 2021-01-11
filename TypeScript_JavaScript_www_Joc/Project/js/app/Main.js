
/**
 * Created by Adrian on 11/18/2014.
 */

define([
	"dojo/_base/declare",
	"app/Mediator",
	
	"app/main/AppController",
	"app/main/LogWarningErrorHandler",
	"app/main/ScreenSizeMonitor",
	"app/main/AppSizeHandler",
	"app/main/Renderer",
	
	"app/main/VisualAssetsCache",
	"app/main/ObjectCache",
	"app/main/AppLayers",
	"app/main/AssetsLoader",
	"app/main/AppLoop",
	
	"app/main/ViewController",
	"app/main/AudioManager",
	"app/main/PlayerProfileHandler",
	"app/main/VisualFXHandler"
	
	// non-exported libs;
], function(declare, Mediator,
		AppController, LogWarningErrorHandler, ScreenSizeMonitor, AppSizeHandler, Renderer, 
		VisualAssetsCache, ObjectCache, AppLayers, AssetsLoader, AppLoop, 
		ViewController, AudioManager, PlayerProfileHandler, VisualFXHandler
	){
	"use strict";
	return declare("app.Main", null, {
		mediator: null,
		
		publishMsg: {
			mCreateModulesList:{channel:"mCreateModulesList", listenerType:"unique", mustExist:true},
			initializeApp:{channel:"initializeApp", listenerType:"unique", mustExist:true}
		},
		
		constructor: function() {
			//void
		},
		
		startApp: function(appCanvas) {
			this.mediator = new Mediator();
			
			this.mediator.publish(this.publishMsg.mCreateModulesList, [
				AppController,
				LogWarningErrorHandler,
				ScreenSizeMonitor,
				AppSizeHandler,
				Renderer,
				
				VisualAssetsCache,
				ObjectCache,
				AppLayers,
				AssetsLoader,
				AppLoop,
				
				ViewController,
				AudioManager,
				PlayerProfileHandler,
				VisualFXHandler
			]);
			
			this.mediator.publish(this.publishMsg.initializeApp, appCanvas);
		}
	});
});
