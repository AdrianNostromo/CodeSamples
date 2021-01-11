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
	return declare("app.main.view.ViewSideBushes", ModuleBase, {
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
		
		viewID: ViewType.SIDE_BUSHES,
		viewCompound: null,
		
		vElems: null,
		
		cfg: {
		},
		
		viewConfig: {
		},
		
		viewLayout: [
			{
				id:"imgLeftBush",
				params:[
					{prop:"width", val:{percent:75 / 640, param:"width", targetID:"_app"}},
					{prop:"height", val:{percent:960 / 960, param:"height", targetID:"_app"}},
					{prop:"x", val:{hasStates: true,
						sIn:{percent:0 / 640, param:"width", targetID:"_app"},
						sInEaseName: "easeOutSine",
						sOut:{subtract:[
							{percent:0 / 640, param:"width", targetID:"_app"},
							{percent:1, param:"width", targetID:"_self"}
						]},
						sOutEaseName: "easeOutSine"
					}}
				]
			},
			{
				id:"imgRightBush",
				params:[
					{prop:"width", val:{percent:-75 / 640, param:"width", targetID:"_app"}},
					{prop:"height", val:{percent:960 / 960, param:"height", targetID:"_app"}},
					{prop:"x", val:{hasStates: true,
						sIn:{percent:640 / 640, param:"width", targetID:"_app"},
						sInEaseName: "easeOutSine",
						// Surbtract because the width is negative.
						sOut:{subtract:[
							{percent:640 / 640, param:"width", targetID:"_app"},
							{percent:1, param:"width", targetID:"_self"}
						]},
						sOutEaseName: "easeOutSine"
					}}
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
					parentLayer: this.app.layers.viewSideBushes,
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
			
			vElemsLocal.imgLeftBush = this.app.popAsset(this.moduleBranch, "sprite", "viewSideBushes/sideBush");
			vElemsLocal.imgLeftBush.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.imgLeftBush);
			
			vElemsLocal.imgRightBush = this.app.popAsset(this.moduleBranch, "sprite", "viewSideBushes/sideBush");
			vElemsLocal.imgRightBush.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.imgRightBush);
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
