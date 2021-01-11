/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/EntityType",
	"app/main/gamelevel/util/EntityVisualType",
	"app/horizontalhackandslash/objects/WorldEntity",
	"app/horizontalhackandslash/objects/worldentity/EntityModuleVisual_Spine",
	"lib/pixijs/pixi",
	
	"app/util/MathUtil",
	"app/horizontalhackandslash/roomhandler/RoomTile"
], function(declare, ModuleBase,
	EntityType, EntityVisualType, WorldEntity, EntityModuleVisual_Spine, PIXI,
	MathUtil, RoomTile
){
	"use strict";
	return declare("app.horizontalhackandslash.RoomHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false},
			{channel:"gActivateRoom", cbName:"onGActivateRoom", isUnique:true}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game: null,
		
		roomElements: null,
		roomID: null,
		roomTemplate: null,
		
		roomTeplatesList: {
			City_1:{
				size: {
					width: 700,
					height: 320
				},
				exitZones: [
					{
						targetRoom: {
							offset:{
								x: +1,
								y: 0
							},
							targetDoor: {
								x: 0,
								y: -1
							}
						},
						shape: "BOX",
						pos:{
							x: 400,
							y: 50
						},
						size: {
							width: 200,
							height: 100
						},
						rotD: 0
					},
				],
				collissionBoxes:[
					{
						shape: "BOX",
						pos:{
							x: 0,
							y: 0
						},
						size: {
							width: 200,
							height: 100
						},
						rotD: 0
					},
					{
						shape: "BOX",
						pos:{
							x: 0,
							y: 200
						},
						size: {
							width: 100,
							height: 50
						},
						rotD: 0
					}
				],
				layers:[
					{
						type: "CONTENT",
						name: "Content_layer",
						objects: [
							{
								type: "TILE",
								id: "zone_dungeon/tiles/tile13",
								pos: {
									x: 0,
									y: 0
								},
								size: {
									width: 128,
									height: 64
								}
							},
							{
								type: "TILE",
								id: "zone_dungeon/tiles/tile13",
								pos: {
									x: 0,
									y: 128
								},
								size: {
									width: 128,
									height: 64
								}
							}
						]
					},
					{
						type: "CONTENT",
						name: "Content_layer",
						objects: [
							{
								type: "TILE",
								id: "zone_dungeon/tiles/tile16",
								pos: {
									x: 128,
									y: 0
								},
								size: {
									width: 128,
									height: 64
								}
							},
							{
								type: "TILE",
								id: "zone_dungeon/tiles/tile16",
								pos: {
									x: 128,
									y: 128
								},
								size: {
									width: 128,
									height: 64
								}
							}
						]
					},
					{
						type: "UNITS",
						name: "Units_holder"
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
		},
		
		onGActivateRoom: function(roomID, doorX, doorY) {
			if(this.roomID) {
				this.disposeActiveRoom();
			}
			
			// Get the room teplate.
			this.roomTemplate = this.roomTeplatesList[roomID];
			if(!this.roomTemplate) {
				this.postError("err 6");
				
				return ;
			}
			
			this.roomID = roomID;
			
			this.roomElements = {
				objects: [],
				layerContainers: [],
				unitsLayer: null
			};
			for(var i=0;i<this.roomTemplate.layers.length;i++) {
				var roomLayer = this.roomTemplate.layers[i];
				
				var layerContainer = this.app.popAsset(this.moduleBranch, "container");
				this.game.layers.roomZone.addChild(this.roomElements.layerContainers[i] = layerContainer);
				
				if(roomLayer.type === "CONTENT") {
					this.createLayerObjects(roomLayer, layerContainer);
				}else if(roomLayer.type === "UNITS") {
					this.roomElements.unitsLayer = layerContainer;
				}
			}
		},
		
		createLayerObjects: function(roomLayer, layerContainer) {
			for(var i=0;i<roomLayer.objects.length;i++) {
				var objectConfig = roomLayer.objects[i];
				if(objectConfig.type === "TILE") {
					var object = this.createNewTile(objectConfig, layerContainer);
				}else {
					this.postError("err 6");
					
					return ;
				}
			}
		},
		
		createNewTile: function(objectConfig, layerContainer) {
			var visual = this.app.popAsset(this.moduleBranch, "sprite", objectConfig.id);
			visual.width = objectConfig.size.width;
			visual.height = objectConfig.size.height;
			visual.x = objectConfig.pos.x;
			visual.y = objectConfig.pos.y;
			visual.rotation = MathUtil.degreesToRadians(objectConfig.rotD);
			
			var object = new RoomTile({rootVisual: visual}, objectConfig, layerContainer);
			
			layerContainer.addChild(object.vElems.rootVisual);
			
			return object;
		},
		
		disposeActiveRoom: function() {
			//asd_002
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
