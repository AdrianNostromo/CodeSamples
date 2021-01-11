/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase"
], function(declare, ModuleBase
    ){
	"use strict";
	return declare("app.ingamea.GameLayers", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitLayers", cbName:"onGInitLayers", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game:null,
		
		constructor: function() {
			//void
		},
		
		onGInitLayers:function(rootContainer) {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.game.layers = {
				rootContainer:rootContainer
			};
			
			this.game.layers.rootContainer.addChild(this.game.layers.gameContent = this.app.popAsset(this.moduleBranch, "container"));
				this.game.layers.gameContent.addChild(this.game.layers.gameContentUnder = this.app.popAsset(this.moduleBranch, "container"));
				this.game.layers.gameContent.addChild(this.game.layers.pannableWorld = this.app.popAsset(this.moduleBranch, "container"));
					this.game.layers.pannableWorld.addChild(this.game.layers.grid = this.app.popAsset(this.moduleBranch, "container"));
						this.game.layers.grid.addChild(this.game.layers.gridBlocks = this.app.popAsset(this.moduleBranch, "container"));
						this.game.layers.grid.addChild(this.game.layers.gridBlockIndicator = this.app.popAsset(this.moduleBranch, "container"));
			
			this.game.layers.rootContainer.addChild(this.game.layers.userInterface = this.app.popAsset(this.moduleBranch, "container"));
				this.game.layers.userInterface.addChild(this.game.layers.vLayer_interfaceFx = this.app.popAsset(this.moduleBranch, "container"));
			
			this.game.layers.rootContainer.addChild(this.game.layers._fx_gameContent = this.app.popAsset(this.moduleBranch, "container"));
				this.game.layers._fx_gameContent.addChild(this.game.layers._fx_pannableWorld = this.app.popAsset(this.moduleBranch, "container"));
					this.game.layers._fx_pannableWorld.addChild(this.game.layers._fx_grid = this.app.popAsset(this.moduleBranch, "container"));
						this.game.layers._fx_grid.addChild(this.game.layers.vLayer_gridFxMain = this.app.popAsset(this.moduleBranch, "container"));
						this.game.layers._fx_grid.addChild(this.game.layers.vLayer_gridFxOver = this.app.popAsset(this.moduleBranch, "container"));
		},
		
		destroy: function() {
			if(this.game.layers) {
				this.app.cache.releaseVElems(this.moduleBranch, this.game.layers);
				
				this.game.layers = null;
			}
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
