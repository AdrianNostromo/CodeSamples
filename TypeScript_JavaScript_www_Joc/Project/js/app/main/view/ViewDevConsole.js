/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/main/view/util/ViewType"
], function(declare, ModuleBase, 
		ViewType
	){
	"use strict";
	return declare("app.main.view.ViewDevConsole", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"postDevLog", cbName:"onPostDevLog", isUnique:true},
			
			{channel:"initializeView", cbName:"onInitializeView", isUnique:false},
			{channel:"appLoopUserInterface", cbName:"onAppLoopUserInterface", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp",  listenerType:"unique", mustExist:true},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false}
		},
		
		app: null,
		
		messagesCount: 0,
		
		cfg: {
		},
		
		viewID: ViewType.DEV_CONSOLE,
		viewCompound: null,
		
		vElems: null,
		
		viewConfig: {
		},
		
		viewLayout: [
			{
				id:"container",
				params:[
				]
			},
			{
				id:"panelConsoleOut",
				list:[
					{
						id:"container",
						params:[
						]
					},
					{
						id:"lblConsoleOut",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"200 "}, {percent:22 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}, {value:"px 'Arial'"}]}},
							{prop:["style", "strokeThickness"], val:{percent:8 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}},
							{prop:["style", "wordWrapWidth"], val:{percent:640 / 640, mathOperation: "ceil", param:"width", targetID:"_app"}},
							{prop:"x", val:{percent:0 / 640, param:"width", targetID:"_app"}},
							{prop:"y", val:{percent:0 / 960, param:"height", targetID:"_app"}}
						]
					}
				]
			}
		],
		
		constructor: function() {
			//void
		},
		
		onInitializeView: function(viewID, ret) {
			if(viewID === this.viewID) {
				this.createVisuals();
				
				this.viewCompound = {
					config: this.viewConfig,
					layout: this.viewLayout,
					vElems: this.vElems,
					parentLayer: this.app.layers.debugControlView,
					dependantViews: null,
					cbcViewLayoutUpdateFinished: null,
					cbcViewActivated: null
				};
				
				ret.viewCompound = this.viewCompound;
				
				this.onPostDevLog("Console active.");
			}
		},
		
		onPostDevLog: function(msg) {
			this.messagesCount++;
			this.vElems.panelConsoleOut.lblConsoleOut.text = "Dev console:\n" + this.messagesCount + " > " + msg;
			this.vElems.panelConsoleOut.lblConsoleOut.alpha = 1;
		},
		
		onAppLoopUserInterface:function(appTime, deltaMS, deltaS) {
			if(this.vElems.panelConsoleOut.lblConsoleOut.alpha > 0) {
				this.vElems.panelConsoleOut.lblConsoleOut.alpha = Math.max(0, this.vElems.panelConsoleOut.lblConsoleOut.alpha - 0.25 * deltaS);
			}
		},
		
		createVisuals:function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.vElems = {};
			
			this.createVisuals_base(this.vElems);
			this.createVisuals_panelConsoleOut(this.vElems);
		},
		
		createVisuals_base:function(parentVElems) {
			var vElemsLocal = parentVElems;
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
		},
		
		updateData: function(extraData) {
			//void
		},
		
		createVisuals_panelConsoleOut:function(parentVElems) {
			var vElemsLocal = parentVElems.panelConsoleOut = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			vElemsLocal.lblConsoleOut = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "",
				style: {
					font: "200 22px 'Arial'",
					fill: "#FFFFFF",
					align: "left",
					stroke: "#000000",
					strokeThickness: 8,
					wordWrap: true,
					wordWrapWidth: 640
				}
			});
			vElemsLocal.lblConsoleOut.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.lblConsoleOut);
		},
		
		onBtnClick:function(event) {
			this.postError("Err 6.");
			
			return null;
		},
		
		destroy: function() {
			if(this.vElems) {
				this.app.cache.releaseVElems(this.moduleBranch, this.vElems);
				
				this.vElems = null;
			}
			
			this.__proto__.__proto__.destroy.call(this);
		}

	});
});
