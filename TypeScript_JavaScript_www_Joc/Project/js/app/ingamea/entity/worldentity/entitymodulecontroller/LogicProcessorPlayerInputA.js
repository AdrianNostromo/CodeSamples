/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	
	"app/ingamea/entity/worldentity/util/EntityActionType"
], function(declare,
	EntityActionType
){
	"use strict";
	return declare("app.ingamea.entity.worldentity.entitymodulecontroller.LogicProcessorPlayerInputA", null, {
		
		playerInput: null,
		
		constructor: function(playerInput) {
			this.playerInput = playerInput;
		},
		
		computeAction: function(controllerModule, gameTime, deltaMS, deltaS) {
			var actionCompound = {
				movementAction: null
			};
			
			var directionX;
			if(this.playerInput.changeDirection.isDown) {
				directionX = -1;
			}else {
				directionX = 1;
			}
			
			actionCompound.movementAction = {
				type: EntityActionType.FLY,
				directionHorizontal: directionX,
				directionVertical: -1
			};
			
			return actionCompound;
		},
		
		destroy: function() {
			//void
		}

	});
});
