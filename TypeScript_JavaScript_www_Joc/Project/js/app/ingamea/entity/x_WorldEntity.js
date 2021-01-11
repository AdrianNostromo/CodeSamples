/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	
	"app/util/EventsDispatcher",
	"app/ingamea/util/WorldObjectType"
], function(declare,
	EventsDispatcher, WorldObjectType
){
	"use strict";
	return declare("app.ingamea.entity.WorldEntity", null, {
		
		worldObjectType: null,
		
		type: null,
		id: null,
		
		config: null,
		
		// If true, the unit is in the entities list. This is used to check if a entity was destroyed.
		isActive: true,
		
		// If false, the entity is ghosted but still in the entities list and it doesn't interract with anything.
		// This may be used in certain cases to remove the entity from the world temporarilly.
		isPhasedIn: true,
		
		/**
		 * Modules list:
		 * .visual;
		 * .controller;
		 * .groundUnit;
		 * .physics;
		 */
		modules: null,
		
		/**
		 * Events list:
		 * By: this
		 * positionChanged(entity, posX, posY);
		 * isPhasedInChanged(entity, isPhasedIn);
		 * 
		 * By: controller
		 * processControllerAction(entity, action);
		 */
		events: null,
		
		// .isPlayerUnit
		// .isPlayerHero
		flags: null,
		
		userData: null,
		
		constructor: function(type, classID, config) {
			this.worldObjectType = WorldObjectType.ENTITY;
			
			this.type = type;
			
			this.classID = classID;
			
			this.config = config;
			
			this.modules = {};
			
			this.events = new EventsDispatcher();
			
			this.flags = {};
			
			this.userData = {};
		},
		
		getPosition: function() {
			if(this.modules.physics) {
				return {
					x: this.modules.physics.pElems.mainBody.position[0],
					y: this.modules.physics.pElems.mainBody.position[1]
				};
			}
			
			return {
				x: this.modules.visual.vElems.rootVisual.x,
				y: this.modules.visual.vElems.rootVisual.y
			};
		},
		
		setPosition: function(x, y) {
			this.events.dispatch("positionChangedManually", this, x, y);
			
			this.syncPosition(x, y);
		},
		
		syncPosition: function(x, y) {
			this.events.dispatch("positionChanged", this, x, y);
		},
		
		getRotationR: function() {
			if(this.modules.physics) {
				return this.modules.physics.pElems.mainBody.angle;
			}
			
			return this.modules.visual.vElems.rootVisual.rotation;
		},
		
		setRotationR: function(rotR) {
			this.events.dispatch("rotationChangedManually", this, rotR);
			
			this.syncRotationR(rotR);
		},
		
		syncRotationR: function(rotR) {
			this.events.dispatch("rotationChanged", this, rotR);
		},
		
		setIsPhasedIn: function(isPhasedIn) {
			if(this.isPhasedIn == isPhasedIn) {
				return ;
			}
			
			this.isPhasedIn = isPhasedIn;
			
			this.events.dispatch("isPhasedInChanged", this, this.isPhasedIn);
		},
		
		destroy: function() {
			if(this.modules) {
				for (var key in this.modules) {
					if (this.modules.hasOwnProperty(key) && this.modules[key]) {
						this.modules[key].destroy();
						this.modules[key] = null;
					}
				}
				
				this.modules = null;
			}
			
			if(this.events) {
				this.events.destroy();
				
				this.events = null;
			}
		}

	});
});
