/**
 * Created by Adrian on 11/21/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	// global libs
	"lib/util/stats.min"
], function(declare, ModuleBase
	){
	"use strict";
	return declare("app.main.AppLoop", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"startAppLoop", cbName:"onStartAppLoop", isUnique:false}
		],
		publishMsg: {
			appLoop:{channel:"appLoop", listenerType:"list", mustExist:false},
			appLoopUserInterface:{channel:"appLoopUserInterface", listenerType:"list", mustExist:false},
			appLoopVisualFx:{channel:"appLoopVisualFx", listenerType:"list", mustExist:false},
			appLoop2:{channel:"appLoop2", listenerType:"list", mustExist:false},
			appLoop10:{channel:"appLoop10", listenerType:"list", mustExist:false},
			appLoop60:{channel:"appLoop60", listenerType:"list", mustExist:false},
			appLoopInGame:{channel:"appLoopInGame", listenerType:"any", mustExist:false},
			appLoopInGameUI:{channel:"appLoopInGameUI", listenerType:"any", mustExist:false},
			renderGameStage:{channel:"renderGameStage", listenerType:"any", mustExist:false},
			
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},
		
		app:null,
		
		firstFrameTime:-1,
		lastFrameTime:-1,
		
		fpsCounter:null,
		
		// start at -1,-2,-10 so it is activated on the first frame;
		frameSkipCount2:-1,
		deltaMsCounterLoop2: 0,
		
		frameSkipCount10:-1,
		deltaMsCounterLoop10: 0,
		
		frameSkipCount60:-1,
		deltaMsCounterLoop60: 0,
		
		constructor: function() {
			//void
		},
		
		onStartAppLoop:function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			if(this.app.dev.profileFPS) {
				this.app.dev.fps = 0;
				
				this.fpsCounter = {
					startTime : 0,
					frameNumber : 0,
					getFPS : function(){
						this.frameNumber++;
						var d = new Date().getTime(),
							currentTime = ( d - this.startTime ) / 1000,
							result = Math.floor( ( this.frameNumber / currentTime ) );

						if( currentTime > 1 ){
							this.startTime = new Date().getTime();
							this.frameNumber = 0;
						}
						
						return result;
					}	
				};
			}
			
			if(this.app.dev.enableStats) {
				this.stats = new Stats();
				
				document.body.appendChild( this.stats.domElement );
				this.stats.domElement.style.position = "absolute";
				this.stats.domElement.style.top = "0px";
			}
			
			window.requestAnimationFrame(this.handleAnimationFrame.bind(this));
		},
		
		handleAnimationFrame:function(timestamp) {
			if(this.app.dev.enableStats) {
				this.stats.begin();
			}
			
			// do math and call appLoop;
			if(this.lastFrameTime < 0) {
				this.firstFrameTime = timestamp;
				this.lastFrameTime = timestamp;
			}
			
			var appTime = timestamp - this.firstFrameTime;
			
			var deltaMS = timestamp - this.lastFrameTime;
			
			// Snap the fps to a minimum of 30.
			deltaMS = Math.min(1000/30, deltaMS);
			
			// Clip the fps at 60.
			//deltaMS = 17;// Math.ceil(1000 / 60) = 17;
			
			var deltaS = deltaMS / 1000;
			
			this.app.appTime = appTime;
			this.lastFrameTime = timestamp;
			
			if(this.fpsCounter) {
				this.app.dev.fps = this.fpsCounter.getFPS();
			}
			
			this.publisher.publish(this.publishMsg.appLoop, appTime, deltaMS, deltaS);
			this.publisher.publish(this.publishMsg.appLoopUserInterface, appTime, deltaMS, deltaS);
			this.publisher.publish(this.publishMsg.appLoopVisualFx, appTime, deltaMS, deltaS);
			this.publisher.publish(this.publishMsg.appLoopInGame, appTime, deltaMS, deltaS);
			this.publisher.publish(this.publishMsg.appLoopInGameUI, appTime, deltaMS, deltaS);
			
			this.deltaMsCounterLoop2 += deltaMS;
			this.frameSkipCount2 = (this.frameSkipCount2 + 1) % 2;
			if(this.frameSkipCount2 === 0) {
				this.publisher.publish(this.publishMsg.appLoop2, appTime, this.deltaMsCounterLoop2, this.deltaMsCounterLoop2 / 1000);
				
				this.deltaMsCounterLoop2 = 0;
			}
			
			this.deltaMsCounterLoop10 += deltaMS;
			this.frameSkipCount10 = (this.frameSkipCount10 + 1) % 10;
			if(this.frameSkipCount10 === 0) {
				this.publisher.publish(this.publishMsg.appLoop10, appTime, this.deltaMsCounterLoop10, this.deltaMsCounterLoop10 / 1000);
			}
			
			this.deltaMsCounterLoop60 += deltaMS;
			this.frameSkipCount60 = (this.frameSkipCount60 + 1) % 60;
			if(this.frameSkipCount60 === 0) {
				this.publisher.publish(this.publishMsg.appLoop60, appTime, this.deltaMsCounterLoop60, this.deltaMsCounterLoop60 / 1000);
				
				if(this.fpsCounter) {
					console.log("Fps: " + this.app.dev.fps);
				}
			}
			
			this.publisher.publish(this.publishMsg.renderGameStage);
			
			window.requestAnimationFrame(this.handleAnimationFrame.bind(this));
			
			if(this.app.dev.enableStats) {
				this.stats.end();
			}
		},
		
		destroy: function() {
			//void
			
			this.__proto__.__proto__.destroy.call(this);
		}

	});
});
