/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	
	"app/util/tween/EaseFunctions",
	"app/ingamea/util/EntityVisualType",
	"app/util/ObjectUtil"
], function(declare, 
	EaseFunctions, EntityVisualType, ObjectUtil
){
	"use strict";
	var cls = declare("app.util.VisualUtil", null, {});
	
	cls.createVLayoutVisuals = function(vElemsLayer, vLayoutLayer, app, moduleBranch, parentVElemsLayer, sharedVLayoutAssetIDs, customValues) {
		// Use recursivity to enumerate all vLayout layers and create the needed structure and visuals.
		if(!vLayoutLayer) {
			App.postError("Err 4.");
			
			return false;
		}
		for(var i=0;i<vLayoutLayer.length;i++) {
			var node = vLayoutLayer[i];
			
			if(node.visual) {
				vElemsLayer[node.id] = this.createTemplateVisual(node.visual, app, moduleBranch, sharedVLayoutAssetIDs, customValues);
				
				if(node.initParams) {
					if(!this.processNodeParams(vElemsLayer, app.appRect, node, node.initParams, true, null, customValues)) {
						App.postError("Err 4.");
						
						return null;
					}
				}
				
				if(node.visual.parentID) {
					this.attachVElemsToParent(vElemsLayer[node.id], node.visual.parentID, vElemsLayer, parentVElemsLayer);
				}
			}
			
			if("list" in node) {
				vElemsLayer[node.id] = {isVContainer: true};
				
				if(!this.createVLayoutVisuals(vElemsLayer[node.id], node.list, app, moduleBranch, vElemsLayer, sharedVLayoutAssetIDs, customValues)) {
					App.postError("Err 4.");
					
					return false;
				}
			}
		}
		
		return true;
	};
	
	cls.createTemplateVisual = function(visualTemplate, app, moduleBranch, sharedVLayoutAssetIDs, customValues) {
		var visual;
		
		// Prepare the assetID.
		var finalAssetID = null;
		if("assetId" in visualTemplate) {
			if(sharedVLayoutAssetIDs) {
				finalAssetID = this.prepareAssetID(visualTemplate.assetId, sharedVLayoutAssetIDs);
			}else {
				finalAssetID = visualTemplate.assetId;
			}
		}
		var extraData = null;
		if("extraData" in visualTemplate) {
			extraData = visualTemplate.extraData;
		}
		
		visual = app.popAsset(moduleBranch, visualTemplate.type, finalAssetID, extraData);
		
		return visual;
	};
	
	cls.prepareAssetID = function(assetID, sharedVLayoutAssetIDs) {
		var finalAssetID = assetID;
		
		if(sharedVLayoutAssetIDs) {
			for(var i=0;i<sharedVLayoutAssetIDs.length;i++) {
				var entry = sharedVLayoutAssetIDs[i];
				
				if(finalAssetID.indexOf(entry.id) >= 0) {
					finalAssetID = finalAssetID.replace(entry.id, entry.value);
				}
			}
		}
		
		return finalAssetID;
	};
	
	cls.attachVElemsToParent = function(visual, parentID, vElemsLayer, parentVElemsLayer) {
		if(parentID) {
			var parent;
			
			if(Array.isArray(parentID)) {
				if(parentID[0] != "_parent" || parentID.length !== 2) {
					App.postError("Err 4.");
					
					return null;
				}
				
				parent = parentVElemsLayer[parentID[1]];
			}else {
				parent = vElemsLayer[parentID];
			}
			
			if(!parent) {
				App.postError("Err 4.");
				
				return null;
			}
			
			parent.addChild(visual);
		}
	};
	
	cls.updateLayout = function(vElems, vLayout, appSize, parentLayer, updateStaticValues, stateStatus, customValues) {
		var appRect = {
			x: 0,
			y: 0,
			width: appSize.width,
			height: appSize.height
		};
		
		if((stateStatus.stateEnd !== "sOut" || stateStatus.completionPercent < 1) && !vElems.container.parent) {
			// Add to parent.
			
			parentLayer.addChild(vElems.container);
		}
		
		this.applyLayoutToVElems(vElems, vLayout, appRect, updateStaticValues, stateStatus, customValues);
		
		if(stateStatus.stateEnd === "sOut" && stateStatus.completionPercent >= 1 && vElems.container.parent) {
			// Remove from parent.
			
			vElems.container.parent.removeChild(vElems.container);
		}
	};
	
	cls.applyLayoutToVElems = function(vElems, vLayout, appRect, updateStaticValues, stateStatus, customValues) {
		var currentElementsNode = vElems;
		for(var i=0;i<vLayout.length;i++) {
			var node = vLayout[i];
			
			if("params" in node) {
				if(!this.processNodeParams(vElems, appRect, node, node.params, updateStaticValues, stateStatus, customValues)) {
					App.postError("Err 4.");
					
					return false;
				}
			}
			
			if("list" in node) {
				if(!this.applyLayoutToVElems(vElems[node.id], node.list, appRect, updateStaticValues, stateStatus, customValues)) {
					App.postError("Err 4.");
					
					return false;
				}
			}
			
			if("_refreshTextStyle" in node && node._refreshTextStyle) {
				vElems[node.id].style = vElems[node.id].style;
			}
		}
		
		return true;
	};
	
	cls.processNodeParams = function(vElems, appRect, node, paramsList, updateStaticValues, stateStatus, customValues) {
		for(var i=0;i<paramsList.length;i++) {
			var param = paramsList[i];
			if(param) {
				this.processNodeParamEntry(vElems, appRect, vElems[node.id], param, updateStaticValues, stateStatus, customValues);
			}
		}
		
		return true;
	};
	
	cls.processNodeParamEntry = function(vElems, appRect, selfVisual, param, updateStaticValues, stateStatus, customValues) {
		if("functionName" in param) {
			selfVisual[param.functionName]();
		}else if("prop" in param) {
			var n;
			if(!param.val.hasStates && updateStaticValues) {
				// Update the static value.
				
				n = this.computeValue(selfVisual, param.val, appRect, vElems, customValues);
			}else if(param.val.hasStates && stateStatus) {
				// Update the animated value;
				
				var finalStateStart = this.processStateName(param.val, stateStatus.stateStart);
				if(!finalStateStart) {
					App.postError("Err 4.");
					
					return false;
				}
				var finalStateEnd = this.processStateName(param.val, stateStatus.stateEnd);
				if(!finalStateEnd) {
					App.postError("Err 4.");
					
					return false;
				}
				
				var startParamVal = param.val[finalStateStart];
				var endParamVal = param.val[finalStateEnd];
				
				var valStart = this.computeValue(selfVisual, param.val[finalStateStart], appRect, vElems, customValues);
				var valEnd = this.computeValue(selfVisual, param.val[finalStateEnd], appRect, vElems, customValues);
				
				var endDuration = 1;
				var endDelay = 0;
				if(typeof endParamVal !== "number") {
					if("duration" in endParamVal) {
						endDuration = endParamVal.duration;
					}
					if("delay" in endParamVal) {
						endDelay = endParamVal.delay;
					}
				}
				
				var completionPercent = (stateStatus.completionPercent - endDelay) / endDuration;
				completionPercent = Math.max(0, Math.min(1, completionPercent));
				
				// Get the optional target ease function name or use the linear one if none found.
				var easeFunctionParamName = stateStatus.stateEnd + "EaseName";
				if(!(easeFunctionParamName in param.val)) {
					easeFunctionParamName = "defaultEaseName";
				}
				
				if(easeFunctionParamName in param.val) {
					var easeFunctioName = param.val[easeFunctionParamName];
					if(!easeFunctioName) {
						App.postError("Err 4.");
						
						return false;
					}
					
					var easeFunction = EaseFunctions[param.val[easeFunctionParamName]];
					if(!easeFunction) {
						App.postError("Err 4.");
						
						return false;
					}
					
					n = easeFunction(completionPercent, valStart, valEnd - valStart, 1);
				}else {
					n = valStart + (valEnd - valStart) * completionPercent;
				}
			}else {
				return true;
			}
			
			if(Array.isArray(param.prop)) {
				if(!ObjectUtil.setCompositeProperty(selfVisual, param.prop, n)) {
					App.postError("Err 4.");
					
					return false;
				}
			}else if(param.prop.indexOf("_") == 0) {
				if(param.prop === "_widthHeight") {
					selfVisual.width = n;
					selfVisual.scale.y = selfVisual.scale.x;
				}else if(param.prop === "_heightWidth") {
					selfVisual.height = n;
					selfVisual.scale.x = selfVisual.scale.y;
				}else {
					App.postError("Err 4.");
					
					return false;
				}
			}else {
				if(Array.isArray(param.prop)) {
					ObjectUtil.setCompositeProperty(selfVisual, param.prop, n);
				}else {
					selfVisual[param.prop] = n;
				}
			}
		}
		
		return true;
	};
	
	cls.processStateName = function(targetVal, stateName) {
		var finalStateStart = stateName;
		if(!targetVal.hasOwnProperty(finalStateStart)) {
			if(finalStateStart.indexOf("sIn") == 0 && finalStateStart != "sIn") {
				finalStateStart = "sIn";
			}else if(finalStateStart.indexOf("sOut") == 0 && finalStateStart != "sOut") {
				finalStateStart = "sOut";
			}else {
				return null;
			}
			
			if(!targetVal.hasOwnProperty(finalStateStart)) {
				return null;
			}
		}
		
		return finalStateStart;
	},
	
	cls.computeValue = function(selfVisual, paramValue, appRect, vElems, customValues) {
		var ret;
		
		if((typeof paramValue === "number") || (typeof paramValue === "boolean")) {
			ret = paramValue;
		}else {
			if("min" in paramValue) {
				ret = Math.min(this.computeValue(selfVisual, paramValue.min[0], appRect, vElems, customValues), this.computeValue(selfVisual, paramValue.min[1], appRect, vElems, customValues));
			}else if("max" in paramValue) {
				ret = Math.max(this.computeValue(selfVisual, paramValue.max[0], appRect, vElems, customValues), this.computeValue(selfVisual, paramValue.max[1], appRect, vElems, customValues));
			}else if("add" in paramValue) {
				if(paramValue.add.length > 2) {
					var ret = this.computeValue(selfVisual, paramValue.add[0], appRect, vElems, customValues);
					for(var i=1;i<paramValue.add.length;i++) {
						ret = ret + this.computeValue(selfVisual, paramValue.add[i], appRect, vElems, customValues);
					}
				}else {
					ret = this.computeValue(selfVisual, paramValue.add[0], appRect, vElems, customValues) + this.computeValue(selfVisual, paramValue.add[1], appRect, vElems, customValues);
				}
			}else if("subtract" in paramValue) {
				if(paramValue.subtract.length > 2) {
					ret = this.computeValue(selfVisual, paramValue.subtract[0], appRect, vElems, customValues);
					for(var i=1;i<paramValue.subtract.length;i++) {
						ret = ret - this.computeValue(selfVisual, paramValue.subtract[i], appRect, vElems, customValues);
					}
				}else {
					ret = this.computeValue(selfVisual, paramValue.subtract[0], appRect, vElems, customValues) - this.computeValue(selfVisual, paramValue.subtract[1], appRect, vElems, customValues);
				}
			}else if("multiply" in paramValue) {
				if(paramValue.multiply.length > 2) {
					var ret = this.computeValue(selfVisual, paramValue.multiply[0], appRect, vElems, customValues);
					for(var i=1;i<paramValue.multiply.length;i++) {
						ret = ret * this.computeValue(selfVisual, paramValue.multiply[i], appRect, vElems, customValues);
					}
				}else {
					ret = this.computeValue(selfVisual, paramValue.multiply[0], appRect, vElems, customValues) * this.computeValue(selfVisual, paramValue.multiply[1], appRect, vElems, customValues);
				}
			}else if("divide" in paramValue) {
				if(paramValue.divide.length > 2) {
					var ret = this.computeValue(selfVisual, paramValue.divide[0], appRect, vElems, customValues);
					for(var i=1;i<paramValue.divide.length;i++) {
						ret = ret / this.computeValue(selfVisual, paramValue.divide[i], appRect, vElems, customValues);
					}
				}else {
					ret = this.computeValue(selfVisual, paramValue.divide[0], appRect, vElems, customValues) / this.computeValue(selfVisual, paramValue.divide[1], appRect, vElems, customValues);
				}
			}else {
				var targetObject = null;
				if("targetID" in paramValue) {
					if(paramValue.targetID === "_app") {
						targetObject = appRect;
					}else if(paramValue.targetID === "_area") {
						targetObject = vElems;
					}else if(paramValue.targetID === "_self") {
						targetObject = selfVisual;
					}else if(paramValue.targetID === "_customValues") {
						targetObject = customValues;
					}else {
						targetObject = vElems[paramValue.targetID];
					}
				}
				
				if("value" in paramValue) {
					if("param" in paramValue) {
						App.postError("Err 4.");
						
						return null;
					}
					
					ret = paramValue.value;
				}else {
					if(!("param" in paramValue)) {
						App.postError("Err 4.");
						
						return null;
					}
					if(typeof paramValue.param === "string") {
						ret = targetObject[paramValue.param];
					}else if(Array.isArray(paramValue.param)) {
						ret = ObjectUtil.getCompositeProperty(targetObject, paramValue.param);
					}else {
						App.postError("Err 4.");
						
						return null;
					}
					
					if("percent" in paramValue) {
						ret = ret * paramValue.percent;
					}
				}
			}
			
			if("posOffset" in paramValue) {
				ret += this.computeValue(selfVisual, paramValue.posOffset, appRect, vElems, customValues);
			}
			
			if("mathOperation" in paramValue) {
				if(paramValue.mathOperation === "floor") {
					ret = Math.floor(ret);
				}else if(paramValue.mathOperation === "ceil") {
					ret = Math.ceil(ret);
				}else if(paramValue.mathOperation === "round") {
					ret = Math.round(ret);
				}else if(paramValue.mathOperation === "sin") {
					ret = Math.sin(ret);
				}else if(paramValue.mathOperation === "cos") {
					ret = Math.cos(ret);
				}else {
					App.postError("Err 4.");
					
					return null;
				}
			}
		}
		
		if(paramValue.logValue) {
			console.log("VLayout value: " + ret);
		}
		
		return ret;
	};
	
	return cls;
});

/* Old code from previous version;
	
	// @DEPRECATED ; use vLayout insted.
	cls.createConfiguredVisual = function(app, moduleBranch, visualConfig, parent) {
		var visual = null;
		
		if(visualConfig.type === EntityVisualType.SPRITE) {
			var texID = visualConfig.id;
			visual = app.popAsset(moduleBranch, "sprite", texID);
		}else if(visualConfig.type === EntityVisualType.SPRITE_SHEET) {
			// implement corect version with a default animation flag;
			/*
			visual = app.popAsset(moduleBranch, "movieClip", "blankMC_###");
			
			var textureGroups = {};
			for (var key in visualConfig.animationsList) {
				if (visualConfig.animationsList.hasOwnProperty(key)) {
					var value = visualConfig.animationsList[key];
					
					var texID = visualConfig.id + "/" + key + "_###";
					var texturesList = app.getMovieClipAssetTextures(texID);
					
					textureGroups[key] = {
						config: value,
						textures: texturesList
					};
				}
			}
			
			vElems.visual.textures = textureGroups[visualConfig.defaultAnimation].textures;
			
			entity.modules.visual = new EntityModuleVisual_SpriteSheet(visualConfig, entity, vElems, vElems.visual.textures, textureGroups, parentLayer);
			*//*
		}else {
			App.postError("err 6");
			
			return false;
		}
		
		visual.anchor.set(visualConfig.anchor.x, visualConfig.anchor.y);
		visual.width = visualConfig.size.width;
		visual.height = visualConfig.size.height;
		
		if(parent) {
			parent.addChild(visual);
		}
		
		return visual;
	};
	
			if(param.prop.indexOf("_") == 0) {
				
			}else {
				var paramEntry = node[paramName];
				var n = this.computeValue(node, paramEntry, appRect, vElems);
				vElems[node.id][paramName] = n;
			}
			
			
			
			/*
			// size must be processed before position;
			if("size" in node) {
				var sizeParam = node.size;
				if("wh" in sizeParam) {
					var n = this.computeValue(node, sizeParam.wh, appRect, vElems);
					
					vElems[node.id].width = n;
					vElems[node.id].scale.y = vElems[node.id].scale.x;
				}else if("hw" in sizeParam) {
					var n = this.computeValue(node, sizeParam.hw, appRect, vElems);
					
					vElems[node.id].height = n;
					vElems[node.id].scale.x = vElems[node.id].scale.y;
				}else {
					var isHeightProcessed = false;
					// so height is processed first when whe width uses the self height as input;
					if("hPre" in sizeParam) {
						vElems[node.id].height = this.computeValue(node, sizeParam.h, appRect, vElems);
						
						isHeightProcessed = true;
					}
					
					if("w" in sizeParam) {
						vElems[node.id].width = this.computeValue(node, sizeParam.w, appRect, vElems);
					}else {
						App.postError("Err 4.");
						
						return null;
					}
					
					if(!isHeightProcessed) {
						if("h" in sizeParam) {
							vElems[node.id].height = this.computeValue(node, sizeParam.h, appRect, vElems);
						}else {
							App.postError("Err 4.");
							
							return null;
						}
					}
				}
			}
			
			// scale must be processed before position;
			if("scale" in node) {
				var scaleParam = node.scale;
				if("x" in scaleParam) {
					vElems[node.id].scale.x = this.computeValue(node, scaleParam.x, appRect, vElems);
				}else {
					App.postError("Err 4.");
					
					return null;
				}
				
				if("y" in scaleParam) {
					vElems[node.id].scale.y = this.computeValue(node, scaleParam.y, appRect, vElems);
				}else {
					App.postError("Err 4.");
					
					return null;
				}
			}
			
			// font must be processed before position;
			if("fontSize" in node) {
				var fontSize = Math.max(1, Math.round(this.computeValue(node, node.fontSize, appRect, vElems)));
				
				vElems[node.id].fontSize = fontSize;
			}
			if("strokeThickness" in node) {
				var strokeThickness = Math.max(1, Math.round(this.computeValue(node, node.strokeThickness, appRect, vElems)));
				
				vElems[node.id].strokeThickness = strokeThickness;
			}
			
			// font must be processed before position;
			if("font" in node) {
				var fontNode = node.font;
				
				var style = vElems[node.id].style;
				
				var isFontChange = false;
				if("size" in fontNode) {
					var n = Math.max(1, Math.round(this.computeValue(node, fontNode.size, appRect, vElems)));
					
					style.font = fontNode.fontTemplate.replace("#fontSize#", n);
					
					isFontChange = true;
				}
				
				if("strokeThickness" in fontNode) {
					var n = Math.max(1, Math.round(this.computeValue(node, fontNode.strokeThickness, appRect, vElems)));
					
					style.strokeThickness = n;
					
					isFontChange = true;
				}
				
				if(!isFontChange) {
					App.postError("Err 4.");
					
					return null;
				}
				
				vElems[node.id].setStyle(style);
			}
			
			// position must be processed last;
			if("pos" in node) {
				var posParam = node.pos;
				if("x" in posParam) {
					if(posParam.x !== false) {
						vElems[node.id].x = this.computeValue(node, posParam.x, appRect, vElems);
					}
				}else if("marginRight" in posParam) {
					var n = this.computeValue(node, posParam.marginRight, appRect, vElems);
					
					vElems[node.id].x = n - vElems[node.id].width * (1 - vElems[node.id].anchor.x);
				}else {
					App.postError("Err 4.");
					
					return null;
				}
				
				if("y" in posParam) {
					if(posParam.y !== false) {
						vElems[node.id].y = this.computeValue(node, posParam.y, appRect, vElems);
					}
				}else if("marginBottom" in posParam) {
					var n = this.computeValue(node, posParam.marginBottom, appRect, vElems);
					
					vElems[node.id].y = n - vElems[node.id].height * (1 - vElems[node.id].anchor.y);
				}else {
					App.postError("Err 4.");
					
					return null;
				}
			}*/