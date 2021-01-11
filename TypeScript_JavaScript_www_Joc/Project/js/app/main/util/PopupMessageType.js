/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	var cls = declare("app.main.util.PopupMessageType", null, {});
	
	cls.INSUFFICIENT_DIAMONDS_FOR_ACTION = "INSUFFICIENT_DIAMONDS_FOR_ACTION";
	cls.NOT_AVAILABLE = "NOT_AVAILABLE";
	
	return cls;
	
});
