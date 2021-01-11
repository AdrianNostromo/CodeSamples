/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	var cls = declare("app.util.MathUtil", null, {});
	
	/**
	 * Returns a random number between min (inclusive) and max (exclusive)
	 */
	cls.randomInRangeNumber = function(min, max) {
		return Math.random() * (max - min) + min;
	};
		
	/**
	 * Returns a random integer between min (inclusive) and max (inclusive)
	 * Using Math.round() will give you a non-uniform distribution!
	 */
	cls.randomInRangeInt = function(min, max) {
		return Math.floor(Math.random() * (max - min + 1)) + min;
	};
	
	cls.randomInRangeIntPrng = function(prngFunc, min, max) {
		return Math.floor(prngFunc() * (max - min + 1)) + min;
	};

	cls.radiansToDegrees = function(angR) {
		return angR * (180/Math.PI);
	};

	cls.degreesToRadians = function(angD) {
		return angD * (Math.PI/180);
	};

	cls.computeDistance = function(aX, aY, bX, bY) {
		return Math.sqrt((bX - aX) * (bX - aX) + (bY - aY) * (bY - aY));
	};

	cls.computePointDistance = function(aX, aY) {
		return Math.sqrt(aX * aX + aY * aY);
	};

	cls.numberToZeroPadded = function(num, padLen) {
		var N = Math.pow(10, padLen);
        return num < N ? ("" + (N + num)).slice(1) : "" + num;
	};
	
	cls.computeRotDegreesInTopParentWorld = function(baseChild, topParent, angDOffset) {
		if(!baseChild || !topParent) {
			App.postError("err_012");
			
			return false;
		}
		
		var angDDir = 1;
		
		if(baseChild !== topParent && !baseChild.parent) {
			// if not the same visual, the parent of the baseChild must be defined;
			
			App.postError("err_012");
			
			return false;
		}

		var currentItem = null;
		var angR = 0;
		
		do {
			if(!currentItem) {
				currentItem = baseChild;
			}else {
				currentItem = currentItem.parent;
			}

			angR += currentItem.rotation;
			if((currentItem.scale.x < 0 && currentItem.scale.y > 0) || (currentItem.scale.x > 0 && currentItem.scale.y < 0)) {
				angR += Math.PI;
				
				angDDir *= -1;
			}
		}while(currentItem !== topParent);
		
		var retAngD = angR * (180/Math.PI);
		
		retAngD += angDDir * angDOffset;
		
		return retAngD;
	};

	cls.computePosInTopParentWorld = function(posOnChild, baseChild, topParent) {
		if(!baseChild || !topParent || !baseChild.parent) {
			App.postError("err_012");
			
			return false;
		}
		
		var posOnCurrentX = posOnChild.x,
			posOnCurrentY = posOnChild.y;
		
		var currentItem = null;
		do {
			if(!currentItem) {
				currentItem = baseChild;
			}else {
				currentItem = currentItem.parent;
			}
			// compute the position in the coordinates that currentItem resides in;
			
			var len = Math.sqrt(posOnCurrentX * posOnCurrentX + posOnCurrentY * posOnCurrentY),
				angR = Math.atan2(posOnCurrentY, posOnCurrentX);
			angR += currentItem.rotation;
			
			posOnCurrentX = currentItem.x + Math.cos(angR) * (len * currentItem.scale.x);
			posOnCurrentY = currentItem.y + Math.sin(angR) * (len * currentItem.scale.y);
		}while(currentItem !== topParent);
		
		var ret = {
			x:posOnCurrentX,
			y:posOnCurrentY
		};
		
		return ret;
	};
	
	cls.getSmallestArcAngleDegrees = function(p_sourceAngleDegrees, p_targetAngleDegrees) {
		var mv_currentRot = p_sourceAngleDegrees;
		var mv_targAng = p_targetAngleDegrees;
		var mv_angDif;

		while(mv_targAng < 0) {
			mv_targAng += 360;
		}
		while(mv_targAng >= 360) {
			mv_targAng -= 360;
		}

		while(mv_currentRot < 0) {
			mv_currentRot += 360;
		}
		while(mv_currentRot >= 360) {
			mv_currentRot -= 360;
		}

		mv_angDif = mv_targAng - mv_currentRot;
		if(mv_angDif <= -180) {
			mv_angDif += 360;
		}
		if(mv_angDif > 180) {
			mv_angDif -= 360;
		}

		return mv_angDif;
	};
	
	cls.shuffleFisherYates = function(array) {
		var m = array.length, t, i;
		
		while (m) {
			// Pick a remaining element…
			i = Math.floor(Math.random() * m--);
			
			// And swap it with the current element.
			t = array[m];
			array[m] = array[i];
			array[i] = t;
		}

		return array;
	};
	
	cls.cloneVertices2DScaled = function(vertsList, scaleX, scaleY) {
		var newVertsList = [];
		
		for(var i=0;i<vertsList.length;i++) {
			var entry = vertsList[i];
			
			newVertsList.push({x: entry.x * scaleX, y: entry.y * scaleY});
		}
		
		return newVertsList;
	};
	
	return cls;
});
