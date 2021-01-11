/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/util/InputUtil",
	"app/util/VisualUtil"
], function(declare, ModuleBase,
	InputUtil, VisualUtil
){
	"use strict";
	return declare("app.ingamea.PlayerInputHandler", ModuleBase, {
		moduleBranch: "inGame",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"gInitPersistentContent", cbName:"onGInitPersistentContent", isUnique:false},
			{channel:"appSizeChanged",  cbName:"onAppSizeChanged",  isUnique:false}
		],
		publishMsg: {
			gCreateEntity:{channel:"gCreateEntity", listenerType:"unique", mustExist:true},
			
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			gGetGame:{channel:"gGetGame", listenerType:"unique", mustExist:true},
			gGetUnpausedCounter:{channel:"gGetUnpausedCounter", listenerType:"unique", mustExist:true},
			gIncrementUnpauseState:{channel:"gIncrementUnpauseState", listenerType:"any", mustExist:false}
		},
		
		app:null,
		game: null,
		
		inputKeys: null,
		
		cfg: {
			keyCodes: {
				UP: 38,
				DOWN: 40,
				LEFT: 37,
				RIGHT: 39,
				SPACE: 32,
				ESC: 27
			}
		},
		
		constructor: function() {
			//void
		},
		
		onGInitPersistentContent: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			this.game = this.publisher.publish(this.publishMsg.gGetGame);
			
			this.game.playerInput = {
				changeDirection: {
					isDown: false, 
					downImpulseCount: 0,
					upImpulseCount: 0
				}
			};
			
			// Hook keyboard input.
			window.addEventListener("keydown", this.onKeyDown.bind(this), false);
			window.addEventListener("keyup", this.onKeyUp.bind(this), false);
			
			// Hook touch input on a visual.
			this.vElems = {
				touchDetector: this.app.popAsset(this.moduleBranch, "sprite", "touchDetector")
			};
			this.vElems.touchDetector.alpha = 0;
			this.onAppSizeChanged();
			
			this.vElems.touchDetector.interactive = true;
			
			this.vElems.touchDetector.on('touchstart', this.onTouchDetectorTouchStart.bind(this));
		    this.vElems.touchDetector.on('touchend', this.onTouchDetectorTouchEnd.bind(this));
		    this.vElems.touchDetector.on('touchendoutside', this.onTouchDetectorTouchEnd.bind(this));
		    this.vElems.touchDetector.on('touchmove', this.onTouchDetectorTouchMove.bind(this));
		    
		    this.vElems.touchDetector.on('mousedown', this.onTouchDetectorMouseDown.bind(this));
		    this.vElems.touchDetector.on('mouseup', this.onTouchDetectorMouseUp.bind(this));
		    this.vElems.touchDetector.on('mouseupoutside', this.onTouchDetectorMouseUp.bind(this));
		    this.vElems.touchDetector.on('mousemove', this.onTouchDetectorMouseMove.bind(this));
			
			this.game.layers.touchDetector.addChild(this.vElems.touchDetector);
		},
		
		onTouchDetectorTouchStart: function(event) {
			//asd_tst;
			if(event.data.global.y < this.app.appSize.height / 3) {
				if(this.publisher.publish(this.publishMsg.gGetUnpausedCounter) == 0) {
					this.game.isLevelLost = false;
					
					this.publisher.publish(this.publishMsg.gIncrementUnpauseState, true);
				}else if(this.publisher.publish(this.publishMsg.gGetUnpausedCounter) == 1) {
					this.publisher.publish(this.publishMsg.gIncrementUnpauseState, false);
				}
			}
			
			if(!this.game.playerInput.changeDirection.isDown) {
				this.game.playerInput.changeDirection.isDown = true;
				
				this.game.playerInput.changeDirection.downImpulseCount++;
			}
		},
		
		onTouchDetectorTouchEnd: function(event) {
			if(this.game.playerInput.changeDirection.isDown) {
				this.game.playerInput.changeDirection.isDown = false;
				
				this.game.playerInput.changeDirection.upImpulseCount++;
			}
		},
		
		onTouchDetectorTouchMove: function(event) {
			//void
		},
		
		onTouchDetectorMouseDown: function(event) {
			if(!this.game.playerInput.changeDirection.isDown) {
				this.game.playerInput.changeDirection.isDown = true;
				
				this.game.playerInput.changeDirection.downImpulseCount++;
			}
		},
		
		onTouchDetectorMouseUp: function(event) {
			if(this.game.playerInput.changeDirection.isDown) {
				this.game.playerInput.changeDirection.isDown = false;
				
				this.game.playerInput.changeDirection.upImpulseCount++;
			}
		},
		
		onTouchDetectorMouseMove: function(event) {
			//void
		},
		
		onAppSizeChanged: function() {
			this.vElems.touchDetector.width = this.app.appSize.width;
			this.vElems.touchDetector.height = this.app.appSize.height;
		},
		
		onKeyDown: function(event) {
			var playerInputKey = null;
			
			if(event.keyCode == this.cfg.keyCodes.UP) {
				playerInputKey = this.game.playerInput.UP;
			}else if(event.keyCode == this.cfg.keyCodes.DOWN) {
				playerInputKey = this.game.playerInput.DOWN;
			}else if(event.keyCode == this.cfg.keyCodes.LEFT) {
				playerInputKey = this.game.playerInput.LEFT;
			}else if(event.keyCode == this.cfg.keyCodes.RIGHT) {
				playerInputKey = this.game.playerInput.RIGHT;
			}
			
			if(playerInputKey && !playerInputKey.isDown) {
				playerInputKey.isDown = true;
				playerInputKey.downImpulseCount++;
			}
		},
		
		onKeyUp: function(event) {
			var playerInputKey = null;
			
			if(event.keyCode == this.cfg.keyCodes.UP) {
				playerInputKey = this.game.playerInput.UP;
			}else if(event.keyCode == this.cfg.keyCodes.DOWN) {
				playerInputKey = this.game.playerInput.DOWN;
			}else if(event.keyCode == this.cfg.keyCodes.LEFT) {
				playerInputKey = this.game.playerInput.LEFT;
			}else if(event.keyCode == this.cfg.keyCodes.RIGHT) {
				playerInputKey = this.game.playerInput.RIGHT;
			}
			
			if(playerInputKey && playerInputKey.isDown) {
				playerInputKey.isDown = false;
			}
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
