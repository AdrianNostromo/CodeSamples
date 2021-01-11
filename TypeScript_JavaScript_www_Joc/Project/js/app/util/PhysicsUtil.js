/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	
	"app/util/MathUtil",
	"app/ingamea/util/CollisionFilters",
	
	// Global libs.
	"lib/p2js/p2"
], function(declare,
	MathUtil, CollisionFilters
){
	"use strict";
	var cls = declare("app.util.PhysicsUtil", null, {});
	
	cls.generatePhysicsFromConfig = function(app, moduleBranch, physicsConfig, pElems, debugVElems, entitySize, pos, rotR) {
		if(debugVElems) {
			debugVElems.rootVisual = app.popAsset(moduleBranch, "container");
			debugVElems.rootVisual.position.set(pos.x, pos.y);
			debugVElems.rootVisual.rotation = rotR;
		}
		
		pElems.mainBody = new p2.Body({
			type: physicsConfig.type,
			mass: physicsConfig.mass,
			position: [pos.x, pos.y],
			angle: rotR,
			fixedRotation: physicsConfig.fixedRotation
		});
		
		pElems.shapes = {};
		
		var absEntitySize = {
			width: Math.abs(entitySize.width),
			height: Math.abs(entitySize.height)
		}
		
		for (var key in physicsConfig.shapes) {
			if (physicsConfig.shapes.hasOwnProperty(key) && physicsConfig.shapes[key]) {
				var shapeConfig = physicsConfig.shapes[key];
				
				if(!this.createPShape(app, moduleBranch, shapeConfig, pElems, absEntitySize, debugVElems)) {
					App.postError("err 6");
	    			
	    			return false;
				}
			}
		}
		
		return pElems;
	};
	
	cls.createPShape = function(app, moduleBranch, shapeConfig, pElems, entitySize, debugVElems) {
		var pShape;
		var debugVerts;
		var ww;
		var hh;
		var scaledPVertsList;
		
		if(shapeConfig.shape === "BOX") {
			pShape = new p2.Rectangle(Math.abs(entitySize.width * shapeConfig.pSize.width), Math.abs(entitySize.height * shapeConfig.pSize.height));
			
			// Create physics debug chape if needed.
			if(debugVElems) {
				ww = entitySize.width * shapeConfig.pSize.width;
				hh = entitySize.height * shapeConfig.pSize.height;
				
				debugVerts = [
					{x: -ww / 2, y: -hh / 2},
					{x: ww / 2, y: -hh / 2},
					{x: ww / 2, y: hh / 2},
					{x: -ww / 2, y: hh / 2}
				];
			}
		}else if(shapeConfig.shape === "POLYGON") {
			scaledPVertsList = this.pointArrayToP2Vec2ArrayScaled(shapeConfig.pVertexesList, entitySize.width, entitySize.height);
			
			pShape = new p2.Convex(scaledPVertsList);
			
			// Create physics debug chape if needed.
			if(debugVElems) {
				debugVerts = MathUtil.cloneVertices2DScaled(shapeConfig.pVertexesList, entitySize.width, entitySize.height);
			}
		}else if(shapeConfig.shape === "TRIANGLE_ISOSCELES") {
			ww = entitySize.width * shapeConfig.pSize.width;
			hh = entitySize.height * shapeConfig.pSize.height;
			
			if(shapeConfig.orientation === "RIGHT") {
				scaledPVertsList = [
					p2.vec2.fromValues(ww, 0),
					p2.vec2.fromValues(0, hh / 2),
					p2.vec2.fromValues(0, -hh / 2)
				];
			}else if(shapeConfig.orientation === "LEFT") {
				scaledPVertsList = [
					p2.vec2.fromValues(-ww, 0),
					p2.vec2.fromValues(0, -hh / 2),
					p2.vec2.fromValues(0, hh / 2)
				];
			}else if(shapeConfig.orientation === "TOP") {
				scaledPVertsList = [
					p2.vec2.fromValues(0, -hh),
					p2.vec2.fromValues(ww / 2, 0),
					p2.vec2.fromValues(-ww / 2, 0)
				];
			}else if(shapeConfig.orientation === "BOTTOM") {
				scaledPVertsList = [
					p2.vec2.fromValues(0, hh),
					p2.vec2.fromValues(-ww / 2, 0),
					p2.vec2.fromValues(ww / 2, 0)
				];
			}else {
				App.postError("err 6");
				
				return false;
			}
			
			pShape = new p2.Convex(scaledPVertsList);
			
			// Create physics debug chape if needed.
			if(debugVElems) {
				debugVerts = scaledPVertsList;
			}
		}else if(shapeConfig.shape === "RHOMBUS") {
			ww = entitySize.width * shapeConfig.pSize.width;
			hh = entitySize.height * shapeConfig.pSize.height;
			
			scaledPVertsList = [
				p2.vec2.fromValues(0, -hh / 2),
				p2.vec2.fromValues(ww / 2, 0),
				p2.vec2.fromValues(0, hh / 2),
				p2.vec2.fromValues(-ww / 2, 0)
			];
			
			pShape = new p2.Convex(scaledPVertsList);
			
			// Create physics debug chape if needed.
			if(debugVElems) {
				debugVerts = scaledPVertsList;
			}
		}else {
			App.postError("err 6");
			
			return false;
		}
		
		pShape.sensor = shapeConfig.sensor;
		
		pShape.collisionGroup = CollisionFilters[shapeConfig.collissionFilterID].group;
		pShape.collisionMask = CollisionFilters[shapeConfig.collissionFilterID].mask;
		
		pElems.shapes[shapeConfig.id] = pShape;
		
		if(debugVElems) {
			var visual = this.createDebugGraphic(app, moduleBranch, entitySize, shapeConfig, debugVerts);
			
			debugVElems.rootVisual.addChild(visual);
			
			debugVElems[shapeConfig.id] = visual;
		}
		
		pElems.mainBody.addShape(pShape, [shapeConfig.offsetP.x * entitySize.width, shapeConfig.offsetP.y * entitySize.height], shapeConfig.angR);
		
		return true;
	};
	
	cls.createDebugGraphic = function(app, moduleBranch, entitySize, shapeConfig, pVertexList) {
		var visual = app.popAsset(moduleBranch, "graphics");
		
		visual.position.set(shapeConfig.offsetP.x * entitySize.width, shapeConfig.offsetP.y * entitySize.height);
		visual.rotation = shapeConfig.angR;
		
		visual.clear();
		
		visual.lineStyle(2, 0x0000ff, 1);
		
	    visual.beginFill(0x00ff00, 0.4);
	    
	    var vert = pVertexList[0];
	    visual.moveTo(vert.x, vert.y);
	    
	    for(var i=1;i<pVertexList.length;i++) {
	    	vert = pVertexList[i];
		    visual.lineTo(vert.x, vert.y);
	    }
	    
	    vert = pVertexList[0];
	    visual.lineTo(vert.x, vert.y);
	    
	    visual.endFill();
	    
	    return visual;
	};
	
	cls.pointArrayToP2Vec2ArrayScaled = function(vertsList, scaleX, scaleY) {
		var newVertsList = [];
		
		for(var i=0;i<vertsList.length;i++) {
			var entry = vertsList[i];
			
			newVertsList.push(p2.vec2.fromValues(entry.x * scaleX, entry.y * scaleY));
		}
		
		return newVertsList;
	};
	
	return cls;
});
