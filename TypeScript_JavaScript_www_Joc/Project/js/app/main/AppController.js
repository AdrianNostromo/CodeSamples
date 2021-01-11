/**
 * Created by Adrian on 11/21/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/main/view/util/ViewType",
	"app/main/assetsLoader/AssetsGroupID",
	"app/ingamea/util/UnitID",
	"app/ingamea/util/LevelID",
	"app/util/SafeNumber",
	
	"app/main/util/SoundTemplate"
], function(declare, ModuleBase,
	ViewType, AssetsGroupID, UnitID, LevelID, SafeNumber,
	SoundTemplate
){
	"use strict";
	return declare("app.main.AppController", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"getApp", cbName:"onGetApp", isUnique:true},
			{channel:"initializeApp", cbName:"onInitializeApp", isUnique:true},
			{channel:"assetsLoadComplete", cbName:"onAssetsLoadComplete", isUnique:false}
		],
		publishMsg: {
			initializeScreenMonitor:{channel:"initializeScreenMonitor", listenerType:"list", mustExist:true},
			initializeAppSize:{channel:"initializeAppSize", listenerType:"list", mustExist:true},
			initializeLogHandler:{channel:"initializeLogHandler", listenerType:"list", mustExist:true},
			initializeAssetsCache:{channel:"initializeAssetsCache", listenerType:"list", mustExist:true},
			initializeProfileData:{channel:"initializeProfileData", listenerType:"any", mustExist:false},
			initializeAudio:{channel:"initializeAudio", listenerType:"unique", mustExist:true},
			initializeRenderer:{channel:"initializeRenderer", listenerType:"unique", mustExist:true},
			initializeAppDependantComponents:{channel:"initializeAppDependantComponents", listenerType:"list", mustExist:false},
			poolRemoteData:{channel:"poolRemoteData", listenerType:"list", mustExist:false},
			loadAppAssets:{channel:"loadAppAssets", listenerType:"unique", mustExist:true},
			startAppLoop:{channel:"startAppLoop", listenerType:"list", mustExist:false},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false}
		},
		
		/*
			.dev:{profileFPS:Boolean, enableStats:Boolean}
			.appCanvas:dom_node
			.screenSize:{width:Number, height:Number}
			.appSize:{width:Number, height:Number}
			.stage:PIXI.Container
			.log:{postLog, postWarning, postError}
			.layers:{...}
			.appTime:Number;
			.assetsVariantName:String;
		*/
		app: null,
		
		constructor: function() {
			//void
		},

		onGetApp: function() {
			return this.app;
		},

		onInitializeApp:function(appCanvas) {
			this.app = {
				game: null
			};
			
			this.app.dev = {};
			this.app.dev.profileFPS = false;
			this.app.dev.enableStats = false;
			this.app.dev.debugPhysicsVisuals = false;
			this.app.dev.enableDevConsole = true;// ASD_DEV;
			
			this.app.appCanvas = appCanvas;
			
			this.publisher.publish(this.publishMsg.initializeScreenMonitor);
			this.publisher.publish(this.publishMsg.initializeProfileData);
			this.publisher.publish(this.publishMsg.initializeAudio);
			this.publisher.publish(this.publishMsg.initializeRenderer);
			this.publisher.publish(this.publishMsg.initializeAppSize);
			this.publisher.publish(this.publishMsg.initializeLogHandler);
			this.publisher.publish(this.publishMsg.initializeAssetsCache);
			this.publisher.publish(this.publishMsg.initializeAppDependantComponents);
			this.publisher.publish(this.publishMsg.startAppLoop);
			
			if(this.app.dev.enableDevConsole) {
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.DEV_CONSOLE, "sIn", false, null, 0, null, null);
			}
			
			this.postDevLog("Preloader asset load - start.");
			
			this.publisher.publish(this.publishMsg.loadAppAssets, AssetsGroupID.PRELOADER);
		},
		
		onAssetsLoadComplete:function(assetsGroupID, groupConfig) {
			if(assetsGroupID === AssetsGroupID.PRELOADER) {
				this.postDevLog("Preloader asset load - finish.");
				this.postDevLog("Game asset load - start.");
				
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.PRELOADER, "sIn", false, null, 0, null, null);
				
				this.publisher.publish(this.publishMsg.loadAppAssets, AssetsGroupID.GAME);
			}else if(assetsGroupID === AssetsGroupID.GAME) {
				this.postDevLog("Game asset load - finish.");
				
				// asd_003;// add a bg music.
				// this.app.audio.playSoundTemplate(SoundTemplate.BG_MUSIC);
				
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.PRELOADER, "sOut", true, null, 0, null, null);
				
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.MAIN, "sIn", true, null, 0, null, null);
				
				// ASD_dev
				/*
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.SHOP, "sIn", true, null, 0, null, null);
				*//*
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.IN_GAME, "sIn", false, {
					levelID: LevelID.Terra,
					snLevelIndice: new SafeNumber(1)
				}, 0, null, null);*/
			}else {
				this.postError("Error. AssetsLoader.002.");
				
				return ;
			}
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
