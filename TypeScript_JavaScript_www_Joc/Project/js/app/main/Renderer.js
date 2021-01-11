/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	"lib/pixijs/pixi"
], function(declare, ModuleBase, PIXI){
	"use strict";
	return declare("app.main.Renderer", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"initializeRenderer", cbName:"onInitializeRenderer", isUnique:true},
			{channel:"screenSizeChange", cbName:"onScreenSizeChange", isUnique:false},
			{channel:"renderGameStage", cbName:"onRenderGameStage", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},
		
		app:null,
		
		constructor: function() {
			//void
		},
		
		onInitializeRenderer: function () {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			var options = {
			    view:this.app.appCanvas,
			    transparent:false,
        		backgroundColor: 0xffffff,
			    antialias:false,
			    preserveDrawingBuffer:false,// WebGL only;
			    resolution:1,
			    clearBeforeRender: true// Canvas only;
			};
			
			this.app.renderer = new PIXI.CanvasRenderer(this.app.screenSize.width, this.app.screenSize.height, options);
			// ASD_00x;// Use the canvas renderer for this game.
			// this.app.renderer = new PIXI.WebGLRenderer(this.app.screenSize.width, this.app.screenSize.height, options);
			
			this.app.stage = new PIXI.Container();
		},
		
		onScreenSizeChange:function() {
			this.app.renderer.resize(this.app.screenSize.width, this.app.screenSize.height);
		},
		
		onRenderGameStage: function() {
			this.app.renderer.render(this.app.stage);
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});