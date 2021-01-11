/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare
){
	"use strict";
	var cls = declare("app.util.SafeNumber", null, {
		
		valueA: 0,
		valueB: 0,
		
		constructor: function(value) {
			this.setValue(value);
		},
		
		getValue: function() {
			return this.valueA + this.valueB;
		},
		
		setValue: function(value) {
			var range = 1000 + Math.ceil(Math.random() * 1000);
			var valueA = Math.floor(value * (Math.random() - 0.5));
			var valueB = value - valueA;
			
			this.valueA = -range + valueA;
			this.valueB = range + valueB;
		},
		
		clone: function() {
			return new cls(this.getValue());
		}

	});
	
	return cls;
	
});
