/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/util/ArrayUtil",
	"app/main/util/AssetType",
	
	// Global libraries.
	"lib/fontloaderjs/webfont"
], function(declare, ModuleBase,
		ArrayUtil, AssetType
	){
	"use strict";
	return declare("app.main.assetsLoader.WebfontsLoader", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"loadAssets", cbName:"onLoadAssets", isUnique:false}
		],
		publishMsg: {
			fontAssetItemLoaded:{channel:"fontAssetItemLoaded", listenerType:"any", mustExist:true},
			
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		
		loadInstances: 0,
		
		constructor: function() {
			//void
		},
		
		onLoadAssets:function(assetsList) {
			if(!this.app) {
				this.app = this.publisher.publish(this.publishMsg.getApp);
			}
			
			if(this.loadInstances != 0) {
				// There is already a font loader in progress. Currently the fonts must be loaded in sinlge stream mode.
				
				this.postError("err 6");
				
				return ;
			}
			
			var uniqueFamilies = [];
			var uniqueUrls = [];
			
			for(var i=0;i<assetsList.length;i++) {
				var entry = assetsList[i];
				if(entry && entry.type === AssetType.WEBFONT) {
					if(!ArrayUtil.isEntryInArraySegment(uniqueFamilies, entry.family, i)) {
						uniqueFamilies.push(entry.family);
						
						this.app.assetsLoader.globalTotalAssetsToLoad++;
					}
					
					if(!ArrayUtil.isEntryInArraySegment(uniqueUrls, entry.url, i)) {
						uniqueUrls.push(entry.url);
					}
				}
			}
			
			if(uniqueFamilies.length <= 0) {
				// No fonts need loading.
				
				return ;
			}
			
			this.loadInstances++;
			
			var loaderConfig = {
				active: this.onAllFontsLoaded.bind(this),
				fontactive: this.onFontItemLoaded.bind(this),
				fontinactive: this.onFontLoadError.bind(this),
				custom: {
					families: uniqueFamilies,
				    urls: uniqueUrls
				},
				timeout: 2000
			};
			
			WebFont.load(loaderConfig);
		},
		
		onAllFontsLoaded:function() {
			//void
		},
		
		onFontItemLoaded:function() {
			this.app.assetsLoader.globalLoadedAssetsCount += 1;
			 
			this.publisher.publish(this.publishMsg.fontAssetItemLoaded);
		},
		
		onFontLoadError:function(event, bb, cc) {
			// Error loading font.
			
			// Note. The font is loaded, there is no error. Maybe some caching occurs and load is canceled.
			this.postWarning("Dev warning. Font load error.");
			
			this.onFontItemLoaded();
			
			return ;
		}
		
	});
});