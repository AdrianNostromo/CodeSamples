/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare
){
	"use strict";
	return declare("app.ingamea.entity.DTBlockAnimation", null, {
		
		template: null,
		
		animationDelayMS: 0,
		
		lifeMS: 0,
		
		delayCounterMS: 0,
		
		// If -1, no sync occured, apply offset values here.
		lastVisualSinkLifeMS: -1,
		
		constructor: function(template, animationDelayMS) {
			this.template = template;
			
			this.animationDelayMS = animationDelayMS;
		}
		
	});
});
