/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	return declare("app.ingamea.util.DTWorldObjectsInterraction", null, {
		
		pBodyA: null,
		objectA: null,
		
		pBodyB: null,
		objectB: null,
		
		constructor: function(pBodyA, objectA, pBodyB, objectB) {
			this.pBodyA = pBodyA;
			this.objectA = objectA;
			
			this.pBodyB = pBodyB;
			this.objectB = objectB;
		}

	});
});
