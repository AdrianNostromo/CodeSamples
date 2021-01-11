/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	
	"app/util/Point"
], function(declare, Point){
	"use strict";
	var cls = declare("app.util.ArrayUtil", null, {});
	
	cls.isEntryInArraySegment = function(array, entry, entryIndice) {
		for(var i=0;i<entryIndice;i++) {
			var checkingEntry = array[i];
			if(checkingEntry === entry) {
				return true;
			}
		}
		
		return false;
	};
	
	cls.isEntryWithPropertyInArraySegment = function(array, entry, entryIndice, propertyName) {
		for(var i=0;i<entryIndice;i++) {
			var checkingEntry = array[i];
			if(checkingEntry && checkingEntry[propertyName] === entry[propertyName]) {
				return true;
			}
		}
		
		return false;
	};
	
	cls.removeEntryWithIndice = function(array, indice) {
		if(array.length > 0 && indice < array.length) {
			var entry = array[indice];
			if(array.length > 1 && indice < array.length - 1) {
				array[indice] = array[array.length - 1];
			}
			array.pop();
			
			return entry;
		}
		
		return null;
	};
	
	cls.removeEntry = function(array, entry) {
		var indice = array.indexOf(entry);
		
		if(indice < 0) {
			return null;
		}
		
		return this.removeEntryWithIndice(array, indice);
	};
	
	/**
	 * Returns a random number between min (inclusive) and max (exclusive)
	 */
	cls.isPositionInPointsArray = function(targetArray, posX, posY) {
		for(var i=0;i<targetArray.length;i++) {
			if(targetArray[i] && targetArray[i].x === posX && targetArray[i].y === posY) {
				return true;
			}
		}
		
		return false;
	};
	
	cls.cloneArrayShallow = function(src) {
		var ret;
		
		if(src) {
			ret = [];
			
			for(var i=0;i<src.length;i++) {
				ret[i] = src[i];
			}
		}
		
		return ret;
	};
	
	cls.includeEntries = function(targetArray, sourceEntriesList) {
		for(var i=0;i<sourceEntriesList.length;i++) {
			if(sourceEntriesList[i]) {
				targetArray.push(sourceEntriesList[i]);
			}
		}
	};
	
	cls.generateIndicesArray = function(length) {
		var ret = [];
		for(var i=0;i<length;i++) {
			ret[i] = i;
		}
		
		return ret;
	};
	
	return cls;
});
