/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	
	"lib/pixijs/pixi"
], function(declare, 
		PIXI
	){
	"use strict";
	var cls = declare("app.extension.PixiX", null, {});
	
	// START : Button;
	//ASD_002;// implement a delay so the touch down hangs for aproximative 200ms before restoring to up state; Use a implemented delay function that uses appLoop and is kinda global. App.startTimer(200, {callback, context});
	cls.SpriteButton = function(id, textures) {
	    PIXI.Sprite.call(this, textures.up);
	    
	    this.id = id;
	    
	    this.textures = textures;
	    
	    this.interactive = true;
	    
	    this.on('touchstart', this.onTouchStart);
	    this.on('touchend', this.onTouchEnd);
	    this.on('touchendoutside', this.onTouchEndOutside);
	    this.on('tap', this.onTouchTap);
	    
	    this.on('mouseout', this.onMouseOut);
	    this.on('mouseover', this.onMouseOver);
	    this.on('mousedown', this.onMouseDown);
	    this.on('mouseup', this.onMouseUp);
	    this.on('mouseupoutside', this.onMouseUpOutside);
	    this.on('click', this.onMouseClick);
	};
	
	// constructor
	cls.SpriteButton.prototype = Object.create(PIXI.Sprite.prototype);
	cls.SpriteButton.prototype.constructor = PIXI.Button;
	
	// (target);
	cls.SpriteButton.prototype.cbcClick = null;
	
	cls.SpriteButton.prototype.id = null;
	
	/*
		{
			up
			down
			mouseHover
			mouseDown
			disabled
			
			selected_up
			selected_down
			selected_mouseHover
			selected_mouseDown
			selected_disabled
		}
	*/
	cls.SpriteButton.prototype.textures = null;
	
	cls.SpriteButton.prototype._isEnabled = true;
	cls.SpriteButton.prototype._isSelected = false;
	
	cls.SpriteButton.prototype.isTouchDown = false;
	cls.SpriteButton.prototype.isMouseHover = false;
	cls.SpriteButton.prototype.isMouseDown = false;
	
	cls.SpriteButton.prototype.getIsEnabled = function() {
		return this._isEnabled;
	};
	
	cls.SpriteButton.prototype.setIsEnabled = function(isEnabled) {
		if(this._isEnabled != isEnabled) {
			this._isEnabled = isEnabled;
			
			this.updateState();
		}
	};
	
	cls.SpriteButton.prototype.getIsSelected = function() {
		return this._isSelected;
	};
	
	cls.SpriteButton.prototype.setIsSelected = function(isSelected) {
		if(this._isSelected != isSelected) {
			this._isSelected = isSelected;
			
			this.updateState();
		}
	};
	
	cls.SpriteButton.prototype.onTouchStart = function(event) {
		this.isTouchDown = true;
		this.updateState();
	};
	
	cls.SpriteButton.prototype.onTouchEnd = function(event) {
		this.isTouchDown = false;
		this.updateState();
	};
	
	cls.SpriteButton.prototype.onTouchEndOutside = function(event) {
		this.isTouchDown = false;
		this.updateState();
	};
	
	cls.SpriteButton.prototype.onTouchTap = function(event) {
		if(this._isEnabled) {
			if(this.cbcClick) {
				this.cbcClick.callback.call(this.cbcClick.context, event);
			}
		}
	};
	
	cls.SpriteButton.prototype.onMouseOut = function(event) {
		this.isMouseHover = false;
		this.updateState();
	};
	
	cls.SpriteButton.prototype.onMouseOver = function(event) {
		this.isMouseHover = true;
		this.updateState();
	};
	
	cls.SpriteButton.prototype.onMouseDown = function(event) {
		this.isMouseDown = true;
		this.updateState();
	};
	
	cls.SpriteButton.prototype.onMouseUp = function(event) {
		this.isMouseDown = false;
		this.updateState();
		
		if(this._isEnabled) {
			if(this.cbcClick) {
				this.cbcClick.callback.call(this.cbcClick.context, event);
			}
		}
	};
	
	cls.SpriteButton.prototype.onMouseUpOutside = function(event) {
		this.isMouseDown = false;
		this.updateState();
	};
	
	cls.SpriteButton.prototype.onMouseClick = function(event) {
		if(this._isEnabled) {
			if(this.cbcClick) {
				this.cbcClick.callback.call(this.cbcClick.context, event);
			}
		}
	};
	
	cls.SpriteButton.prototype.updateState = function() {
		if(!this._isEnabled) {
			if(!this._isSelected) {
				if(this.texture != this.textures.disabled) {
					this.texture = this.textures.disabled;
				}
			}else {
				if(this.texture != this.textures.selected_disabled) {
					this.texture = this.textures.selected_disabled;
				}
			}
		}else if(this.isTouchDown) {
			if(!this._isSelected) {
				if(this.texture != this.textures.down) {
					this.texture = this.textures.down;
				}
			}else {
				if(this.texture != this.textures.selected_down) {
					this.texture = this.textures.selected_down;
				}
			}
		}else if(this.isMouseDown) {
			if(!this._isSelected) {
				if(this.texture != this.textures.mouseDown) {
					this.texture = this.textures.mouseDown;
				}
			}else {
				if(this.texture != this.textures.selected_mouseDown) {
					this.texture = this.textures.selected_mouseDown;
				}
			}
		}else if(this.isMouseHover) {
			if(!this._isSelected) {
				if(this.texture != this.textures.mouseHover) {
					this.texture = this.textures.mouseHover;
				}
			}else {
				if(this.texture != this.textures.selected_mouseHover) {
					this.texture = this.textures.selected_mouseHover;
				}
			}
		}else {
			// There is no interraction with the button;
			
			if(!this._isSelected) {
				if(this.texture != this.textures.up) {
					this.texture = this.textures.up;
				}
			}else {
				if(this.texture != this.textures.selected_up) {
					this.texture = this.textures.selected_up;
				}
			}
		}
	};
	// END : Button;
	
	// START : ScaleButton;
	cls.ScaleButton = function(id, texture, baseWidth, baseHeight, hoverScale, downScale) {
	    PIXI.Sprite.call(this, texture);
	    
	    this.id = id;
	    
	    this.baseWidth = baseWidth;
	    this.baseHeight = baseHeight;
	    this.hoverScale = hoverScale;
	    this.downScale = downScale;
	    
	    this.interactive = true;
	    
	    this.on('touchstart', this.onTouchStart);
	    this.on('touchend', this.onTouchEnd);
	    this.on('touchendoutside', this.onTouchEndOutside);
	    this.on('tap', this.onTouchTap);
	    
	    this.on('mouseout', this.onMouseOut);
	    this.on('mouseover', this.onMouseOver);
	    this.on('mousedown', this.onMouseDown);
	    this.on('mouseup', this.onMouseUp);
	    this.on('mouseupoutside', this.onMouseUpOutside);
	    this.on('click', this.onMouseClick);
	};
	
	// constructor
	cls.ScaleButton.prototype = Object.create( PIXI.Sprite.prototype );
	cls.ScaleButton.prototype.constructor = PIXI.ScaleButton;
	
	// (target);
	cls.ScaleButton.prototype.cbcClick = null;
	
	cls.ScaleButton.prototype.id = null;
	
	cls.ScaleButton.prototype.baseWidth = null;
	cls.ScaleButton.prototype.baseHeight = null;
	
	cls.ScaleButton.prototype.hoverScale = null;
	cls.ScaleButton.prototype.downScale = null;
	
	cls.ScaleButton.prototype.fTouchDown = false;
	cls.ScaleButton.prototype.fMouseOver = false;
	cls.ScaleButton.prototype.fMouseDown = false;
	
	cls.ScaleButton.prototype.onTouchStart = function(event) {
		this.fTouchDown = true;
		this.updateState();
	};
	
	cls.ScaleButton.prototype.onTouchEnd = function(event) {
		this.fTouchDown = false;
		this.updateState();
	};
	cls.ScaleButton.prototype.onTouchEndOutside = function(event) {
		this.fTouchDown = false;
		this.updateState();
	};
	
	cls.ScaleButton.prototype.onTouchTap = function(event) {
		if(this.cbcClick) {
			this.cbcClick.callback.call(this.cbcClick.context, event);
		}
	};
	
	cls.ScaleButton.prototype.onMouseOut = function(event) {
		this.fMouseOver = false;
		this.updateState();
	};
	
	cls.ScaleButton.prototype.onMouseOver = function(event) {
		this.fMouseOver = true;
		this.updateState();
	};
	
	cls.ScaleButton.prototype.onMouseDown = function(event) {
		this.fMouseDown = true;
		this.updateState();
	};
	
	cls.ScaleButton.prototype.onMouseUp = function(event) {
		this.fMouseDown = false;
		this.updateState();
	};
	
	cls.ScaleButton.prototype.onMouseUpOutside = function(event) {
		this.fMouseDown = false;
		this.updateState();
	};
	
	cls.ScaleButton.prototype.onMouseClick = function(event) {
		if(this.cbcClick) {
			this.cbcClick.callback.call(this.cbcClick.context, event);
		}
	};
	
	cls.ScaleButton.prototype.updateState = function() {
		var currentScale;
		if(this.fTouchDown || this.fMouseDown) {
			// is pressed down by touch or mouse;
			
			currentScale = this.downScale;
		}else if(this.fMouseOver) {
			// is mouse hover;
			
			currentScale = this.hoverScale;
		}else {
			// there is no interraction with the button;
			
			currentScale = 1;
		}
		this.width = this.baseWidth * currentScale;
		this.height = this.baseHeight * currentScale;
	};
	// END : ScaleButton;
	
	return cls;
});
