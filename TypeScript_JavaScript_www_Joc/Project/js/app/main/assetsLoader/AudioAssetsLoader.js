/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/util/ArrayUtil",
	"app/main/util/AssetType"
], function(declare, ModuleBase,
		ArrayUtil, AssetType
	){
	"use strict";
	return declare("app.main.assetsLoader.AudioAssetsLoader", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"loadAssets", cbName:"onLoadAssets", isUnique:false}
		],
		publishMsg: {
			audioAssetItemLoaded:{channel:"audioAssetItemLoaded", listenerType:"any", mustExist:true},
			
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		
		constructor: function() {
			//void
		},
		
		onLoadAssets:function(assetsList) {
			if(!this.app) {
				this.app = this.publisher.publish(this.publishMsg.getApp);
				
				createjs.Sound.addEventListener("fileload", this.onAudioAssetItemLoaded.bind(this));
			}
			
			var sounds = [];
			for(var i=0;i<assetsList.length;i++) {
				var entry = assetsList[i];
				if(entry && entry.type === AssetType.AUDIO) {
					sounds.push({id: entry.name, src: entry.url});
					
					this.app.assetsLoader.globalTotalAssetsToLoad++;
				}
			}
			if(sounds.length <= 0) {
				return ;
			}
			
			createjs.Sound.alternateExtensions = ["mp3"];
			createjs.Sound.registerSounds(sounds);
		},
		
		onAllAudioAssetsLoaded:function() {
			// This is called when all assets finish loading;
			
			//void
		},
		
		onAudioAssetItemLoaded: function() {
			this.app.assetsLoader.globalLoadedAssetsCount += 1;
			 
			this.publisher.publish(this.publishMsg.audioAssetItemLoaded);
		}
		
	});
});