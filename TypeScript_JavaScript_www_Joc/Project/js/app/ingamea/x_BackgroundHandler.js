/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase"
], function(declare, ModuleBase
    ){
	"use strict";
	return declare("app.ingamea.BackgroundHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false},
			{channel:"gViewAreaChanged", cbName:"onGViewAreaChanged", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game: null,
		
		bgID: null,
		bgConfig: null,
		bgLayers: null,
		
		cfg: {
			visualOverGrowAmount: 2// the visuals have 2 pixels overflow; +4px width, +4px height, -2px x, -2px y;
		},
		
		backgroundConfigs: {
			BgA:{
				layers:[
					{
						/*asd_005;// refactor the paralax mecahnism because the background is on the game pannable layer now.
						speedMultiplier: {
							x: 0,
							y: 0
						},*/
						visual:{
							id: "inGame/bg/BgA/ground",
							size: {
								wPercent: 0.5,
								hPercent: 0.5
							},
							anchor: {
								x: 0,
								y: 0
							},
							posY: 0
						}
					}
				]
			}
		},
		
		constructor: function() {
			//void
		},
		
		onGInitBaseComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.setBackground(this.game.levelConfig.backgroundID);
		},
		
		setBackground: function(bgID) {
			this.clearBackground();
			
			this.bgID = bgID;
			this.bgConfig = this.backgroundConfigs[bgID];
			
			this.bgLayers = [];
			for(var i=0;i<this.bgConfig.layers.length;i++) {
				var entry = this.bgConfig.layers[i];
				
				var layerEntry = {
					layerConfig: entry,
					visuals: [],
					localizedBounds: {
						left: 0,
						right: 0
					},
					visualsContainer: this.app.popAsset(this.moduleBranch, "container")
				};
				this.bgLayers[i] = layerEntry;
				//this.game.layers.staticBackground.addChild(layerEntry.visualsContainer);
			}
		},
		
		clearBackground: function() {
			if(this.bgID) {
				this.bgID = null;
				
				this.bgConfig = null;
				
				for(var i=0;i<this.bgLayers.length;i++) {
					var layerEntry = this.bgLayers[i];
					if(layerEntry) {
						this.bgLayers[i] = null;
						
						for(var j=layerEntry.visuals.length-1;j>=0;j--) {
							var visualEntry = layerEntry.visuals[j];
							
							if(visualEntry.parent) {
								visualEntry.parent.removeChild(visualEntry);
							}
							
							this.app.pushAsset(this.moduleBranch, visualEntry);
							
							layerEntry.visuals[j] = null;
							
							layerEntry.visuals.pop();
						}
						
						if(layerEntry.visualsContainer) {
							if(layerEntry.visualsContainer.parent) {
								layerEntry.visualsContainer.parent.removeChild(layerEntry.visualsContainer);
							}
							
							this.app.pushAsset(this.moduleBranch, layerEntry.visualsContainer);
							
							layerEntry.visualsContainer = null;
						}
					}
				}
				this.bgLayers = null;
			}
		},
		
		//asd_003;// refactor background when needed;
		isGenerated: false,
		onGViewAreaChanged: function(worldArea) {
			if(!this.bgLayers) {
				return ;
			}
			
			for(var i=0;i<this.bgLayers.length;i++) {
				var layerEntry = this.bgLayers[i];
				
				if(!this.isGenerated) {
					this.isGenerated = true;
					
					this.createBgImage(layerEntry, worldArea);
				}
				
				// Move the layer.
				/*asd_002;// refactor code because the bg layer is now on the pannable layer.
				if(layerEntry.layerConfig.speedMultiplier.x != 0) {
					layerEntry.visualsContainer.x = -worldArea.x * layerEntry.layerConfig.speedMultiplier.x;
				}
				
				if(layerEntry.layerConfig.speedMultiplier.y != 0) {
					layerEntry.visualsContainer.y = -worldArea.y * layerEntry.layerConfig.speedMultiplier.y;
				}*/
				
				// Remove hidden visuals.
				//asd_002; this.removeLayerHiddenVisuals(layerEntry, worldArea);
				/*
				// Create new visuals.
				if(worldArea.y <= 0) {
					if(layerEntry.visuals.length == 0) {
						layerEntry.localizedBounds.left = worldArea.x;
						
						layerEntry.localizedBounds.right = layerEntry.localizedBounds.left;
					}
					
					while(layerEntry.localizedBounds.left > worldArea.x) {
						// Create a visual on the left side.
						
						this.createNewLayerVisual(layerEntry, worldArea, -1);
					}
					while(layerEntry.localizedBounds.right < worldArea.x + worldArea.width) {
						// Create visual to the right.
						
						this.createNewLayerVisual(layerEntry, worldArea, 1);
					}
				}*/
			}
		},
		
		createBgImage: function(layerEntry, worldArea) {
			var visual = this.app.popAsset(this.moduleBranch, "sprite", layerEntry.layerConfig.visual.id);
			
			visual.anchor.set(0, 0);
			
			visual.position.x = 0;
			visual.position.y = 0;
			
			visual.width = worldArea.width;
			visual.height = worldArea.height;
			
			layerEntry.visualsContainer.addChild(visual);
			
			layerEntry.visuals.push(visual);
		},
		
		removeLayerHiddenVisuals: function(layerEntry, worldArea) {
			var newBounds_left;
			var newBounds_right;
			var isNewBounds = false;
			var isAnyRemoved = false;
			
			var visualEntry = layerEntry.visuals[i];
			for(var i=0;i<layerEntry.visuals.length;i++) {
				if(visualEntry) {
					if(visualEntry.x > worldArea.x + worldArea.width 
						|| visualEntry.x + visualEntry.width < worldArea.x
						|| worldArea.y > 0 // asd_00x; this is a hack, refactor when background regions are implemented or something changes;
					) {
						isAnyRemoved = true;
						
						if(visualEntry.parent) {
							visualEntry.parent.removeChild(visualEntry);
						}
						
						this.app.pushAsset(this.moduleBranch, visualEntry);
						
						if(layerEntry.visuals.length > 1 && i < layerEntry.visuals.length - 1) {
							layerEntry.visuals[i] = layerEntry.visuals[layerEntry.visuals.length - 1];
						}
						layerEntry.visuals.pop();
					}else {
						if(!isNewBounds) {
							isNewBounds = true;
							
							newBounds_left = visualEntry.x;
							newBounds_right = visualEntry.x + visualEntry.width;
						}else {
							newBounds_left = Math.min(layerEntry.localizedBounds.left, visualEntry.x);
							newBounds_right = Math.max(layerEntry.localizedBounds.right, visualEntry.x + visualEntry.width);
						}
					}
				}
			}
			
			if(isAnyRemoved && isNewBounds) {
				layerEntry.localizedBounds.left = newBounds_left + this.cfg.visualOverGrowAmount;
				layerEntry.localizedBounds.right = newBounds_right - this.cfg.visualOverGrowAmount;
			}
		},
		
		createNewLayerVisual: function(layerEntry, worldArea, direction) {
			var visual = this.app.popAsset(this.moduleBranch, "sprite", layerEntry.layerConfig.visual.id);
			
			visual.anchor.set(layerEntry.layerConfig.visual.anchor.x, layerEntry.layerConfig.visual.anchor.y);
			
			if("hwPercent" in layerEntry.layerConfig.visual.size) {
				visual.height = worldArea.height * layerEntry.layerConfig.visual.size.hwPercent;
				visual.scale.x = visual.scale.y;
			}else {
				visual.width = worldArea.width * layerEntry.layerConfig.visual.size.wPercent;
				visual.height = worldArea.height * layerEntry.layerConfig.visual.size.hPercent;
			}
			
			var posX;
			if(direction < 0) {
				// Create visual to the left;
				
				posX = layerEntry.localizedBounds.left - visual.width;
				
				layerEntry.localizedBounds.left = layerEntry.localizedBounds.left - visual.width + this.cfg.visualOverGrowAmount;
			}else {
				// Create visual to the right;
				
				posX = layerEntry.localizedBounds.right;
				
				layerEntry.localizedBounds.right = layerEntry.localizedBounds.right + visual.width - this.cfg.visualOverGrowAmount;
			}
			
			visual.x = posX;
			visual.y = layerEntry.layerConfig.visual.posY;
			
			layerEntry.visualsContainer.addChild(visual);
			
			layerEntry.visuals.push(visual);
		},
		
		destroy: function() {
			this.clearBackground();
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
