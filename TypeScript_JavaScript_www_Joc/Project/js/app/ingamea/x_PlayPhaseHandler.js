/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/ingamea/util/PlayPhaseType",
	"lib/pixijs/pixi"
], function(declare, ModuleBase,
	PlayPhaseType, PIXI
){
	"use strict";
	return declare("app.ingamea.PlayPhaseHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitBaseComponents", cbName:"onGInitBaseComponents", isUnique:false},
			{channel:"gInitGeneralContent", cbName:"onGInitGeneralContent", isUnique:false}
		],
		publishMsg: {
			gInitializeWave:{channel:"gInitializeWave", listenerType:"list", mustExist:false},
			gGenerateNewGameBoard:{channel:"gGenerateNewGameBoard", listenerType:"any", mustExist:true},
			gPlayPhaseChanged:{channel:"gPlayPhaseChanged", listenerType:"list", mustExist:false},
			
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		game: null,
		
		constructor: function() {
			//void
		},
		
		asd_001;// move the game start logic from here.
		
		onGInitBaseComponents: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.game.playPhase = PlayPhaseType.PRE_WAVE;
			
			this.game.wave = {
				indice: 0,
				template: null
			};
		},
		
		onGInitGeneralContent: function() {
			if(this.game.playPhase == PlayPhaseType.PRE_WAVE) {
				this.startNewWave();
			}
		},
		
		//asd_002; // Wait for a level win or loss condition to change the playPhase;
		
		startNewWave: function() {
			// Go to next template indice and initialize board.
			this.game.wave.indice++;
			
			this.game.wave.template = this.game.levelConfig.levelWaves[Math.min(this.game.levelConfig.levelWaves.length - 1, this.game.wave.indice)];
			
			this.publisher.publish(this.publishMsg.gInitializeWave);
			this.publisher.publish(this.publishMsg.gGenerateNewGameBoard);
			
			this.setPlayPhase(PlayPhaseType.WAVE_ACTIVE);
		},
		
		setPlayPhase: function(playPhase) {
			this.game.playPhase = playPhase;
			
			this.publisher.publish(this.publishMsg.gPlayPhaseChanged, this.game.playPhase);
		},
		
		destroy: function() {
			//asd_002; remove the entities;
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
