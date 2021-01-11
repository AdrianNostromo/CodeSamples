/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare
){
	"use strict";
	return declare("app.ingamea.entity.DTBlockAnimationTemplate", null, {
		
		templateDelayMS: 0,
		
		totalLifeMS: 0,
		
		/*
			[]:DTBlockAnimationPropertyKeyframe
		*/
		easeParams: null,
		
		/*
			:{
				delayMS,
				template
			}
		*/
		postAnimationCompound: null,
		
		constructor: function(templateDelayMS, totalLifeMS, easeParams, postAnimationCompound) {
			this.templateDelayMS = templateDelayMS;
			
			this.totalLifeMS = totalLifeMS;
			
			this.easeParams = easeParams;
			
			this.postAnimationCompound = postAnimationCompound;
		}
	});
});
