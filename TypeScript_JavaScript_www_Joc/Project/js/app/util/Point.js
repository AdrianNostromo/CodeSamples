/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	return declare("app.util.Point", null, {
		x: undefined,
		y: undefined,

		constructor: function(x, y) {
			this.x = x;
			this.y = y;
		}

	});
});
