/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/main/util/SoundType",
	"app/main/util/AssetType",
	"app/main/util/SoundTemplate"
], function(declare, ModuleBase,
	SoundType, AssetType, SoundTemplate
){
	"use strict";
	return declare("app.main.AudioManager", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"initializeAudio", cbName:"onInitializeAudio", isUnique:true},
			{channel:"toggleAudioUnmuteState", cbName:"onToggleAudioUnmuteState", isUnique:false},
			{channel:"assetsLoadComplete", cbName:"onAssetsLoadComplete", isUnique:false}
		],
		publishMsg: {
			audioUnmuteStateChanged:{channel:"audioUnmuteStateChanged", listenerType:"list", mustExist:false},
			
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},
		
		app:null,
		
		audioSprites: null,
		
		constructor: function() {
			//void
		},
		
		onInitializeAudio: function () {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.audioSprites = {};
			
			this.app.audio = {
				isUnmuted: true,
				playSoundTemplate: this.onPlaySoundTemplate.bind(this)
			};
			
			if (!createjs.Sound.initializeDefaultPlugins()) {
				// Sound play is not possible in this browser.
				
				this.postError("err_004");
				
				return null;
			}
		},
		
		onToggleAudioUnmuteState:function() {
			if(this.app.audio.isUnmuted) {
				this.app.audio.isUnmuted = false;
			}else {
				this.app.audio.isUnmuted = true;
			}
			
			// asd_002;// update sounds.
			
			this.publisher.publish(this.publishMsg.audioUnmuteStateChanged, this.app.audio.isUnmuted);
		},
		
		onAssetsLoadComplete:function(assetsGroupID, groupConfig) {
			if(!groupConfig.processedAssets) {
				return ;
			}
			
			for(var i=0;i<groupConfig.processedAssets.length;i++) {
				var entry = groupConfig.processedAssets[i];
				if(entry && entry.type === AssetType.AUDIO_SPRITE) {
					// Get the config file.
					var config = this.app.assetsLoader.cache[entry.configId];
					if(!config) {
						this.postError("err 7");
						
						return null;
					}
					
					if(this.audioSprites[entry.audioId]) {
						this.postError("err 7");
						
						return null;
					}
					
					this.audioSprites[entry.audioId] = config;
				}
			}
		},
		
		onPlaySoundTemplate: function(template) {
			if(!template.isInit) {
				var asConfig = this.audioSprites[template.soundID];
				if(!asConfig) {
					this.postError("err 7");
					
					return null;
				}
				
				var soundSprite = asConfig.sprite[template.spriteID];
				if(!soundSprite) {
					this.postError("err 7");
					
					return null;
				}
				
				template.playProps.delay = soundSprite[0];
				template.playProps.duration = soundSprite[1];
				
				template.isInit = true;
			}
			
			var soundInstance = createjs.Sound.play(template.soundID, template.playProps);
			if(soundInstance.playState === createjs.SoundJS.PLAY_FAILED) {
				this.postError("err 7");
				
				return null;
			}
			
			// asd_002;// save sound in instance;
			
			return soundInstance;
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
		/*asd_002
		renderer:null,
		audioConfigs:null,
		
		audioVolume:null,
		
		// .key.marker:Number;
		lastSoundPlayTime:null,
		
		soundOverlapThresholdMS:100,
		
		constructor: function() {
			this.subscribeListeners = {
				appLoaded:{channel:"appLoaded", callback:this.onAppLoaded, isUnique:false, autoHook:true, context:this, hook:null},
				getAudioUnmuteStatusForType:{channel:"getAudioUnmuteStatusForType", callback:this.onGetAudioUnmuteStatusForType, isUnique:true, autoHook:true, context:this, hook:null},
				toggleAudioUnmuteStatusForType:{channel:"toggleAudioUnmuteStatusForType", callback:this.onToggleAudioUnmuteStatusForType, isUnique:true, autoHook:true, context:this, hook:null},
				getAudioVolumeForType:{channel:"getAudioVolumeForType", callback:this.onGetAudioVolumeForType, isUnique:true, autoHook:true, context:this, hook:null},
				setAudioVolumeForType:{channel:"setAudioVolumeForType", callback:this.onSetAudioVolumeForType, isUnique:true, autoHook:true, context:this, hook:null},
				startAudioFX:{channel:"startAudioFX", callback:this.onStartAudioFX, isUnique:true, autoHook:true, context:this, hook:null}
			}
			
			this.audioVolume = {};
			
			this.audioVolume[SoundType.INGAME_SFX] = {};
			this.audioVolume[SoundType.INGAME_SFX].volume = 1;
			this.audioVolume[SoundType.INGAME_SFX].unmuteStatus = true;
			
			this.audioVolume[SoundType.BG_MUSIC] = {};
			this.audioVolume[SoundType.BG_MUSIC].volume = 1;
			this.audioVolume[SoundType.BG_MUSIC].unmuteStatus = true;
			
			this.activeAudioSpritesContainer = {};
			this.lastSoundPlayTime = {};
		},
		
		moduleConnectOccured: function() {
			//void
		},
		
		onAppLoaded:function() {
			this.renderer = this.publisher.publish(this.publishMsg.getRenderer);
			
			this.audioConfigs = this.publisher.publish(this.publishMsg.getCacheAsset, "json", "audioConfigs");
			if(!this.audioConfigs) {
				this.postError("err_004");
				
				return null;
			}
			
			var soundsKeysList = this.publisher.publish(this.publishMsg.getCacheKeysFromPhaserCacheType, Phaser.Cache.SOUND);
			
			if(soundsKeysList.length > 1) {
				// there should be only 1 audio sprite item;
				this.postError("err_004");
				
				return null;
			}
			
			var audioSprite;
			for(var i=0;i<soundsKeysList.length;i++) {
				var sountItemKey = soundsKeysList[i];
				
				this.activeAudioSpritesContainer[sountItemKey] = audioSprite = new Phaser.AudioSprite(this.renderer, sountItemKey);
				for (var marker in audioSprite.sounds) {
					if (audioSprite.sounds.hasOwnProperty(marker)) {
						var soundItem = audioSprite.sounds[marker];
						
						soundItem.allowMultiple = false;
					}
				}
			}
		},
		
		onGetAudioUnmuteStatusForType:function(soundType) {
			return this.audioVolume[soundType].unmuteStatus;
		},
		
		onToggleAudioUnmuteStatusForType:function(soundType) {
			if(this.audioVolume[soundType].unmuteStatus) {
				this.audioVolume[soundType].unmuteStatus = false;
				
				// stop all sound of this type;
				this.stopAllSoundsOfType(soundType);
			}else {
				this.audioVolume[soundType].unmuteStatus = true;
			}
			
			this.publisher.publish(this.publishMsg.audioUnmuteStatusChanged, soundType, this.audioVolume[soundType].unmuteStatus);
		},
		
		/**
		* @soundType:SoundType
		* @return curent volume for sound type;
		**//*
		onGetAudioVolumeForType:function(soundType) {
			return this.audioVolume[soundType].volume;
		},
		
		stopAllSoundsOfType:function(soundType) {
			console.log("asd_002 stop sounds because muted;")
			//asd_002;
		},
		
		/**
		* @soundType:SoundType
		* @return curent volume for sound type;
		**//*
		onSetAudioVolumeForType:function(soundType, volume) {
			this.audioVolume[soundType].volume = volume;
			
			return this.audioVolume[soundType].volume;
		},
		
		/**
		* @playConfig: {key, marker, volume}
		**//*
		onStartAudioFX:function(soundType, key, marker, volume) {
			if(!this.audioVolume[soundType].unmuteStatus) {
				return false;
			}
			
			var tm = Date.now();
			
			if(!this.lastSoundPlayTime[key]) {
				this.lastSoundPlayTime[key] = {};
			}else if(this.lastSoundPlayTime[key][marker] && this.lastSoundPlayTime[key][marker] + this.soundOverlapThresholdMS >= tm) {
				return null;
			}
			this.lastSoundPlayTime[key][marker] = tm;
			
			if(!this.activeAudioSpritesContainer[key]) {
				this.postError("err_004");
				
				return null;
			}
			
			var finalVolume;
			
			if(this.audioConfigs[key] && this.audioConfigs[key][marker]) {
				if(!this.audioConfigs[key][marker].hasOwnProperty("volume")) {
					this.postError("err_004");
					
					return null;
				}
				
				finalVolume = this.audioConfigs[key][marker]["volume"] * volume;
			}else {
				finalVolume = volume;
			}
			finalVolume *= this.audioVolume[soundType].volume;
			
			this.activeAudioSpritesContainer[key].play(marker, finalVolume);
		},
		
		destroy: function() {
			for(var i=this.activeParticlesList.length - 1;i>=0;i--) {
				var particleItem = this.activeParticlesList[i];
				if(particleItem) {
					particleItem.destroy();
					
					this.activeParticlesList[i] = null;
				}
			}
			
			this.__proto__.__proto__.destroy.call(this);
		}
		*/
		
	});
});