/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare
){
	"use strict";
	
	return declare("app.main.visualfxhandler.util.DTVisualFXGenerator", null, {
		
		template: null,
		
		pos: null,
		
		assetIDs: null,
		
		moduleBranch: null,
		
		extraData: null,
		
		delayMS: 0,
		
		pauseOnInGamePause: false,
		
		viewID: null,
		
		constructor: function(template, pos, assetIDs, moduleBranch, extraData, delayMS, pauseOnInGamePause, viewID) {
			this.template = template;
			
			this.pos = pos;
			
			this.assetIDs = assetIDs;
			
			this.moduleBranch = moduleBranch;
			
			this.extraData = extraData;
			
			this.delayMS = delayMS;
			
			this.pauseOnInGamePause = pauseOnInGamePause;
			
			this.viewID = viewID;
		}

	});
});
