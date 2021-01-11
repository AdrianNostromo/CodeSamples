/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	
	"app/util/Point"
], function(declare, Point){
	"use strict";
	var cls = declare("app.util.TimeUltil", null, {});
	
	cls.secondsToComposedDayHourMinuteSecond = function(tmS) {
		var totalDays = Math.floor(tmS / (60 * 60 * 24));
		var totalHours = Math.floor(tmS / (60 * 60));
		var totamMinutes = Math.floor(tmS / 60);
		var totalSeconds = tmS;
		
		var ret = {
			days:totalDays,
			hours:totalHours - totalDays * 24,
			minutes:totamMinutes - totalHours * 60,
			seconds:totalSeconds - totamMinutes * 60
		};
		
		return ret;
	};
	
	cls.secondsToComposedMinuteSecond = function(tmS) {
		var totamMinutes = Math.floor(tmS / 60);
		var totalSeconds = tmS;
		
		var ret = {
			minutes:totamMinutes,
			seconds:totalSeconds - totamMinutes * 60
		};
		
		return ret;
	};
	
	return cls;
});
