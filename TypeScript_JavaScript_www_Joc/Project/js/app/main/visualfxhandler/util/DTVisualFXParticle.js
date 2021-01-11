/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare
){
	"use strict";
	
	return declare("app.main.visualfxhandler.util.DTVisualFXParticle", null, {
		
		template: null,
		
		vElems: null,
		
		delayMS: 0,
		
		kfItemGroups: null,
		
		extraData: null,
		assetIDs: null,
		
		moduleBranch: null,
		
		pauseOnInGamePause: false,
		
		igParticlesIFlagIDs: null,
		
		cbcKFGroupChanged: null,
		
		lifeMS: 0,
		// Used to compute and initial offset for offset keyframes.
		lastVisualsSynkLifeMS: -1,
		
		currentKFGroupIndice: 0,
		kfGroupStartParticleLifeMS: 0,
		// If item is true, kf item finished processing.
		kfGroupCompletionArray: null,
		
		isTracked: true,
		
		viewID: null,
		
		data: null,
		
		constructor: function(template, vElems, delayMS, kfItemGroups, extraData, assetIDs, moduleBranch, pauseOnInGamePause, igParticlesIFlagIDs, cbcKFGroupChanged, isTracked, viewID, data) {
			this.template = template;
			
			this.vElems = vElems;
			
			this.delayMS = delayMS;
			
			this.kfItemGroups = kfItemGroups;
			
			this.extraData = extraData;
			this.assetIDs = assetIDs;
			
			this.moduleBranch = moduleBranch;
			
			this.pauseOnInGamePause = pauseOnInGamePause;
			
			this.igParticlesIFlagIDs = igParticlesIFlagIDs;
			
			this.cbcKFGroupChanged = cbcKFGroupChanged;
			
			this.isTracked = isTracked;
			
			this.viewID = viewID;
			
			this.data = data;
			
			this.kfGroupCompletionArray = [];
		}

	});
});
