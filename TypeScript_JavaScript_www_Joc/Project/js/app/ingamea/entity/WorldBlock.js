/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare
){
	"use strict";
	return declare("app.ingamea.entity.WorldBlock", null, {
		
		blocksInterractor: null,
		
		type: null,
		version: null,
		
		config: null,
		
		vElems: null,
		
		gPos: null,
		
		// this is used to animate in the blocks at start.
		activeAnimation: null,
		
		vSettle: null,
		hSettle: null,
		
		isTouchDown: false,
		
		/*
			.{
				delayMS,
				eventType,
				eventExtraData
			}
		*/
		autoDestruction: null,
		
		// If true, the block was destroyed.
		isDestroyed: false,
		
		constructor: function(blocksInterractor, type, version, config, vElems, gPos) {
			this.blocksInterractor = blocksInterractor;
			
			this.type = type;
			this.version = version;
			
			this.config = config;
			
			this.vElems = vElems;
			
			this.gPos = gPos;
			
			this.vElems.rootVisual.interactive = true;
			this.vElems.rootVisual.on('touchstart', this.onTouchStart, this);
			this.vElems.rootVisual.on('touchend', this.onTouchEnd, this);
			this.vElems.rootVisual.on('touchendoutside', this.onTouchEndOutside, this);
			this.vElems.rootVisual.on('touchmove', this.onTouchMove, this);
			this.vElems.rootVisual.on('tap', this.onTouchTap, this);
			
			this.vElems.rootVisual.on('mousedown', this.onTouchStart, this);
		},
		
		onTouchStart: function(event) {
			this.isTouchDown =  true;
			
			this.blocksInterractor.dispatch("blockTouchStart", this, event);
		},
		
		onTouchEnd: function(event) {
			this.isTouchDown =  false;
			
			this.blocksInterractor.dispatch("blockTouchEndOver", this, event);
			this.blocksInterractor.dispatch("blockTouchEndAnywhere", this, event);
		},
		
		onTouchEndOutside: function(event) {
			this.isTouchDown =  false;
			
			this.blocksInterractor.dispatch("blockTouchEndOutside", this, event);
			this.blocksInterractor.dispatch("blockTouchEndAnywhere", this, event);
		},
		
		onTouchMove: function(event) {
			if(this.isTouchDown) {
				this.blocksInterractor.dispatch("blockTouchDrag", this, event);
			}
		},
		
		onTouchTap: function(event) {
			this.blocksInterractor.dispatch("blockTouchTap", this, event);
		},
		
		getVPos: function() {
			return {
				x: this.vElems.rootVisual.x,
				y: this.vElems.rootVisual.y
			};
		},
		
		destroyBlock: function(settleBlocks, resetSettleVelocities) {
			this.blocksInterractor.dispatch("blockDestroyRequest", this, settleBlocks, resetSettleVelocities);
		},
		
		activateEvent: function(eventType, extraData) {
			this.blocksInterractor.dispatch("blockActivateEventRequest", this, eventType, extraData);
		},
		
		destroy: function() {
			//void
		}

	});
});
