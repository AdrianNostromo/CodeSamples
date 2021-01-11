/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/main/view/util/ViewType",
	"app/util/TimeUltil",
	"lib/pixijs/pixi",
	"app/util/VisualUtil",
	"app/util/tween/EaseFunctions"
], function(declare, ModuleBase, 
		ViewType, TimeUltil, PIXI, VisualUtil, EaseFunctions
	){
	"use strict";
	return declare("app.main.view.ViewBackground", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"initializeView", cbName:"onInitializeView", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp",  listenerType:"unique", mustExist:true},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false},
			activateInGame:{channel:"activateInGame", listenerType:"list", mustExist:false}
		},
		
		app: null,
		
		viewID: ViewType.BACKGROUND,
		viewCompound: null,
		
		vElems: null,
		
		cfg: {
		},
		
		viewConfig: {
		},
		
		viewLayout: [
			{
				id:"bg",
				params:[
					{prop:"width", val:{percent:640 / 640, param:"width", targetID:"_app"}},
					{prop:"height", val:{percent:960 / 960, param:"height", targetID:"_app"}}
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
					parentLayer: this.app.layers.viewBackground,
					cbcViewLayoutUpdateFinished: null,
					cbcViewActivated: null
				};
				
				ret.viewCompound = this.viewCompound;
			}
		},
		
		createVisuals:function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.vElems = {};
			
			this.createVisuals_base();
		},
		
		createVisuals_base:function() {
			var vElemsLocal = this.vElems;
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			
			vElemsLocal.bg = this.app.popAsset(this.moduleBranch, "sprite", "viewBackground/bgA");
			vElemsLocal.bg.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bg);
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
