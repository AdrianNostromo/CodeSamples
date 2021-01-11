/**
 * Created by Adrian on 11/21/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	var cls = declare("app.util.tween.EaseFunctions", null, {});
	
	// t: current time, b: begInnIng value, c: change In value, d: duration
	cls.easeLinear = function (t, b, c, d) {
		return c*(t/d) + b;
	};
	cls.easeInQuad = function (t, b, c, d) {
		return c*(t/=d)*t + b;
	};
	cls.easeOutQuad = function (t, b, c, d) {
		return -c *(t/=d)*(t-2) + b;
	};
	cls.easeInOutQuad = function (t, b, c, d) {
		if ((t/=d/2) < 1) return c/2*t*t + b;
		return -c/2 * ((--t)*(t-2) - 1) + b;
	};
	cls.easeInCubic = function (t, b, c, d) {
		return c*(t/=d)*t*t + b;
	};
	cls.easeOutCubic = function (t, b, c, d) {
		return c*((t=t/d-1)*t*t + 1) + b;
	};
	cls.easeInOutCubic = function (t, b, c, d) {
		if ((t/=d/2) < 1) return c/2*t*t*t + b;
		return c/2*((t-=2)*t*t + 2) + b;
	};
	cls.easeInQuart = function (t, b, c, d) {
		return c*(t/=d)*t*t*t + b;
	};
	cls.easeOutQuart = function (t, b, c, d) {
		return -c * ((t=t/d-1)*t*t*t - 1) + b;
	};
	cls.easeInOutQuart = function (t, b, c, d) {
		if ((t/=d/2) < 1) return c/2*t*t*t*t + b;
		return -c/2 * ((t-=2)*t*t*t - 2) + b;
	};
	cls.easeInQuint = function (t, b, c, d) {
		return c*(t/=d)*t*t*t*t + b;
	};
	cls.easeOutQuint = function (t, b, c, d) {
		return c*((t=t/d-1)*t*t*t*t + 1) + b;
	};
	cls.easeInOutQuint = function (t, b, c, d) {
		if ((t/=d/2) < 1) return c/2*t*t*t*t*t + b;
		return c/2*((t-=2)*t*t*t*t + 2) + b;
	};
	cls.easeInSine = function (t, b, c, d) {
		return -c * Math.cos(t/d * (Math.PI/2)) + c + b;
	};
	cls.easeOutSine = function (t, b, c, d) {
		return c * Math.sin(t/d * (Math.PI/2)) + b;
	};
	cls.easeInOutSine = function (t, b, c, d) {
		return -c/2 * (Math.cos(Math.PI*t/d) - 1) + b;
	};
	cls.easeInExpo = function (t, b, c, d) {
		return (t==0) ? b : c * Math.pow(2, 10 * (t/d - 1)) + b;
	};
	cls.easeOutExpo = function (t, b, c, d) {
		return (t==d) ? b+c : c * (-Math.pow(2, -10 * t/d) + 1) + b;
	};
	cls.easeInOutExpo = function (t, b, c, d) {
		if (t==0) return b;
		if (t==d) return b+c;
		if ((t/=d/2) < 1) return c/2 * Math.pow(2, 10 * (t - 1)) + b;
		return c/2 * (-Math.pow(2, -10 * --t) + 2) + b;
	};
	cls.easeInCirc = function (t, b, c, d) {
		return -c * (Math.sqrt(1 - (t/=d)*t) - 1) + b;
	};
	cls.easeOutCirc = function (t, b, c, d) {
		return c * Math.sqrt(1 - (t=t/d-1)*t) + b;
	};
	cls.easeInOutCirc = function (t, b, c, d) {
		if ((t/=d/2) < 1) return -c/2 * (Math.sqrt(1 - t*t) - 1) + b;
		return c/2 * (Math.sqrt(1 - (t-=2)*t) + 1) + b;
	};
	cls.easeInElastic = function (t, b, c, d) {
		var s=1.70158;var p=0;var a=c;
		if (t==0) return b;  if ((t/=d)==1) return b+c;  if (!p) p=d*.3;
		if (a < Math.abs(c)) { a=c; var s=p/4; }
		else var s = p/(2*Math.PI) * Math.asin (c/a);
		return -(a*Math.pow(2,10*(t-=1)) * Math.sin( (t*d-s)*(2*Math.PI)/p )) + b;
	};
	cls.easeOutElastic = function (t, b, c, d) {
		var s=1.70158;var p=0;var a=c;
		if (t==0) return b;  if ((t/=d)==1) return b+c;  if (!p) p=d*.3;
		if (a < Math.abs(c)) { a=c; var s=p/4; }
		else var s = p/(2*Math.PI) * Math.asin (c/a);
		return a*Math.pow(2,-10*t) * Math.sin( (t*d-s)*(2*Math.PI)/p ) + c + b;
	};
	cls.easeInOutElastic = function (t, b, c, d) {
		var s=1.70158;var p=0;var a=c;
		if (t==0) return b;  if ((t/=d/2)==2) return b+c;  if (!p) p=d*(.3*1.5);
		if (a < Math.abs(c)) { a=c; var s=p/4; }
		else var s = p/(2*Math.PI) * Math.asin (c/a);
		if (t < 1) return -.5*(a*Math.pow(2,10*(t-=1)) * Math.sin( (t*d-s)*(2*Math.PI)/p )) + b;
		return a*Math.pow(2,-10*(t-=1)) * Math.sin( (t*d-s)*(2*Math.PI)/p )*.5 + c + b;
	};
	cls.easeInBack = function (t, b, c, d, s) {
		if (s == undefined) s = 1.70158;
		return c*(t/=d)*t*((s+1)*t - s) + b;
	};
	cls.easeOutBack = function (t, b, c, d, s) {
		if (s == undefined) s = 1.70158;
		return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b;
	};
	cls.easeInOutBack = function (t, b, c, d, s) {
		if (s == undefined) s = 1.70158; 
		if ((t/=d/2) < 1) return c/2*(t*t*(((s*=(1.525))+1)*t - s)) + b;
		return c/2*((t-=2)*t*(((s*=(1.525))+1)*t + s) + 2) + b;
	};
	cls.easeInBounce = function (t, b, c, d) {
		return c - jQuery.easing.easeOutBounce (d-t, 0, c, d) + b;
	};
	cls.easeOutBounce = function (t, b, c, d) {
		if ((t/=d) < (1/2.75)) {
			return c*(7.5625*t*t) + b;
		} else if (t < (2/2.75)) {
			return c*(7.5625*(t-=(1.5/2.75))*t + .75) + b;
		} else if (t < (2.5/2.75)) {
			return c*(7.5625*(t-=(2.25/2.75))*t + .9375) + b;
		} else {
			return c*(7.5625*(t-=(2.625/2.75))*t + .984375) + b;
		}
	};
	cls.easeInOutBounce = function (t, b, c, d) {
		if (t < d/2) return jQuery.easing.easeInBounce (t*2, 0, c, d) * .5 + b;
		return jQuery.easing.easeOutBounce (t*2-d, 0, c, d) * .5 + c*.5 + b;
	}
	
	return cls;
});
