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
	return declare("app.main.assetsLoader.GeneralAssetsLoader", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"loadAssets", cbName:"onLoadAssets", isUnique:false}
		],
		publishMsg: {
			assetItemLoaded:{channel:"assetItemLoaded", listenerType:"any", mustExist:true},
			
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		
		loadQueue: null,
		
		constructor: function() {
			//void
		},
		
		onLoadAssets:function(assetsList) {
			if(!this.app) {
				this.app = this.publisher.publish(this.publishMsg.getApp);
				
				this.loadQueue = new createjs.LoadQueue(true);
				this.loadQueue.on("fileload", this.onAssetItemLoaded, this);
	 			this.loadQueue.on("complete", this.onAllAssetsLoaded, this);
			}
			
			var filesList = [];
			for(var i=0;i<assetsList.length;i++) {
				var entry = assetsList[i];
				if(entry && entry.type === AssetType.JSON) {
					filesList.push({id: entry.id, src: entry.url});
					
					this.app.assetsLoader.globalTotalAssetsToLoad++;
				}
			}
			if(filesList.length <= 0) {
				return ;
			}
			
 			this.loadQueue.loadManifest(filesList);
		},
		
		onAllAssetsLoaded:function() {
			// This is called when all assets finish loading;
			
			//void
		},
		
		onAssetItemLoaded: function(event) {
			this.app.assetsLoader.globalLoadedAssetsCount += 1;
			
			if(this.app.assetsLoader.cache[event.item.id]) {
				this.postError("err 7");
				
				return null;
			}
			
			this.app.assetsLoader.cache[event.item.id] = event.result;
			
			this.publisher.publish(this.publishMsg.assetItemLoaded);
		}
		
	});
});