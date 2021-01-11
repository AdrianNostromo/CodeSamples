/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/TilesLayerID",
	"app/ingamea/util/TileID",
	"app/ingamea/entity/WallPiece",
	"app/util/PhysicsUtil",
	"app/util/MathUtil",
	
	"lib/pixijs/pixi",
	"app/ingamea/util/CollisionFilters",
	
	// Global libs.
	"lib/p2js/p2"
], function(declare, ModuleBase,
	TilesLayerID, TileID, WallPiece, PhysicsUtil, MathUtil,
	PIXI, CollisionFilters
){
	"use strict";
	return declare("app.ingamea.WallsHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitPersistentContent", cbName:"onGInitPersistentContent", isUnique:false},
			{channel:"gViewAreaChanged", cbName:"onGViewAreaChanged", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game: null,
		
		wallsList: null,
		leftGenerationBoundsTop: -700,
		rightGenerationBoundsTop: -500,
		
		wallPieceConfigs: {
			ZoneA: {
				Box1: {
					visual: {
						id: "inGame/wallPieces/ZoneA/Box1",
						anchor: {x: 0.5, y: 0.5},
						baseSize: {width: 213, height: 213},
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
								collissionFilterID: "WALL"
							}
						}
					}
				},
				TriangleIsoscelerRight1: {
					visual: {
						id: "inGame/wallPieces/ZoneA/TriangleIsoscelerRight1",
						anchor: {x: 0, y: 0.5},
						baseSize: {width: 150, height: 301},
						scale: {x: 1, y: 1}
					},
					physics: {
						fixedRotation: true,
						mass: 0,
						type: p2.Body.STATIC,
						shapes: {
							main: {
								id: "main",
								shape: "TRIANGLE_ISOSCELES",
								orientation: "RIGHT",
								pSize: {width: 1, height: 1},
								offsetP: {x: 0, y: 0},
								angR: 0,
								sensor: true,
								collissionFilterID: "WALL"
							}
						}
					}
				},
				TriangleIsoscelerLeft1: {
					visual: {
						id: "inGame/wallPieces/ZoneA/TriangleIsoscelerRight1",
						anchor: {x: 0, y: 0.5},
						baseSize: {width: 150, height: 301},
						scale: {x: -1, y: 1}
					},
					physics: {
						fixedRotation: true,
						mass: 0,
						type: p2.Body.STATIC,
						shapes: {
							main: {
								id: "main",
								shape: "TRIANGLE_ISOSCELES",
								orientation: "LEFT",
								pSize: {width: 1, height: 1},
								offsetP: {x: 0, y: 0},
								angR: 0,
								sensor: true,
								collissionFilterID: "WALL"
							}
						}
					}
				},
				Rhombus1: {
					visual: {
						id: "inGame/wallPieces/ZoneA/Rhombus1",
						anchor: {x: 0.5, y: 0.5},
						baseSize: {width: 301, height: 301},
						scale: {x: 1, y: 1}
					},
					physics: {
						fixedRotation: true,
						mass: 0,
						type: p2.Body.STATIC,
						shapes: {
							main: {
								id: "main",
								shape: "RHOMBUS",
								pSize: {width: 1, height: 1},
								offsetP: {x: 0, y: 0},
								angR: 0,
								sensor: true,
								collissionFilterID: "WALL"
							}
						}
					}
				},
				TrianglePentagonRight1: {
					visual: {
						id: "inGame/wallPieces/ZoneA/TrianglePentagonRight1",
						anchor: {x: 0, y: 0.5},
						baseSize: {width: 300, height: 301},
						scale: {x: 1, y: 1}
					},
					physics: {
						fixedRotation: true,
						mass: 0,
						type: p2.Body.STATIC,
						shapes: {
							main: {
								id: "main",
								shape: "POLYGON",
								pVertexesList: [
									{x: 0, y: -0.5},
									{x: 0.5, y: -0.5},
									{x: 1, y: 0},
									{x: 0.5, y: 0.5},
									{x: 0, y: 0.5}
								],
								offsetP: {x: 0, y: 0},
								angR: 0,
								sensor: true,
								collissionFilterID: "WALL"
							}
						}
					}
				},
				TrianglePentagonLeft1: {
					visual: {
						id: "inGame/wallPieces/ZoneA/TrianglePentagonRight1",
						anchor: {x: 0, y: 0.5},
						baseSize: {width: 300, height: 301},
						scale: {x: -1, y: 1}
					},
					physics: {
						fixedRotation: true,
						mass: 0,
						type: p2.Body.STATIC,
						shapes: {
							main: {
								id: "main",
								shape: "POLYGON",
								pVertexesList: [
									{x: 0, y: -0.5},
									{x: 0, y: 0.5},
									{x: -0.5, y: 0.5},
									{x: -1, y: 0},
									{x: -0.5, y: -0.5}
								],
								offsetP: {x: 0, y: 0},
								angR: 0,
								sensor: true,
								collissionFilterID: "WALL"
							}
						}
					}
				}
			}
		},
		
		constructor: function() {
			//void
		},
		
		onGInitPersistentContent: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.wallsList = [];
			
			this.createSideWall(-1);
			this.createSideWall(1);
		},
		
		createSideWall: function(screenSideX) {
			var posX;
			var angleR;
			
			if(screenSideX < 0) {
				posX = 0;
				angleR = -Math.PI / 2;
			}else {
				posX = this.game.worldArea.width;
				angleR = Math.PI / 2;
			}
			
			var pBody = new p2.Body({
				type: p2.Body.STATIC,
				mass: 0,
				position: [posX, 0],
				angle: angleR,
				fixedRotation: true
			});
			var pShape = new p2.Plane();
			pShape.collisionGroup = CollisionFilters.WALL.group;
			pShape.collisionMask = CollisionFilters.WALL.mask;
			pShape.sensor = true;
			pBody.addShape(pShape, [0, 0], 0);
			
			var pElems = {
				mainBody: pBody
			};
			
			var newWall = new WallPiece(null, null, pElems, null);
			
			pElems.mainBody.userData = {
				worldEntity: newWall
			};
			
			this.game.pWorld.addBody(pElems.mainBody);
		},
		
		onGViewAreaChanged: function(worldArea) {
			// Remove under walls.
			this.removeOffscreenUnderWalls();
			
			// Create new upper walls.
			if(this.leftGenerationBoundsTop > worldArea.y + worldArea.height) {
				this.leftGenerationBoundsTop = worldArea.y + worldArea.height;
			}
			
			if(this.rightGenerationBoundsTop > worldArea.y + worldArea.height) {
				this.rightGenerationBoundsTop = worldArea.y + worldArea.height;
			}
			
			while(this.leftGenerationBoundsTop > worldArea.y) {
				this.leftGenerationBoundsTop = this.createNewWallPiece(-1, worldArea, this.leftGenerationBoundsTop);
			}
			
			while(this.rightGenerationBoundsTop > worldArea.y) {
				this.rightGenerationBoundsTop = this.createNewWallPiece(1, worldArea, this.rightGenerationBoundsTop);
			}
		},
		
		removeOffscreenUnderWalls: function() {
			if(this.wallsList) {
				for(var i=0;i<this.wallsList.length;i++) {
					var entry = this.wallsList[i];
					
					if(entry.vElems.rootVisual.y - Math.abs(entry.vElems.rootVisual.height) / 2 > this.game.worldArea.y + this.game.worldArea.height) {
						// Wall piece is under screen, remove it.
						
						// Recycle the visuals.
						if(entry.vElems) {
							this.app.cache.releaseVElems(this.moduleBranch, entry.vElems);
							
							entry.vElems = null;
						}
						if(entry.debugVElems) {
							this.app.cache.releaseVElems(this.moduleBranch, entry.debugVElems);
							
							entry.debugVElems = null;
						}
						
						if(this.wallsList.length > 1 && i < this.wallsList.length - 1) {
							this.wallsList[i] = this.wallsList[this.wallsList.length - 1];
							
							i--;
						}
						this.wallsList.pop();
					}
				}
			}
		},
		
		createNewWallPiece: function(screenSide, worldArea, generationBoundsTop) {
			var shapesList;
			
			if(screenSide > 0) {
				shapesList = ["TriangleIsoscelerLeft1", "Box1", "Rhombus1", "TrianglePentagonLeft1"];
			}else {
				shapesList = ["TriangleIsoscelerRight1", "Box1", "Rhombus1", "TrianglePentagonRight1"];
			}
			
			var pieceConfig = this.wallPieceConfigs.ZoneA[shapesList[MathUtil.randomInRangeInt(0, shapesList.length - 1)]];
			
			var visual = this.app.popAsset(this.moduleBranch, "sprite", pieceConfig.visual.id);
			visual.anchor.set(pieceConfig.visual.anchor.x, pieceConfig.visual.anchor.y);
			
			visual.width = pieceConfig.visual.baseSize.width * pieceConfig.visual.scale.x;
			visual.height = pieceConfig.visual.baseSize.height * pieceConfig.visual.scale.y;
			
			visual.rotation = 0;
			
			var posX;
			if(screenSide > 0) {
				posX = worldArea.width;
			}else {
				posX = 0;
			}
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
			
			if(!PhysicsUtil.generatePhysicsFromConfig(this.app, this.moduleBranch, pieceConfig.physics, pElems, debugVElems, {width: visual.width, height: visual.height}, {x: posX, y: posY}, visual.rotation)) {
				this.postError("err 6");
    			
    			return false;
			}
			
			var newWall = new WallPiece(pieceConfig, vElems, pElems, debugVElems);
			this.wallsList.push(newWall);
			
			pElems.mainBody.userData = {
				worldEntity: newWall
			};
			
			this.game.pWorld.addBody(pElems.mainBody);
			
			this.game.layers.wallTiles.addChild(visual);
			
			if(debugVElems) {
				this.game.layers.physicsDebug.addChild(debugVElems.rootVisual);
			}
			
			return generationBoundsTop - Math.abs(visual.height);
		},
		
		destroy: function() {
			//asd_005;// remove the tiles assets;
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
