/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	return declare("app.ingamea.blocksgridhandler.util.DTGridInteractor", null, {
		
		cbc: null,
		
		hooks: null,
		
		constructor: function(cbc, hooks) {
			this.cbc = cbc;
			
			this.hooks = hooks;
		}

	});
});
