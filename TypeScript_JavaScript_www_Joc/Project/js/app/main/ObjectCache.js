/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"lib/pixijs/pixi",
	"app/extension/PixiX"
], function(declare, ModuleBase, 
		PIXI, PixiX
	){
	"use strict";
	return declare("app.main.ObjectCache", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"initializeAssetsCache", cbName:"onInitializeAssetsCache", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},
		
		app: null,
		
		// .group.type:{cacheList, cacheCount}
		// type: "text";
		cacheData:null,
		
		constructor: function() {
			//void
		},
		
		onInitializeAssetsCache: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.cacheData = {};
			
			if(!this.app.cache) {
				this.app.cache = {};
			}
			
			this.app.cache.borrowObject = this.borrowObject.bind(this);
			this.app.cache.releaseObject = this.releaseObject.bind(this);
			
			this.app.cache.clearObjectCacheGroup = this.clearObjectCacheGroup.bind(this);
		},
		
		// id: frameID of pixi sprite;
		borrowObject:function(group, type) {
			if(!this.cacheData[group]) {
				this.cacheData[group] = {};
			}
			if(!this.cacheData[group][type]) {
				this.cacheData[group][type] = {
					cacheList:[],
					cacheCount:0
				};
			}
			
			if(this.cacheData[group][type].cacheCount > 0) {
				this.cacheData[group][type].cacheCount--;
				
				var obj = this.cacheData[group][type].cacheList[this.cacheData[group][type].cacheCount];
				
				this.cacheData[group][type].cacheList[this.cacheData[group][type].cacheCount] = null;
				
				return obj;
			}
			
			return null;
		},
		
		releaseObject:function(group, type, obj) {
			if(obj.parent) {
				obj.parent.removeChild(obj);
			}
			
			this.cacheData[group][type].cacheList[this.cacheData[group][type].cacheCount] = obj;
			this.cacheData[group][type].cacheCount++;
		},
		
		clearObjectCacheGroup:function(group) {
			// enum all properties of that group and clear them;
			if(this.cacheData && this.cacheData[group]) {
				for (var type in this.cacheData[group]) {
					if (this.cacheData[group].hasOwnProperty(type)) {
						for (var cacheID in this.cacheData[group][type]) {
							if (this.cacheData[group][type].hasOwnProperty(cacheID)) {
								var cacheGroupEntry = this.cacheData[group][type][cacheID];
								
								while(cacheGroupEntry.cacheCount > 0) {
									cacheGroupEntry.cacheCount--;
									
									if(obj instanceof PIXI.Sprite) {
										//void
									}else if(obj instanceof PixiX.SpriteButton) {
										//void
									}else {
										this.app.postError("Error - cache destructon of unhandled asset type.");
										
										return null;
									}
									
									cacheGroupEntry.cacheList[cacheGroupEntry.cacheCount] = null;
								}
							}
						}
					}
				}
			}
		}
		
	});
});