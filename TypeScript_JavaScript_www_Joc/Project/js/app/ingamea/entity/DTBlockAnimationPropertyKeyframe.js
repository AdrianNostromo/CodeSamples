/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare
){
	"use strict";
	return declare("app.ingamea.entity.DTBlockAnimationPropertyKeyframe", null, {
		
		visualAddress: null,
		
		param: null,
		
		initialValue: 0,
		finalValue: 0,
		
		useValueOffset: false,
		
		constructor: function(visualAddress, param, initialValue, finalValue, useValueOffset) {
			this.visualAddress = visualAddress;
			
			this.param = param;
			
			this.initialValue = initialValue;
			this.finalValue = finalValue;
			
			this.useValueOffset = useValueOffset || false;
		}
		
	});
});
