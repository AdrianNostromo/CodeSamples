/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	
	"app/ingamea/entity/worldentity/util/EntityActionType",
	"app/util/MathUtil"
], function(declare,
	EntityActionType, MathUtil
){
	"use strict";
	return declare("app.ingamea.entity.worldentity.EntityModuleDiagonalFlight", null, {
		
		config: null,
		entity: null,
		
		levelData: null,
		
		constructor: function(config, entity, levelData) {
			this.config = config;
			this.entity = entity;
			
			this.levelData = levelData;
			
			this.entity.events.register("setEntityAction", this.onSetEntityAction, this, false);
		},
		
		onSetEntityAction: function(entity, actionCompound, gameTime, deltaMS, deltaS) {
			if(actionCompound.movementAction.type === EntityActionType.FLY) {
				// Fly in a diagonal and set rotation.
				
				var angLeftR = Math.PI * 5 / 4;
				var angRightR = Math.PI * 7 / 4;
				
				var originalAngR = this.entity.modules.physics.pElems.mainBody.angle;
				var angR = originalAngR;
				
				// Normalize the angle.
				while(angR >= Math.PI * 2) {
					angR -= Math.PI * 2;
				}
				while(angR < 0) {
					angR += Math.PI * 2;
				}
				
				// Snap the angle in range.
				if(angR < angLeftR || angR > angRightR) {
					if(angR < Math.PI * 3 / 2 && angR > Math.PI / 2) {
						angR = angLeftR;
					}else {
						angR = angRightR;
					}
				}
				
				if(actionCompound.movementAction.directionHorizontal > 0) {
					angR = Math.min(angRightR, angR + this.levelData.playerTurnSpeedS * deltaS);
				}else if(actionCompound.movementAction.directionHorizontal < 0) {
					angR = Math.max(angLeftR, angR - this.levelData.playerTurnSpeedS * deltaS);
				}
				
				// Rotate the unit.
				if(originalAngR != angR) {
					this.entity.setRotationR(angR);
					
					var velX = Math.cos(angR) * this.levelData.currentPlayerUnitVelocity;
					var velY = Math.sin(angR) * this.levelData.currentPlayerUnitVelocity;
					
					this.entity.modules.physics.pElems.mainBody.velocity[0] = velX;
					this.entity.modules.physics.pElems.mainBody.velocity[1] = velY;
				}
			}else {
				// Stop entity movement.
				this.entity.modules.physics.pElems.mainBody.velocity[0] = 0;
				this.entity.modules.physics.pElems.mainBody.velocity[1] = 0;
				
				this.entity.setRotationR(0);
			}
		},
		
		destroy: function() {
			//void
		}

	});
});
