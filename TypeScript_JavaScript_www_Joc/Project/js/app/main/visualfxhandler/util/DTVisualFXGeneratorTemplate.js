/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare
){
	"use strict";
	return declare("app.main.visualfxhandler.util.DTVisualFXGeneratorTemplate", null, {
		
		templateDelayMS: 0,
		
		// inGame only.
		igGeneratorIFlagIDs: null,
		
		generateConfig: null,
		
		particlesSpawnConfig: null,
		
		constructor: function(templateDelayMS, igGeneratorIFlagIDs, generateConfig, particlesSpawnConfig) {
			this.templateDelayMS = templateDelayMS;
			
			this.igGeneratorIFlagIDs = igGeneratorIFlagIDs;
			
			this.generateConfig = generateConfig;
			
			this.particlesSpawnConfig = particlesSpawnConfig;
		}
		
	});
});
