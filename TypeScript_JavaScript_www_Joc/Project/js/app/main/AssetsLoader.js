/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/util/ArrayUtil",
	"app/main/assetsLoader/VisualAssetsLoader",
	"app/main/assetsLoader/AudioAssetsLoader",
	"app/main/assetsLoader/WebfontsLoader",
	"app/main/assetsLoader/GeneralAssetsLoader",
	
	"app/main/assetsLoader/AssetsGroupID",
	"app/main/util/AssetType"
], function(declare, ModuleBase,
		ArrayUtil, VisualAssetsLoader, AudioAssetsLoader, WebfontsLoader, GeneralAssetsLoader, 
		AssetsGroupID, AssetType
	){
	"use strict";
	return declare("app.main.AssetsLoader", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: [
			VisualAssetsLoader,
			AudioAssetsLoader,
			WebfontsLoader,
			GeneralAssetsLoader
		],
		subscribeListeners:[
			{channel:"loadAppAssets", cbName:"onLoadAppAssets", isUnique:true},
			{channel:"visualAssetItemLoaded", cbName:"onVisualAssetItemLoaded", isUnique:true},
			{channel:"audioAssetItemLoaded", cbName:"onAudioAssetItemLoaded", isUnique:true},
			{channel:"fontAssetItemLoaded", cbName:"onFontAssetItemLoaded", isUnique:true},
			{channel:"assetItemLoaded", cbName:"onAssetItemLoaded", isUnique:true}
		],
		publishMsg: {
			loadAssets:{channel:"loadAssets", listenerType:"any", mustExist:true},
			assetsLoadProgress:{channel:"assetsLoadProgress", listenerType:"any", mustExist:false},
			assetsLoadComplete:{channel:"assetsLoadComplete", listenerType:"any", mustExist:false},
			
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		
		activeAssetsGroupID: null,
		
		// Use this flag to avoid instant loading of assets on load call before all loaders have registered.
		enableLoadCompletionCheck: false,
		
		assetGroups: {
			PRELOADER: {
				externalFiles: [
					{type: AssetType.SPRITE_SHEET, name: "ts_preloader", url: "assets/visual/ts_preloader#SCALE_VERSION#.json"}
				],
				processedAssets: [
					//void
				]
			},
			GAME: {
				externalFiles: [
					{type: AssetType.SPRITE_SHEET, name: "ts_views", url: "assets/visual/ts_views#SCALE_VERSION#.json"},
					{type: AssetType.SPRITE_SHEET, name: "ts_views2", url: "assets/visual/ts_views2#SCALE_VERSION#.json"},
					{type: AssetType.SPRITE_SHEET, name: "ts_inGame", url: "assets/visual/ts_inGame#SCALE_VERSION#.json"},
					{type: AssetType.SPRITE_SHEET, name: "ts_inGame2", url: "assets/visual/ts_inGame2#SCALE_VERSION#.json"},
					
					{type: AssetType.JSON, id: "as_main_json", url: "assets/audio/as_main.json"},
					
					{type: AssetType.AUDIO, name: "as_main", url: "assets/audio/as_main.ogg"},
					
					{type: AssetType.WEBFONT, family: "WFDosis:800:latin", url: "./assets/font/fonts.css"},
					{type: AssetType.WEBFONT, family: "WFDosis:800:latin-ext", url: "./assets/font/fonts.css"},
					{type: AssetType.WEBFONT, family: "WFOpenSans:800:latin", url: "./assets/font/fonts.css"},
					{type: AssetType.WEBFONT, family: "WFOpenSans:800:latin-ext", url: "./assets/font/fonts.css"}
				],
				processedAssets: [
					{type: AssetType.AUDIO_SPRITE, audioId: "as_main", configId: "as_main_json"}
				]
			}
		},
		
		constructor: function() {
			//void
		},
		
		onLoadAppAssets:function(activeAssetsGroupID) {
			this.activeAssetsGroupID = activeAssetsGroupID;
			
			if(!this.app) {
				this.app = this.publisher.publish(this.publishMsg.getApp);
				
				this.app.assetsLoader = {
					globalLoadedAssetsCount: 0,
					globalTotalAssetsToLoad: 0,
					cache: {}
				};
			}
			
			var group = this.assetGroups[this.activeAssetsGroupID];
			
			this.enableLoadCompletionCheck = false;
			
			if(group.externalFiles.length > 0) {
				this.publisher.publish(this.publishMsg.loadAssets, group.externalFiles);
			}
			
			this.enableLoadCompletionCheck = true;
			
			// Check if preloading finished and dispatch a update message.
			this.assetItemLoaded();
		},
		
		onVisualAssetItemLoaded: function() {
			this.assetItemLoaded();
		},
		
		onAudioAssetItemLoaded: function() {
			this.assetItemLoaded();
		},
		
		onFontAssetItemLoaded: function() {
			this.assetItemLoaded();
		},
		
		onAssetItemLoaded: function() {
			this.assetItemLoaded();
		},
		
		assetItemLoaded: function() {
			if(!this.enableLoadCompletionCheck) {
				return ;
			}
			
			if(this.app.assetsLoader.globalLoadedAssetsCount > this.app.assetsLoader.globalTotalAssetsToLoad) {
				this.postError("Error. AssetsLoader.002.");
				
				return ;
			}
			
			var n = 0;
			if(this.app.assetsLoader.globalTotalAssetsToLoad != 0) {
				n = this.app.assetsLoader.globalLoadedAssetsCount / this.app.assetsLoader.globalTotalAssetsToLoad;
			}
			
			// Dispatch progress event.
			this.publisher.publish(this.publishMsg.assetsLoadProgress, this.app.assetsLoader.globalTotalAssetsToLoad, this.app.assetsLoader.globalLoadedAssetsCount, n);
			
			if(this.app.assetsLoader.globalTotalAssetsToLoad == this.app.assetsLoader.globalLoadedAssetsCount) {
				// Dispatch complete event when needed.
				var groupID = this.activeAssetsGroupID;
				var groupConfig = this.assetGroups[groupID];
				this.activeAssetsGroupID = null;
				
				this.publisher.publish(this.publishMsg.assetsLoadComplete, groupID, groupConfig);
			}
		}
		
	});
});