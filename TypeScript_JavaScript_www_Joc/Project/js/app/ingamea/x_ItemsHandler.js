/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/TilesLayerID",
	"app/ingamea/util/TileID",
	"app/ingamea/entity/WorldItem",
	"app/util/PhysicsUtil",
	"app/util/MathUtil",
	
	"app/ingamea/util/WorldItemType",
	"lib/pixijs/pixi"
], function(declare, ModuleBase,
	TilesLayerID, TileID, WorldItem, PhysicsUtil, MathUtil,
	WorldItemType, PIXI
){
	"use strict";
	return declare("app.ingamea.ItemsHandler", ModuleBase, {
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
		
		itemsList: null,
		generationBoundsTop: 0,
		
		itemsConfigs: {
			DIAMOND: {
				visual: {
					id: "inGame/items/Diamond",
					anchor: {x: 0.5, y: 0.5},
					baseSize: {width: 79, height: 79},
					scale: {x: 1, y: 1}
				},
				physics: {
					fixedRotation: true,
					mass: 0,
					type: p2.Body.STATIC,
					shapes: {
						main: {
							id: "main",
							shape: "BOX",
							pSize: {width: 1, height: 1},
							offsetP: {x: 0, y: 0},
							angR: 0,
							sensor: true,
							collissionFilterID: "ITEM"
						}
					}
				}
			},
			BODY_SEGMENT: {
				visual: {
					id: "inGame/items/BodyPart",
					anchor: {x: 0.5, y: 0.5},
					baseSize: {width: 40, height: 80},
					scale: {x: 1, y: 1}
				},
				physics: {
					fixedRotation: true,
					mass: 0,
					type: p2.Body.STATIC,
					shapes: {
						main: {
							id: "main",
							shape: "BOX",
							pSize: {width: 1, height: 1},
							offsetP: {x: 0, y: 0},
							angR: 0,
							sensor: true,
							collissionFilterID: "ITEM"
						}
					}
				}
			}
		},
		
		constructor: function() {
			//void
		},
		
		onGInitBaseComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.itemsList = [];
		},
		
		onGViewAreaChanged: function(worldArea) {
			// Remove under items.
			this.removeOffscreenUnderItems();
			
			// Create new upper items.
			if(this.generationBoundsTop > worldArea.y + worldArea.height) {
				this.generationBoundsTop = worldArea.y + worldArea.height;
			}
			
			while(this.generationBoundsTop > worldArea.y) {
				this.generationBoundsTop = this.createNewItem(-1, worldArea, this.generationBoundsTop);
			}
		},
		
		removeOffscreenUnderItems: function() {
			if(this.itemsList) {
				for(var i=0;i<this.itemsList.length;i++) {
					var entry = this.itemsList[i];
					
					if(entry.vElems.rootVisual.y - Math.abs(entry.vElems.rootVisual.height) / 2 > this.game.worldArea.y + this.game.worldArea.height) {
						// Entry is under screen, remove it.
						
						// Recycle the visuals.
						this.recycleEntry(entry);
						
						if(this.itemsList.length > 1 && i < this.itemsList.length - 1) {
							this.itemsList[i] = this.itemsList[this.itemsList.length - 1];
							
							i--;
						}
						this.itemsList.pop();
					}
				}
			}
		},
		
		createNewItem: function(screenSide, worldArea, generationBoundsTop) {
			var shapesList = [WorldItemType.DIAMOND, WorldItemType.BODY_SEGMENT];
			var itemType = shapesList[MathUtil.randomInRangeInt(0, shapesList.length - 1)];
			var entryConfig = this.itemsConfigs[itemType];
			
			var visual = this.app.popAsset(this.moduleBranch, "sprite", entryConfig.visual.id);
			visual.anchor.set(entryConfig.visual.anchor.x, entryConfig.visual.anchor.y);
			visual.width = entryConfig.visual.baseSize.width * entryConfig.visual.scale.x;
			visual.height = entryConfig.visual.baseSize.height * entryConfig.visual.scale.y;
			
			visual.rotation = 0;
			
			var posX = worldArea.width / 2;
			var posY = generationBoundsTop - Math.abs(visual.height) / 2;
			visual.position.set(posX, posY);
			
			var vElems = {
				rootVisual: visual
			};
			
			var pElems = {};
			
			var debugVElems = null;
			if(this.app.dev.debugPhysicsVisuals) {
				debugVElems = {};
			}
			
			if(!PhysicsUtil.generatePhysicsFromConfig(this.app, this.moduleBranch, entryConfig.physics, pElems, debugVElems, {width: visual.width, height: visual.height}, {x: posX, y: posY}, visual.rotation)) {
				this.postError("err 6");
    			
    			return false;
			}
			
			var newWall = new WorldItem(itemType, entryConfig, vElems, pElems, debugVElems, {callback: this.onItemCollected, context: this});
			this.itemsList.push(newWall);
			
			pElems.mainBody.userData = {
				worldEntity: newWall
			};
			
			this.game.pWorld.addBody(pElems.mainBody);
			
			this.game.layers.items.addChild(visual);
			
			if(debugVElems) {
				this.game.layers.physicsDebug.addChild(debugVElems.rootVisual);
			}
			
			return generationBoundsTop - Math.abs(visual.height) - 150 - 150 * Math.random();
		},
		
		onItemCollected: function(item) {
			// Remove the item from the list and recycle it.
			var i = this.itemsList.indexOf(item);
			if(i < 0) {
				this.postError("err 6");
				
				return ;
			}
			
			item.isActive = false;
			
			this.recycleEntry(item);
			
			if(this.itemsList.length > 1 && i < this.itemsList.length - 1) {
				this.itemsList[i] = this.itemsList[this.itemsList.length - 1];
			}
			this.itemsList.pop();
		},
		
		recycleEntry: function(entry) {
			if(entry.vElems) {
				this.app.cache.releaseVElems(this.moduleBranch, entry.vElems);
				
				entry.vElems = null;
			}
			if(entry.debugVElems) {
				this.app.cache.releaseVElems(this.moduleBranch, entry.debugVElems);
				
				entry.debugVElems = null;
			}
		},
		
		destroy: function() {
			//asd_005;// remove the tiles assets;
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
