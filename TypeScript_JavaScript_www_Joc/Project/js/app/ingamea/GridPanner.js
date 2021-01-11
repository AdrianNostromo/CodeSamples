/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase"
], function(declare, ModuleBase
){
	"use strict";
	return declare("app.ingamea.GridPanner", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitDependantComponents", cbName:"onGInitDependantComponents", isUnique:false},
			{channel:"gPostWorldPrepare", cbName:"onGPostWorldPrepare", isUnique:false},
			{channel:"gBoxGridStructureChange", cbName:"onGBoxGridStructureChange", isUnique:false},
			{channel:"appSizeChanged",  cbName:"onAppSizeChanged", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game: null,
		
		vElems: null,
		
		cfg: {
			bottomUIPanelHeightPercent: 106 / 960,
			gridScaleMinGridWidth: 5
		},
		
		constructor: function() {
			//void
		},
		
		onGInitDependantComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.vElems = {};
			this.createVisuals_base();
		},
		
		onGPostWorldPrepare: function() {
			this.updateLayerScale();
			this.syncGridLayersPosition(false);
		},
		
		createVisuals_base: function() {
			var vElemsLocal = this.vElems;
			
			vElemsLocal.sideEdgeLeft = this.app.popAsset(this.moduleBranch, "sprite", "inGame/misc/sideGlow");
			vElemsLocal.sideEdgeLeft.anchor.set(0, 0);
			this.game.layers.gameContent.addChild(vElemsLocal.sideEdgeLeft);
			
			vElemsLocal.sideEdgeRight = this.app.popAsset(this.moduleBranch, "sprite", "inGame/misc/sideGlow");
			vElemsLocal.sideEdgeRight.anchor.set(0, 0);
			this.game.layers.gameContent.addChild(vElemsLocal.sideEdgeRight);
		},
		
		onAppSizeChanged: function() {
			this.updateLayerScale();
			this.syncGridLayersPosition(false);
		},
		
		onGBoxGridStructureChange: function(waveChanged, linesChanged) {
			if(waveChanged) {
				this.updateLayerScale();
			}
			
			if(waveChanged || linesChanged) {
				this.syncGridLayersPosition();
			}
		},
		
		updateLayerScale: function() {
			// start : update ui size;
			this.vElems.sideEdgeLeft.width = this.game.cfg.grid.sidePadding;
			this.vElems.sideEdgeLeft.height = this.app.appSize.height;
			
			this.vElems.sideEdgeRight.x = this.app.appSize.width;
			this.vElems.sideEdgeRight.width = -this.game.cfg.grid.sidePadding;
			this.vElems.sideEdgeRight.height = this.app.appSize.height;
			// end : update ui size;
			
			var gameAreaWidthForScale = Math.max(this.cfg.gridScaleMinGridWidth, this.game.waveTemplate.gridWidth) * this.game.cfg.grid.cellSize.width;
			var gameAreaWidthForCentering = this.game.waveTemplate.gridWidth * this.game.cfg.grid.cellSize.width;
			var n = (this.app.appSize.width - 2 * this.game.cfg.grid.sidePadding) / gameAreaWidthForScale;
			
			this.game.layers.grid.scale.set(n, n);
			this.game.layers._fx_grid.scale.set(n, n);
			
			this.game.layers.pannableWorld.x = (this.app.appSize.width - gameAreaWidthForCentering * n) / 2;
			this.game.layers._fx_pannableWorld.x = (this.app.appSize.width - gameAreaWidthForCentering * n) / 2;
		},
		
		syncGridLayersPosition: function() {
			var bottomPanelHeight = this.app.appSize.height * this.cfg.bottomUIPanelHeightPercent;
			
			this.game.layers.pannableWorld.y = this.app.appSize.height - bottomPanelHeight;
			this.game.layers._fx_pannableWorld.y = this.app.appSize.height - bottomPanelHeight;
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
