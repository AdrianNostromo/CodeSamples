/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare
){
	"use strict";
	return declare("app.ingamea.entity.worldentity.EntityModuleMiner", null, {
		
		config: null,
		entity: null,
		
		constructor: function(config, entity) {
			this.config = config;
			this.entity = entity;
		},
		
		destroy: function() {
			//void
		}

	});
});
