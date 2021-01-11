/**
 * Created by Adrian on 11/18/2014.
 */
// Used for mediator 
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	return declare("app.util.list.DoublyLinkedList", null, {
		
		cachedEntriesList:null,
		cachedEntriesCount:0,
		
		// contains the head;
		head:null,
		tail:null,
		length:0,
		
		constructor: function() {
			this.cachedEntriesList = [];
			this.cachedEntriesCount = 0;
		},
		
		pushNew:function(data) {
			var node;
			
			if(this.cachedEntriesCount > 0) {
				this.cachedEntriesCount--;
				node = this.cachedEntriesList[this.cachedEntriesCount];
				
				node.data = data;
				node.isAlive = true;
			}else {
				node = {
					list:this,
					prev:null,
					next:null,
					isAlive:true,
					data:data
				};
			}
			
			if(!this.head) {
				this.head = node;
				this.tail = node;
			}else {
				this.tail.next = node;
				
				node.prev = this.tail;
				this.tail = node;
			}
			
			this.length++;
			
			return node;
		},
		
		removeNode:function(node, bufferRemovedNode) {
			if(!node.prev) {
				// remove first element;
				
				this.head = node.next;
				
				if(!this.head) {
					this.tail = null;
				}else {
					this.head.prev = null;
				}
			}else if(!node.next) {
				// remove last element;
				
				this.tail = node.prev;
				this.tail.next = null;
			}else {
				// remove a middle node;
				
				node.prev.next = node.next;
				node.next.prev = node.prev;
			}
			
			var removedData = node.data;
			
			node.prev = null;
			node.next = null;
			node.isAlive = false;
			node.data = null;
			
			this.cachedEntriesList[this.cachedEntriesCount] = node;
			this.cachedEntriesCount++;
			
			this.length--;
			
            return removedData;
		}
		
	});
});
