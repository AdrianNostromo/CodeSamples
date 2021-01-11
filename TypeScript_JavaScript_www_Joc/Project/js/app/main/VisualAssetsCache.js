/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/util/MathUtil",
	"lib/pixijs/pixi",
	"app/extension/PixiX"
], function(declare, ModuleBase, 
		MathUtil, PIXI, PixiX
	){
	"use strict";
	return declare("app.main.VisualAssetsCache", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"initializeAssetsCache", cbName:"onIinitializeAssetsCache", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},

		app: null,

		// .group.type.id:{cacheList, cacheCount}
		// type: "sprite", "spriteButton";
		cacheData:null,

		// .mcID:[];
		movieClipTexturesLists:null,
		
		constructor: function() {
			//void
		},

		onIinitializeAssetsCache: function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.cacheData = {};
			this.movieClipTexturesLists = {};
			
			if(!this.app.cache) {
				this.app.cache = {};
			}
			
			this.app.popAsset = this.borrowVisualAsset.bind(this);
			this.app.pushAsset = this.releaseVisualAsset.bind(this);
			this.app.getMovieClipAssetTextures = this.getMovieClipTextures.bind(this);
			
			this.app.cache.releaseVElems = this.releaseVElems.bind(this);
			this.app.cache.clearVisualCacheGroup = this.clearVisualCacheGroup.bind(this);
		},
		
		// id: frameID of pixi sprite;
		borrowVisualAsset:function(group, type, id, extraData) {
			if(!id) {
				id = 0;
			}
			
			if(!this.cacheData[group]) {
				this.cacheData[group] = {};
			}
			if(!this.cacheData[group][type]) {
				this.cacheData[group][type] = {};
			}
			
			if(!this.cacheData[group][type][id]) {
				this.cacheData[group][type][id] = {
					cacheList:[],
					cacheCount:0
				};
			}
			
			if(this.cacheData[group][type][id].cacheCount > 0) {
				this.cacheData[group][type][id].cacheCount--;
				
				var obj = this.cacheData[group][type][id].cacheList[this.cacheData[group][type][id].cacheCount];
				
				this.cacheData[group][type][id].cacheList[this.cacheData[group][type][id].cacheCount] = null;
				
				if(obj instanceof PIXI.DisplayObject) {
					obj.position.set(0, 0);
					obj.scale.set(1, 1);
					obj.alpha = 1;
				}
				if(obj instanceof PIXI.Sprite) {
					obj.anchor.set(0, 0);
				}
				if(obj instanceof PIXI.extras.MovieClip) {
					obj.gotoAndStop(0);
				}
				
				return obj;
			}
			
			// no item exists, create new visual;
			var obj;
			if(type === "json") {
				obj = PIXI.JsonCache[id];
				
				// object is disposable, recycle will ignore it;
			}else if(type === "movieClip") {
				var mcFrames = this.getMovieClipTextures(id);
				
				obj = new PIXI.extras.MovieClip(mcFrames);
				if(!obj.userData) {
					obj.userData = {};
				}
				obj.userData.cacheID = id;
			}else if(type === "sprite") {
				
				obj = PIXI.Sprite.fromFrame(id);
				if(!obj.userData) {
					obj.userData = {};
				}
				obj.userData.cacheID = id;
			}else if(type === "spriteButton") {
				var textures = {
					up: null,
					down: null,
					mouseHover: null,
					mouseDown: null,
					disabled: null,
					
					selected_up: null,
					selected_down: null,
					selected_mouseHover: null,
					selected_mouseDown: null,
					selected_disabled: null
				};
				
				// start : Normal textures.
				textures.up = PIXI.Texture.fromFrame(id + "up");
				textures.down = PIXI.Texture.fromFrame(id + "down");
				
				if(PIXI.utils.TextureCache[id + "mouseHover"] && PIXI.utils.TextureCache[id + "mouseDown"]) {
					textures.mouseHover = PIXI.Texture.fromFrame(id + "mouseHover");
					textures.mouseDown = PIXI.Texture.fromFrame(id + "mouseDown");
				}else {
					textures.mouseHover = textures.down;
					textures.mouseDown = textures.up;
				}
				
				if(PIXI.utils.TextureCache[id + "disabled"]) {
					textures.disabled = PIXI.Texture.fromFrame(id + "disabled");
				}
				// end : Normal textures.
				
				// start : Selected textures.
				if(PIXI.utils.TextureCache[id + "selected_up"] && PIXI.utils.TextureCache[id + "selected_down"]) {
					textures.selected_up = PIXI.Texture.fromFrame(id + "selected_up");
					textures.selected_down = PIXI.Texture.fromFrame(id + "selected_down");
					
					if(PIXI.utils.TextureCache[id + "selected_mouseHover"] && PIXI.utils.TextureCache[id + "selected_mouseDown"]) {
						textures.selected_mouseHover = PIXI.Texture.fromFrame(id + "selected_mouseHover");
						textures.selected_mouseDown = PIXI.Texture.fromFrame(id + "selected_mouseDown");
					}else {
						textures.selected_mouseHover = textures.selected_down;
						textures.selected_mouseDown = textures.selected_up;
					}
				}
				// end : Selected textures.
				
				obj = new PixiX.SpriteButton(id, textures);
			}else if(type === "scaleButton") {
				var tex = PIXI.Texture.fromFrame(id);
				
				obj = new PixiX.ScaleButton(id, tex, extraData.baseWidth, extraData.baseHeight, extraData.hoverScale, extraData.downScale);
			}else if(type === "text") {
				obj = new PIXI.Text(extraData.text, extraData.style);
			}else if(type === "container") {
				obj = new PIXI.Container();
			}else if(type === "graphics") {
				obj = new PIXI.Graphics();
			}else {
				this.app.postError("Err 1.");
				
				return null;
			}
			
			return obj;
		},
		
		getMovieClipTextures:function(mcID) {
			var mcTextures = this.movieClipTexturesLists[mcID];
			
			if(!mcTextures) {
				if(mcID.indexOf("#") < 0) {
					this.app.postError("Error - cache release of unhandled asset type.");
					
					return null;
				}
				var counterLength = mcID.lastIndexOf("#") - mcID.indexOf("#") + 1;
				if(counterLength < 1) {
					this.app.postError("Error - cache release of unhandled asset type.");
					
					return null;
				}
				
				var counterString = "";
				for(var i=0;i<counterLength;i++) {
					counterString = counterString + "#";
				}
				
				var tex;
				var frameID;
				
				if(PIXI.utils.TextureCache[mcID.replace(counterString, MathUtil.numberToZeroPadded(0, counterLength))]) {
					frameID = 0;
				}else if(PIXI.utils.TextureCache[mcID.replace(counterString, MathUtil.numberToZeroPadded(1, counterLength))]) {
					frameID = 1;
				}else {
					this.app.postError("Error - cache release of unhandled asset type.");
					
					return null;
				}
				
				mcTextures = [];
				while(tex = PIXI.utils.TextureCache[mcID.replace(counterString, MathUtil.numberToZeroPadded(frameID, counterLength))]) {
					mcTextures.push(tex);
					
					frameID++;
				}
				
				if(mcTextures.length <= 0) {
					this.app.postError("Error - cache release of unhandled asset type.");
					
					return null;
				}
				
				this.movieClipTexturesLists[mcID] = mcTextures;
			}
			
			return mcTextures;
		},
		
		// ASD_002;// add code to check if the asset is already released;
		releaseVisualAsset:function(group, obj) {
			var type;
			var id;
			
			if(obj.parent && "removeChild" in obj.parent) {
				obj.parent.removeChild(obj);
			}
			
			if("removeAllListeners" in obj) {
				obj.removeAllListeners();
			}
			
			if(obj instanceof Object) {
				// is a json object or text file, ignore them;
				
				type = null;
				
				id = null;
			}else if(obj instanceof PIXI.extras.MovieClip) {
				type = "movieClip";
				
				id = obj.userData.cacheID;
			}else if(obj instanceof PIXI.Sprite) {
				type = "sprite";
				
				id = obj.userData.cacheID;
			}else if(obj instanceof PixiX.SpriteButton) {
				type = "spriteButton";
				
				obj.cbcClick = null;
				
				obj.texture = obj.textureUp;
				
				id = obj.id;
			}else if(obj instanceof PixiX.ScaleButton) {
				type = "scaleButton";
				
				obj.cbcClick = null;
				
				id = obj.id;
			}else if(obj instanceof PIXI.Text) {
				type = "text";
				
				id = 0;
			}else if(obj instanceof PIXI.Container) {
				type = "container";
				
				id = 0;
			}else if(obj instanceof PIXI.Graphics) {
				type = "graphics";
				
				id = 0;
			}else {
				this.app.postError("Error - cache release of unhandled asset type.");
				
				return null;
			}
			
			if(type && id) {
				this.cacheData[group][type][id].cacheList[this.cacheData[group][type][id].cacheCount] = obj;
				this.cacheData[group][type][id].cacheCount++;
			}
		},

		releaseVElems:function(group, vElems) {
			if(vElems) {
				for (var key in vElems) {
					if (vElems.hasOwnProperty(key)) {
						var value = vElems[key];
						if(value && key != "isHelper" && key != "isVContainer") {
							if(value.isHelper) {
								// void
							}else if(value.isVContainer) {
								// value if a vElems child;
								this.app.cache.releaseVElems(group, value);
							}else {
								this.app.pushAsset(group, value);
							}
							
							vElems[key] = null;
						}
					}
				}
			}
		},

		clearVisualCacheGroup:function(group) {
			// enum all properties of that group and clear them;
			if(this.cacheData && this.cacheData[group]) {
				for (var type in this.cacheData[group]) {
					if (this.cacheData[group].hasOwnProperty(type)) {
						for (var cacheID in this.cacheData[group][type]) {
							if (this.cacheData[group][type].hasOwnProperty(cacheID)) {
								var cacheGroupEntry = this.cacheData[group][type][cacheID];
								
								while(cacheGroupEntry.cacheCount > 0) {
									cacheGroupEntry.cacheCount--;
									
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