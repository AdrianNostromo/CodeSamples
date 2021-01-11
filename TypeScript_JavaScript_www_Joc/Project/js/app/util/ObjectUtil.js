/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	
	"app/util/tween/EaseFunctions",
	"app/ingamea/util/EntityVisualType"
], function(declare, 
	EaseFunctions, EntityVisualType
	){
	"use strict";
	var cls = declare("app.util.ObjectUtil", null, {});
	
	cls.poolCompositePropertyOnCompositeObject = function(baseObj, subObjAddress, composedPropName) {
		var finalObject = this.getCompositeObject(baseObj, subObjAddress);
		
		if(!finalObject) {
			App.postError("Err 4.");
			
			return null;
		}
		
		return this.getCompositeProperty(finalObject, composedPropName);
	};
	
	cls.setCompositePropertyOnCompositeObject = function(baseObj, subObjAddress, composedPropName, value) {
		var finalObject = this.getCompositeObject(baseObj, subObjAddress);
		
		if(!finalObject) {
			App.postError("Err 4.");
			
			return false;
		}
		
		return this.setCompositeProperty(finalObject, composedPropName, value);
	};
	
	cls.setCompositePropertyDeltaOnCompositeObject = function(baseObj, subObjAddress, composedPropName, valueDelta) {
		var finalObject = this.getCompositeObject(baseObj, subObjAddress);
		
		if(!finalObject) {
			App.postError("Err 4.");
			
			return false;
		}
		
		return this.setCompositePropertyDelta(finalObject, composedPropName, valueDelta);
	};
	
	cls.getCompositeObject = function(baseObj, subObjAddress) {
		var finalObject = baseObj;
		
		if(subObjAddress) {
			if(Array.isArray(subObjAddress)) {
				for(var i=0;i<subObjAddress.length;i++) {
					var newChildName = subObjAddress[i];
					
					finalObject = finalObject[newChildName];
				}
			}else {
				finalObject = finalObject[subObjAddress];
			}
		}
		
		if(!finalObject) {
			App.postError("Err 4.");
			
			return null;
		}
		
		return finalObject;
	};
	
	cls.getCompositeProperty = function(targetObject, composedPropName) {
		var obj = null;
		var propName = null;
		
		if(Array.isArray(composedPropName)) {
			for(var i=0;i<composedPropName.length;i++) {
				var newPropName = composedPropName[i];
				
				if(!obj) {
					obj = targetObject;
				}else {
					obj = obj[propName];
				}
				
				propName = newPropName;
			}
		}else {
			obj = targetObject;
			propName = composedPropName;
		}
		
		if(!obj || !propName) {
			App.postError("Err 4.");
			
			return false;
		}
		
		return obj[propName];
	};
	
	cls.callCompositeFunctionOnCompositeObject = function(baseObj, subObjAddress, functionName) {
		var finalObject = this.getCompositeObject(baseObj, subObjAddress);
		if(!finalObject) {
			App.postError("Err 4.");
			
			return false;
		}
		
		return this.callCompositeFunction(finalObject, functionName);
	};
	
	cls.callCompositeFunction = function(targetObject, functionName) {
		var obj = null;
		var propName = null;
		
		if(Array.isArray(functionName)) {
			for(var i=0;i<functionName.length;i++) {
				var newPropName = functionName[i];
				
				if(!obj) {
					obj = targetObject;
				}else {
					obj = obj[propName];
				}
				
				propName = newPropName;
			}
		}else {
			obj = targetObject;
			propName = functionName;
		}
		
		if(!obj || !propName) {
			App.postError("Err 4.");
			
			return false;
		}
		
		obj[propName]();
		
		return true;
	};
	
	cls.setCompositeProperty = function(targetObject, composedPropName, value) {
		var obj = null;
		var propName = null;
		
		if(Array.isArray(composedPropName)) {
			for(var i=0;i<composedPropName.length;i++) {
				var newPropName = composedPropName[i];
				
				if(!obj) {
					obj = targetObject;
				}else {
					obj = obj[propName];
				}
				
				propName = newPropName;
			}
		}else {
			obj = targetObject;
			propName = composedPropName;
		}
		
		if(!obj || !propName) {
			App.postError("Err 4.");
			
			return false;
		}
		
		obj[propName] = value;
		
		return true;
	};
	
	cls.setCompositePropertyDelta = function(targetObject, composedPropName, valueDelta) {
		var obj = null;
		var propName = null;
		
		if(Array.isArray(composedPropName)) {
			for(var i=0;i<composedPropName.length;i++) {
				var newPropName = composedPropName[i];
				
				if(!obj) {
					obj = targetObject;
				}else {
					obj = obj[propName];
				}
				
				propName = newPropName;
			}
		}else {
			obj = targetObject;
			propName = composedPropName;
		}
		
		if(!obj || !propName) {
			App.postError("Err 4.");
			
			return false;
		}
		
		obj[propName] += valueDelta;
		
		return true;
	};
	
	return cls;
});
