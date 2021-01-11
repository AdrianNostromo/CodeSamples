/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/util/MathUtil",
	"app/util/VisualUtil",
	
	"app/main/visualfxhandler/util/DTVisualFXGenerator",
	"app/main/visualfxhandler/util/DTVisualFXParticle",
	"app/util/ObjectUtil",
	
	"app/main/visualfxhandler/util/VisualFxKfType",
	"app/main/visualfxhandler/util/VisualKfFinishEventType",
	"app/main/visualfxhandler/util/VisualFxTemplates",
	"app/util/tween/EaseFunctions"
], function(declare, ModuleBase,
	MathUtil, VisualUtil, 
	DTVisualFXGenerator, DTVisualFXParticle, ObjectUtil, 
	VisualFxKfType, VisualKfFinishEventType, VisualFxTemplates, EaseFunctions
){
	"use strict";
	return declare("app.main.VisualFXHandler", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"initializeAppDependantComponents", cbName:"onInitializeAppDependantComponents", isUnique:false},
			{channel:"appLoopVisualFx", cbName:"onAppLoopVisualFx", isUnique:false},
			{channel:"inGamePreDestruction", cbName:"onInGamePreDestruction", isUnique:true},
			{channel:"viewShowStateChanged", cbName:"onViewShowStateChanged", isUnique:true},
			{channel:"viewFrozenStateChanged", cbName:"onViewFrozenStateChanged", isUnique:true}
		], 
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		
		// []:DTVisualFXGenerator
		fxGenerators: null,
		// []:DTVisualFXParticle
		fxParticles: null,
		
		viewsFxPermissions: null,
		
		cfg: {
			fxSpawnDelayMSPerDistance: 100,
			blockFaceFadeDelayMS: 100,
			blockFaceFadeInSpeedS: 5,
			blockFadeOutSpeedS: 5,
			
			starCollect_verticalCompletionSpeedS: 2,
			starCollect_fadeOutSpeedS: 2
		},
		
		constructor: function() {
			//void
		},
		
		onInitializeAppDependantComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.fxGenerators = [];
			this.fxParticles = [];
			
			this.viewsFxPermissions = {};
			
			this.app.visualFx = {
				activateFX: this.onActivateFX.bind(this),
				activateFXList: this.onActivateFXList.bind(this),
				activateFxTemplate: this.onActivateFxTemplate.bind(this)
			};
		},
		
		onAppLoopVisualFx: function(appTime, deltaMS, deltaS) {
			for(var i=this.fxGenerators.length-1;i>=0;i--) {
				var entry = this.fxGenerators[i];
				
				if(!entry.pauseOnInGamePause || (this.app.game && this.app.game.isUnpaused)) {
					if(!this.tickFXGenerator(entry, deltaMS)) {
						if(this.fxGenerators.length > 1 && i < this.fxGenerators.length - 1) {
							this.fxGenerators[i] = this.fxGenerators[this.fxGenerators.length - 1];
						}
						this.fxGenerators.pop();
						
						this.disposeFreeFxGenerator(entry);
					}
				}
			}
			
			for(var i=this.fxParticles.length-1;i>=0;i--) {
				var entry = this.fxParticles[i];
				
				if(!entry.pauseOnInGamePause || (this.app.game && this.app.game.isUnpaused)) {
					var removeParticle = false;
					
					if(entry.delayMS > 0) {
						entry.delayMS = Math.max(0, entry.delayMS - deltaMS);
					}else {
						entry.lifeMS += deltaMS;
						
						if(!this.syncFXParticleVisuals(entry)) {
							removeParticle = true;
						}
					}
					
					if(removeParticle) {
						if(entry.extraData && entry.isTracked && entry.extraData.cbcTrackedParticleLifeEnd) {
							entry.extraData.cbcTrackedParticleLifeEnd.callback.call(entry.extraData.cbcTrackedParticleLifeEnd.context, entry);
						}
						
						if(this.fxParticles.length > 1 && i < this.fxParticles.length - 1) {
							this.fxParticles[i] = this.fxParticles[this.fxParticles.length - 1];
						}
						this.fxParticles.pop();
						
						this.disposeFreeFxParticle(entry);
					}
				}
			}
		},
		
		disposeFreeFxGenerator: function(entry) {
			// Remove generator from list.
			if(entry.template.igGeneratorIFlagIDs) {
				if(!this.app.game.flags.decrementFlags(entry.template.igGeneratorIFlagIDs)) {
					this.postError("err 6");
					
					return null;
				}
			}
		},
		
		disposeFreeFxParticle: function(entry) {
			if(entry.igParticlesIFlagIDs) {
				if(!this.app.game.flags.decrementFlags(entry.igParticlesIFlagIDs)) {
					this.postError("err 6");
					
					return null;
				}
			}
			
			// Remove particle;
			this.app.cache.releaseVElems(this.moduleBranch, entry.vElems);
			entry.vElems = null;
		},
		
		onActivateFxTemplate: function(templateId, generatorDelayMS, pos, moduleBranch, extraData, viewID) {
			var template = VisualFxTemplates.visualFxPlayTemplates[templateId];
			if(!template) {
				this.postError("err 6");
				
				return null;
			}
			
			this.onActivateFX(template.visualFXConfig.templateID, generatorDelayMS + template.visualFXConfig.delayMS, pos, template.visualFXConfig.assetIDs, moduleBranch, extraData, template.pauseOnInGamePause, viewID);
		},
		
		tickFXGenerator: function(entry, deltaMS) {
			if(entry.delayMS > 0) {
				entry.delayMS = Math.max(0, entry.delayMS - deltaMS);
			}
			
			if(entry.delayMS <= 0) {
				var parentLayer;
				
				if(entry.extraData && entry.extraData.parentLayer) {
					parentLayer = entry.extraData.parentLayer;
				}else {
					parentLayer = ObjectUtil.getCompositeObject(this.app, entry.template.particlesSpawnConfig.visualLayer);
				}
				if(!parentLayer) {
					this.postError("err 6");
					
					return null;
				}
				
				var isTracked;
				if(entry.template.particlesSpawnConfig.isTracked) {
					isTracked = true;
				}else {
					isTracked = false;
				}
				
				this.createParticles(
					entry.template.particlesSpawnConfig.particleTemplateId, 
					entry.template.generateConfig, 
					entry.pos, 
					entry.template.particlesSpawnConfig.initParams, 
					entry.template.particlesSpawnConfig.kfItemGroups, 
					entry.assetIDs, 
					entry.moduleBranch, 
					entry.pauseOnInGamePause, 
					entry.extraData, 
					parentLayer, 
					entry.template.particlesSpawnConfig.igParticlesIFlagIDs,
					isTracked,
					entry.viewID
				);
				
				return false;
			}
			
			return true;
		},
		
		syncFXParticleVisuals: function(particle) {
			var isAllKfFinished = true;
			
			if(particle.currentKFGroupIndice < particle.kfItemGroups.length) {
				var currentKFGroup = particle.kfItemGroups[particle.currentKFGroupIndice];
				
				for(var i=0;i<currentKFGroup.length;i++) {
					var kfItem = currentKFGroup[i];
					
					if(!kfItem || (particle.kfGroupCompletionArray.length > i && particle.kfGroupCompletionArray[i])) {
						continue ;
					}
					
					if(particle.lifeMS - particle.kfGroupStartParticleLifeMS >= kfItem.preDelayMS) {
						var kfTimeMS = (particle.lifeMS - particle.kfGroupStartParticleLifeMS) - kfItem.preDelayMS;
						
						var kfProcessFinished = true;
						
						if(kfItem.kfType === VisualFxKfType.DELAY) {
							//void
						}else if(kfItem.kfType === VisualFxKfType.ANIMATION) {
							var targetObject;
							if(kfItem.extra && kfItem.extra.isLogicKf) {
								targetObject = ObjectUtil.getCompositeObject(particle, kfItem.data.visualAddress);
							}else {
								targetObject = ObjectUtil.getCompositeObject(particle.vElems, kfItem.data.visualAddress);
							}
							
							var cp = Math.max(0, Math.min(1, kfTimeMS / kfItem.data.durationMS));
							if(cp < 1) {
								kfProcessFinished = false;
							}
							
							var processedInitialValue;
							if(typeof kfItem.data.initialValue === "number" || typeof kfItem.data.initialValue === "boolean") {
								processedInitialValue = kfItem.data.initialValue;
							}else {
								processedInitialValue = VisualUtil.computeValue(targetObject, kfItem.data.initialValue, this.app.appRect, null, (!particle.extraData) ? null : particle.extraData.customValues);
							}
							
							var processedFinalValue;
							if(typeof kfItem.data.finalValue === "number" || typeof kfItem.data.finalValue === "boolean") {
								processedFinalValue = kfItem.data.finalValue;
							}else {
								processedFinalValue = VisualUtil.computeValue(targetObject, kfItem.data.finalValue, this.app.appRect, null, (!particle.extraData) ? null : particle.extraData.customValues);
							}
							
							if(!kfItem.data.useValueOffset) {
								var easeFunction = null;
								if(kfItem.extra) {
									easeFunction = EaseFunctions[kfItem.extra.easeFunctioName];
									if(!easeFunction) {
										this.postError("err 6");
										
										return null;
									}
								}
								var val = this.computePercentValue(processedInitialValue, processedFinalValue, cp, easeFunction);
								
								ObjectUtil.setCompositeProperty(targetObject, kfItem.data.param, val);
							}else {
								// Get current value.
								var oldCp = 0;
								if(particle.lastVisualsSynkLifeMS >= 0) {
									var oldKfTimeMS = particle.lastVisualsSynkLifeMS - kfItem.preDelayMS;
									oldCp = Math.max(0, Math.min(1, oldKfTimeMS / kfItem.data.durationMS));
								}
								
								var valDelta = 0;
								
								if(particle.lastVisualsSynkLifeMS < 0) {
									// Apply a negative max offset.
									
									valDelta = processedInitialValue;
								}
								
								var easeFunction = null;
								if(kfItem.extra) {
									easeFunction = EaseFunctions[kfItem.extra.easeFunctioName];
									if(!easeFunction) {
										this.postError("err 6");
										
										return null;
									}
								}
								
								var oldOffset = this.computePercentValue(processedInitialValue, processedFinalValue, oldCp, easeFunction);
								var newOffset = this.computePercentValue(processedInitialValue, processedFinalValue, cp, easeFunction);
								
								valDelta = valDelta + (newOffset - oldOffset);
								
								ObjectUtil.setCompositePropertyDelta(targetObject, kfItem.data.param, valDelta);
							}
						}else if(kfItem.kfType === VisualFxKfType.FUNCTION_CALL) {
							var targetObject;
							if(kfItem.extra && kfItem.extra.isLogicKf) {
								targetObject = ObjectUtil.getCompositeObject(particle, kfItem.data.visualAddress);
							}else {
								targetObject = ObjectUtil.getCompositeObject(particle.vElems, kfItem.data.visualAddress);
							}
							if(!targetObject) {
								this.postError("err 6");
								
								return null;
							}
							
							targetObject[kfItem.data.functionName]();
						}else if(kfItem.kfType === VisualFxKfType.VALUE_DETECTOR) {
							var targetObject;
							if(kfItem.extra && kfItem.extra.isLogicKf) {
								targetObject = ObjectUtil.getCompositeObject(particle, kfItem.data.visualAddress);
							}else {
								targetObject = ObjectUtil.getCompositeObject(particle.vElems, kfItem.data.visualAddress);
							}
							var currentValue = ObjectUtil.getCompositeProperty(targetObject, kfItem.data.paramAddress);
							if(currentValue !== kfItem.data.targetValue) {
								kfProcessFinished = false;
							}
						}else {
							this.postError("err 6");
							
							return null;
						}
						
						if(!kfProcessFinished) {
							isAllKfFinished = false;
						}else {
							particle.kfGroupCompletionArray[i] = true;
							
							if(kfItem.finishEvent) {
								if(kfItem.finishEvent.type == VisualKfFinishEventType.ACTIVATE_FX) {
									this.onActivateFX(kfItem.finishEvent.templateID, kfItem.finishEvent.delayMS, {x: entry.vElems.rootVisual.x, y: entry.vElems.rootVisual.y}, entry.assetIDs, entry.moduleBranch, null, particle.pauseOnInGamePause, particle.viewID);
								}else if(kfItem.finishEvent.type == VisualKfFinishEventType.CREATE_GENERATOR) {
									this.createFxGenerator(
										kfItem.finishEvent.generatorTemplate, 
										kfItem.finishEvent.delayMS, 
										{
											x: particle.vElems.rootVisual.x,
											y: particle.vElems.rootVisual.y
										}, 
										particle.assetIDs,
										particle.moduleBranch, 
										particle.extraData,
										particle.pauseOnInGamePause,
										particle.viewID
									);
								}else {
									this.postError("err 6");
									
									return null;
								}
							}
						}
					}else {
						isAllKfFinished = false;
					}
				}
			 	
				if(isAllKfFinished) {
					if(particle.currentKFGroupIndice + 1 < particle.kfItemGroups.length) {
						particle.currentKFGroupIndice++;
						particle.kfGroupStartParticleLifeMS = particle.lifeMS;
						particle.kfGroupCompletionArray = [];
						
						if(particle.cbcKFGroupChanged) {
							particle.cbcKFGroupChanged.callback.call(particle.cbcKFGroupChanged.context, particle);
						}
						
						isAllKfFinished = false;
					}
				}
			}
			
			particle.lastVisualsSynkLifeMS = particle.lifeMS;
			
			if(isAllKfFinished) {
				return false;
			}
			
			return true;
		},
		
		computePercentValue: function(valA, valB, completionPercent, easeFunction) {
			if(completionPercent <= 0) {
				return valA;
			}else if(completionPercent >= 1) {
				return valB;
			}
			
			if(!easeFunction) {
				return valA + (valB - valA) * completionPercent;
			}else {
				return easeFunction(completionPercent, valA, valB - valA, 1);
			}
		},
		
		createParticles: function(particlesTemplateID, generateConfig, pos, initParams, kfItemGroups, assetIDs, moduleBranch, pauseOnInGamePause, extraData, parentLayer, igParticlesIFlagIDs, isTracked, viewID) {
			if(viewID && (!this.viewsFxPermissions[viewID] || !this.viewsFxPermissions[viewID].enableParticles)) {
				// View is not active, so no generators can be created for it.
				
				return ;
			}
			
			for(var i=0;i<generateConfig.particlesPerInterval;i++) {
				var vElems = {isVContainer: true};
				
				var template = VisualFxTemplates.particleTemplates[particlesTemplateID];
				if(!template) {
					this.postError("err 6");
					
					return null;
				}
				
				VisualUtil.createVLayoutVisuals(vElems, template.vLayout, this.app, this.moduleBranch, null, assetIDs, null);
				
				var particlesActivationDelayMS = 0;
				if(extraData && "particlesActivationDelayMS" in extraData) {
					particlesActivationDelayMS = extraData.particlesActivationDelayMS;
				}
				
				var cbcKFGroupChanged = null;
				if(extraData && "particleCbcKFGroupChanged" in extraData) {
					cbcKFGroupChanged = extraData.particleCbcKFGroupChanged;
				}
				
				var particle = new DTVisualFXParticle(template, vElems, particlesActivationDelayMS, kfItemGroups, extraData, assetIDs, moduleBranch, pauseOnInGamePause, igParticlesIFlagIDs, cbcKFGroupChanged, isTracked, viewID, {
					intervalParticleIndice: i
				});
				
				particle.vElems.rootVisual.position.set(pos.x, pos.y);
				
				if(initParams) {
					for(var n=0;n<initParams.length;n++) {
						var entry = initParams[n];
						
						VisualUtil.processNodeParamEntry(particle.vElems, this.app.appRect, ObjectUtil.getCompositeObject(particle.vElems, entry.objAddress), entry.objChange, true, null, (!extraData) ? null : extraData.customValues);
					}
				}
				
				parentLayer.addChild(particle.vElems.rootVisual);
				
				if(igParticlesIFlagIDs) {
					if(!this.app.game.flags.incrementFlags(igParticlesIFlagIDs)) {
						this.postError("err 6");
						
						return null;
					}
				}
				
				this.fxParticles.push(particle);
			}
		},
		
		onActivateFX: function(templateID, generatorDelayMS, pos, assetIDs, moduleBranch, extraData, pauseOnInGamePause, viewID) {
			var fxTemplate = VisualFxTemplates.fxTemplates[templateID];
			if(!fxTemplate) {
				this.postError("err 6");
				
				return null;
			}
			
			for(var i=0;i<fxTemplate.generators.length;i++) {
				var generatorTemplate = fxTemplate.generators[i];
				
				this.createFxGenerator(generatorTemplate, generatorDelayMS, pos, assetIDs, moduleBranch, extraData, pauseOnInGamePause, viewID);
			}
		},
		
		createFxGenerator: function(generatorTemplate, generatorDelayMS, pos, assetIDs, moduleBranch, extraData, pauseOnInGamePause, viewID) {
			if(viewID && (!this.viewsFxPermissions[viewID] || !this.viewsFxPermissions[viewID].enableGenerators)) {
				// View is not active, so no generators can be created for it.
				
				return ;
			}
			
			var generator = new DTVisualFXGenerator(generatorTemplate, pos, assetIDs, moduleBranch, extraData, generatorDelayMS + generatorTemplate.templateDelayMS, pauseOnInGamePause, viewID);
			
			if(this.tickFXGenerator(generator, 0)) {
				if(generator.template.igGeneratorIFlagIDs) {
					if(!this.app.game.flags.incrementFlags(generator.template.igGeneratorIFlagIDs)) {
						this.postError("err 6");
						
						return null;
					}
				}
				
				this.fxGenerators.push(generator);
			}
		},
		
		onActivateFXList: function(fxPlayConfigsList) {
			for(var i=0;i<fxPlayConfigsList.length;i++) {
				var entry = fxPlayConfigsList[i];
				
				this.onActivateFX(entry.templateID, entry.delayMS, entry.pos, entry.assetIDs, entry.moduleBranch, entry.pauseOnInGamePause, entry.viewID);
			}
		},
		
		onInGamePreDestruction: function() {
			if(this.fxGenerators) {
				for(var i=this.fxGenerators.length-1;i>=0;i--) {
					var entry = this.fxGenerators[i];
					
					if(entry && entry.moduleBranch === "inGame") {
						if(this.fxGenerators.length > 1 && i < this.fxGenerators.length - 1) {
							this.fxGenerators[i] = this.fxGenerators[this.fxGenerators.length - 1];
						}
						this.fxGenerators.pop();
						
						this.disposeFreeFxGenerator(entry);
					}
				}
			}
			
			if(this.fxParticles) {
				for(var i=this.fxParticles.length-1;i>=0;i--) {
					var entry = this.fxParticles[i];
					
					if(entry && entry.moduleBranch === "inGame") {
						if(this.fxParticles.length > 1 && i < this.fxParticles.length - 1) {
							this.fxParticles[i] = this.fxParticles[this.fxParticles.length - 1];
						}
						this.fxParticles.pop();
						
						this.disposeFreeFxParticle(entry);
					}
				}
			}
		},
		
		onViewShowStateChanged: function(viewID, stateEnd) {
			if(!this.viewsFxPermissions[viewID]) {
				this.viewsFxPermissions[viewID] = {
					enableGenerators: false,
					enableParticles: false
				};
			}
			
			if(stateEnd.indexOf("sIn") == 0) {
				this.viewsFxPermissions[viewID].enableGenerators = true;
			}else {
				this.viewsFxPermissions[viewID].enableGenerators = false;
				
				this.removeFxGeneratorsWithViewID(viewID);
			}
		},
		
		onViewFrozenStateChanged: function(viewID, isFrozen) {
			if(!this.viewsFxPermissions[viewID]) {
				this.viewsFxPermissions[viewID] = {
					enableGenerators: false,
					enableParticles: false
				};
			}
			
			if(isFrozen) {
				this.viewsFxPermissions[viewID].enableParticles = false;
				
				this.removeFxParticlesWithViewID(viewID);
			}else {
				this.viewsFxPermissions[viewID].enableParticles = true;
			}
		},
		
		removeFxGeneratorsWithViewID: function(viewID) {
			if(this.fxGenerators) {
				for(var i=this.fxGenerators.length-1;i>=0;i--) {
					var entry = this.fxGenerators[i];
					
					if(entry && entry.viewID && entry.viewID === viewID) {
						if(this.fxGenerators.length > 1 && i < this.fxGenerators.length - 1) {
							this.fxGenerators[i] = this.fxGenerators[this.fxGenerators.length - 1];
						}
						this.fxGenerators.pop();
						
						this.disposeFreeFxGenerator(entry);
					}
				}
			}
		},
		
		removeFxParticlesWithViewID: function(viewID) {
			if(this.fxParticles) {
				for(var i=this.fxParticles.length-1;i>=0;i--) {
					var entry = this.fxParticles[i];
					
					if(entry && entry.viewID && entry.viewID === viewID) {
						if(this.fxParticles.length > 1 && i < this.fxParticles.length - 1) {
							this.fxParticles[i] = this.fxParticles[this.fxParticles.length - 1];
						}
						this.fxParticles.pop();
						
						this.disposeFreeFxParticle(entry);
					}
				}
			}
		},
		
		destroy: function() {
			//void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
