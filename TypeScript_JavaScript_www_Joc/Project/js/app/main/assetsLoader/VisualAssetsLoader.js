/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/util/ArrayUtil",
	"lib/pixijs/pixi",
	"app/main/util/AssetType"
], function(declare, ModuleBase,
		ArrayUtil, PIXI, AssetType
	){
	"use strict";
	return declare("app.main.assetsLoader.VisualAssetsLoader", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"loadAssets", cbName:"onLoadAssets", isUnique:false}
		],
		publishMsg: {
			visualAssetItemLoaded:{channel:"visualAssetItemLoaded", listenerType:"any", mustExist:true},
			
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		
		constructor: function() {
			//void
		},
		
		onLoadAssets:function(assetsList) {
			if(!this.app) {
				this.app = this.publisher.publish(this.publishMsg.getApp);
				
				PIXI.loader.on('load', this.onVisualAssetItemLoaded.bind(this));
			}
			
			var isNewLoadRequired = false;
			var finalUniqueList = [];
			for(var i=0;i<assetsList.length;i++) {
				var entry = assetsList[i];
				if(entry && entry.type === AssetType.SPRITE_SHEET) {
					if(!ArrayUtil.isEntryWithPropertyInArraySegment(finalUniqueList, entry, i, "url")) {
						finalUniqueList.push(entry);
						
						this.app.assetsLoader.globalTotalAssetsToLoad++;
						
						var preparedUrl = entry.url;
						if(preparedUrl.indexOf("#SCALE_VERSION#") >= 0) {
							preparedUrl = preparedUrl.replace("#SCALE_VERSION#", this.app.assetsVariantName);
						}
						PIXI.loader.add(entry.name, preparedUrl, undefined, this.onNewAssetProcessed.bind(this));
						
						isNewLoadRequired = true;
					}
				}
			}
			
			if(isNewLoadRequired) {
				PIXI.loader.load(this.onAllVisualAssetsLoaded.bind(this));
			}
		},
		
		onAllVisualAssetsLoaded:function() {
			// This is called when all assets finish loading;
			
			//void
		},
		
		onVisualAssetItemLoaded: function() {
			// This is called before the asset is processed and saved.
			
			//void;
		},
		
		onNewAssetProcessed: function() {
			this.app.assetsLoader.globalLoadedAssetsCount += 1;
			 
			this.publisher.publish(this.publishMsg.visualAssetItemLoaded);
		}
		
	});
});