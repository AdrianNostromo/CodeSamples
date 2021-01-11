/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/EntityType",
	"app/ingamea/entity/worldentity/EntityModuleSegmentedBody",
	"app/util/PhysicsUtil",
	"app/ingamea/util/WorldObjectType",
	
	// Global libs.
	"lib/p2js/p2"
], function(declare, ModuleBase,
	EntityType, EntityModuleSegmentedBody, PhysicsUtil, WorldObjectType
){
	"use strict";
	return declare("app.ingamea.EntitySegmentedBodyHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false},
			{channel:"gInitEntityBase", cbName:"onGInitEntityBase", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game: null,
		
		cfg: {
			maxBodySegmentsCount: 5
		},
		
		constructor: function() {
			//void
		},
		
		onGInitBaseComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
		},
		
		onGInitEntityBase: function(entity, entityInfo) {
			if(!("segmentedBody" in entity.config.modules)) {
				return ;
			}
			
			var moduleConfig = entity.config.modules.segmentedBody;
			
			entity.modules.segmentedBody = new EntityModuleSegmentedBody(moduleConfig, entity);
			
			entity.events.register("bodySegmentsCountChangeRequest", this.onEntityBodySegmentsCountChangeRequest, this, false);
		},
		
		onEntityBodySegmentsCountChangeRequest: function(entity, newSegmentsCount) {
			var limitedNewSegsCount = Math.min(newSegmentsCount, this.cfg.maxBodySegmentsCount);
			if(entity.modules.segmentedBody.segmentsCount == limitedNewSegsCount) {
				return ;
			}
			
			// Remove extra segments;
			while(entity.modules.segmentedBody.segmentsCount > limitedNewSegsCount) {
				this.removeEntityBodySegment(entity);
			}
			
			// Create new segments;
			while(entity.modules.segmentedBody.segmentsCount < limitedNewSegsCount) {
				this.addNewEntityBodySegment(entity);
			}
			
			entity.modules.segmentedBody.updateSegmentsPosition();
		},
		
		removeEntityBodySegment: function(entity) {
			asd_001;
		},
		
		addNewEntityBodySegment: function(entity) {
			var visualConfig = entity.modules.segmentedBody.config.visual;
			
			var visual = this.app.popAsset(this.moduleBranch, "sprite", visualConfig.id);
			visual.anchor.set(visualConfig.anchor.x, visualConfig.anchor.y);
			visual.width = visualConfig.baseSize.width * visualConfig.scale.x;
			visual.height = visualConfig.baseSize.height * visualConfig.scale.y;
			this.game.layers.units.addChild(visual);
			
			entity.modules.segmentedBody.vElems["bodySegment_" + entity.modules.segmentedBody.segmentsCount] = visual;
			
			entity.modules.segmentedBody.segmentsCount++;
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
