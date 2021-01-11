/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/blocksgridhandler/GridInteraction",
	
	"app/ingamea/util/BlockType",
	"app/ingamea/util/BlockEventType",
	"app/ingamea/util/EntityVisualType",
	"app/ingamea/entity/WorldBlock",
	"lib/pixijs/pixi",
	
	"app/util/VisualUtil",
	"app/util/EventsDispatcher",
	"app/ingamea/entity/DTBlockAnimation",
	"app/ingamea/entity/DTBlockAnimationTemplate",
	"app/util/ObjectUtil",
	
	"app/ingamea/entity/DTBlockAnimationPropertyKeyframe",
	"app/main/view/util/ViewType"
], function(declare, ModuleBase,
	GridInteraction,
	BlockType, BlockEventType, EntityVisualType, WorldBlock, PIXI,
	VisualUtil, EventsDispatcher, DTBlockAnimation, DTBlockAnimationTemplate, ObjectUtil,
	DTBlockAnimationPropertyKeyframe, ViewType
){
	"use strict";
	return declare("app.ingamea.blocksgridhandler.BlockActionsHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites: null,
		subscribeListeners:[
			{channel:"gInitDependantComponents", cbName:"onGInitDependantComponents", isUnique:false},
			{channel:"gGameLoopGameElements", cbName:"onGGameLoopGameElements", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game: null,
		
		cfg: {
		},
		
		blockAnimationTemplates: {
			blockInAll: new DTBlockAnimationTemplate(
				0,
				500,
				[
					new DTBlockAnimationPropertyKeyframe("rootVisual", "y", -120, 0, true),
					new DTBlockAnimationPropertyKeyframe("rootVisual", "alpha", 0, 1, false),
					new DTBlockAnimationPropertyKeyframe("rootVisual", ["scale", "x"], 0.4, 1, false),
					new DTBlockAnimationPropertyKeyframe("rootVisual", ["scale", "y"], 0.4, 1, false)
				],
				null
			),
			specialItemNoActivation: new DTBlockAnimationTemplate(
				0,
				80,
				[
					new DTBlockAnimationPropertyKeyframe("rootVisual", ["scale", "x"], 1, 0.8, false),
					new DTBlockAnimationPropertyKeyframe("rootVisual", ["scale", "y"], 1, 0.8, false)
				],
				{
					delayMS: 0,
					template: new DTBlockAnimationTemplate(
						0,
						80,
						[
							new DTBlockAnimationPropertyKeyframe("rootVisual", ["scale", "x"], 0.8, 1, false),
							new DTBlockAnimationPropertyKeyframe("rootVisual", ["scale", "y"], 0.8, 1, false)
						],
						null
					)
				}
			),
			cementItemNoActivation: new DTBlockAnimationTemplate(
				0,
				80,
				[
					new DTBlockAnimationPropertyKeyframe("rootVisual", ["scale", "x"], 1, 0.8, false),
					new DTBlockAnimationPropertyKeyframe("rootVisual", ["scale", "y"], 1, 0.8, false)
				],
				{
					delayMS: 0,
					template: new DTBlockAnimationTemplate(
						0,
						80,
						[
							new DTBlockAnimationPropertyKeyframe("rootVisual", ["scale", "x"], 0.8, 1, false),
							new DTBlockAnimationPropertyKeyframe("rootVisual", ["scale", "y"], 0.8, 1, false)
						],
						null
					)
				}
			),
		},
		
		constructor: function() {
			//void
		},
		
		onGInitDependantComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.game.grid.blocksInterractor.register("blockActivateEventRequest", this.onBlockActivateEventRequest, this, false);
		},
		
		onBlockActivateEventRequest: function(block, eventType, extraData) {
			this.activateBlockEvent(block, eventType, extraData);
		},
		
		activateBlockEvent: function(block, eventType, extraData) {
			var eventConfig = block.config.eventEffects[eventType];
			if(eventConfig) {
				var delayMS = 0;
				if(extraData && "delayMS" in extraData) {
					delayMS = extraData.delayMS;
				}
				
				if(eventConfig.animationConfig) {
					var animationTemplate = this.blockAnimationTemplates[eventConfig.animationConfig.templateID];
					
					block.activeAnimation = new DTBlockAnimation(animationTemplate, delayMS + eventConfig.animationConfig.delayMS);
					
					this.syncBlockAnimationParams(block);
				}
				
				if(eventConfig.visualFXConfig) {
					this.app.visualFx.activateFX(eventConfig.visualFXConfig.templateID, delayMS + eventConfig.visualFXConfig.delayMS, block.getVPos(), eventConfig.visualFXConfig.assetIDs, this.moduleBranch, extraData, true, ViewType.IN_GAME);
				}
			}
		},
		
		onGGameLoopGameElements: function(gameTime, deltaMS, deltaS) {
			if(!this.tickInAnimation(deltaMS, deltaS)) {
				// void
			}
		},
		
		tickInAnimation: function(deltaMS, deltaS) {
			var ct = 0;
			
			for(var i=0;i<this.game.grid.blocks.length;i++) {
				var column = this.game.grid.blocks[i];
				
				for(var j=0;j<column.length;j++) {
					var block = column[j];
					if(block && block.activeAnimation) {
						if(block.activeAnimation.animationDelayMS + block.activeAnimation.template.templateDelayMS > block.activeAnimation.delayCounterMS) {
							block.activeAnimation.delayCounterMS = Math.min(block.activeAnimation.animationDelayMS + block.activeAnimation.template.templateDelayMS, block.activeAnimation.delayCounterMS + deltaMS);
						}else {
							block.activeAnimation.lifeMS = Math.min(block.activeAnimation.template.totalLifeMS, block.activeAnimation.lifeMS + deltaMS);
							
							this.syncBlockAnimationParams(block);
							
							if(block.activeAnimation.lifeMS >= block.activeAnimation.template.totalLifeMS) {
								if(block.activeAnimation.template.postAnimationCompound) {
									block.activeAnimation = new DTBlockAnimation(block.activeAnimation.template.postAnimationCompound.template, block.activeAnimation.template.postAnimationCompound.delayMS);
								}else {
									block.activeAnimation = null;
								}
							}
						}
						
						if(block.activeAnimation) {
							ct++;
						}
					}
				}
			}
			
			return ct;
		},
		
		syncBlockAnimationParams: function(block) {
			var cp = block.activeAnimation.lifeMS / block.activeAnimation.template.totalLifeMS;
			var oldCp = 0;
			if(block.activeAnimation.lastVisualSinkLifeMS >= 0) {
				oldCp = block.activeAnimation.lastVisualSinkLifeMS / block.activeAnimation.template.totalLifeMS;
			}
			
			for(var i=0;i<block.activeAnimation.template.easeParams.length;i++) {
				var entry = block.activeAnimation.template.easeParams[i];
				
				if(entry.useValueOffset) {
					// Get current value.
					var valDelta = 0;
					
					if(block.activeAnimation.lastVisualSinkLifeMS < 0) {
						// Apply a negative max offset.
						
						valDelta = entry.initialValue;
					}
					
					var oldOffset = entry.initialValue + (entry.finalValue - entry.initialValue) * oldCp;
					var newOffset = entry.initialValue + (entry.finalValue - entry.initialValue) * cp;
					
					valDelta = valDelta + (newOffset - oldOffset);
					
					ObjectUtil.setCompositePropertyDeltaOnCompositeObject(block.vElems, entry.visualAddress, entry.param, valDelta);
				}else {
					// Do a regullar transition.
					
					var val = entry.initialValue + (entry.finalValue - entry.initialValue) * cp;
					
					ObjectUtil.setCompositePropertyOnCompositeObject(block.vElems, entry.visualAddress, entry.param, val);
				}
			}
			
			block.activeAnimation.lastVisualSinkLifeMS = block.activeAnimation.lifeMS;
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
