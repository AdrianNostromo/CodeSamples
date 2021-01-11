/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	
	"app/main/visualfxhandler/util/VisualFxKfType"
], function(declare,
	VisualFxKfType
){
	"use strict";
	var cls = declare("app.main.visualfxhandler.util.DTVisualFxKf", null, {
		
		kfType: null,
		
		preDelayMS: 0,
		
		data: null,
		
		extra: null,
		
		finishEvent: null,
		
		constructor: function(kfType, preDelayMS, data, finishEvent, extra) {
			this.kfType = kfType;
			
			this.preDelayMS = preDelayMS;
			
			this.data = data;
			
			this.finishEvent = finishEvent;
			
			this.extra = extra;
		}
	});
	
	cls.newDelay = function(preDelayMS, finishEvent, extra) {
		return new cls(VisualFxKfType.DELAY, preDelayMS, null, finishEvent, extra);
	};
	
	cls.newAnimation = function(preDelayMS, durationMS, visualAddress, param, initialValue, finalValue, useValueOffset, finishEvent, extra) {
		return new cls(VisualFxKfType.ANIMATION, preDelayMS, {
			preDelayMS: preDelayMS,
			durationMS: durationMS,
			
			visualAddress: visualAddress,
			
			param: param,
			
			initialValue: initialValue,
			finalValue: finalValue,
			
			useValueOffset: useValueOffset
		}, finishEvent, extra);
	};
	
	cls.newFunctionCall = function(preDelayMS, visualAddress, functionName, finishEvent, extra) {
		return new cls(VisualFxKfType.FUNCTION_CALL, preDelayMS, {
			visualAddress: visualAddress,
			functionName: functionName
		}, finishEvent, extra);
	};
	
	cls.newValueDetector = function(preDelayMS, visualAddress, paramAddress, targetValue, finishEvent, extra) {
		return new cls(VisualFxKfType.VALUE_DETECTOR, preDelayMS, {
			visualAddress: visualAddress,
			paramAddress: paramAddress,
			targetValue: targetValue
		}, finishEvent, extra);
	};
	
	return cls;
	
});
