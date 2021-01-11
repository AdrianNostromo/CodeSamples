/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	return declare("app.util.EventsDispatcher", null, {
		channels: null,
		
		constructor: function () {
			this.channels = {};
		},

		/**
		 * 
		 * @param eventName
		 * @param callbackObject:{callback, context}
		 */
		on: function (channel, fn, context, isUnique) {
			if (isUnique === undefined) {
				this.postError("err 9");
				

				return false;
			}

			if (isUnique == true) {
				if (this.channels[channel]) {
					this.postError("err 6");
					

					return false;
				}

				this.channels[channel] = {callback: fn, context: context};
			} else {
				if (!this.channels[channel]) {
					this.channels[channel] = [];
				} else if (!Array.isArray(this.channels[channel])) {
					// there is a unique listener on that channel, it must be a array at this location;

					this.postError("err 7");
					

					return false;
				}

				this.channels[channel].push({callback: fn, context: context});
			}

			return this;
		},
		
		removeListener: function (channel, fn, context) {
			if (!channel) {
				this.postError("err 13");
				//err_crit;

				return false;
			}

			if (!this.channels[channel]) {
				this.postError("warning 14");
				//warning, listener group not found;

				return false;
			}

			var targetChannel = this.channels[channel];

			if (!Array.isArray(targetChannel)) {
				// a unique listener found;

				if(targetChannel.context == context && targetChannel.callback == fn) {
					this.channels[channel] = undefined;
				}else {
					// not found;

					this.postError("warning 15");
					//warning, listener not found;

					return false;
				}
			}else {
				// a array of listeners found;

				for (var i = 0, l = targetChannel.length; i < l; i++) {
					var subscription = targetChannel[i];
					if(subscription.context == context && subscription.callback == fn) {
						targetChannel[i] = undefined;

						return ;
					}
				}

				// not found;

				this.postError("warning 16");
				//warning, listener not found;

				return false;
			}
		},
		
		register: function (channel, fn, context, isUnique) {
			return this.on(channel, fn, context, isUnique);
		},
		
		unregister: function (channel, fn, context) {
			return this.removeListener(channel, fn, context);
		},
		
		/**
		 * 
		 * @param eventName
		 * @param auto variable arguments slicing used;
		 */
		dispatch: function (channel) {
			if (!channel) {
				this.postError("err 13");
				//err_crit;

				return false;
			}

			if (!this.channels[channel]) {
				// all publishers may be optional, error handling is not done here;

				return false;
			}

			var args = Array.prototype.slice.call(arguments, 1);

			var targetChannel = this.channels[channel];

			if (!Array.isArray(targetChannel)) {
				return targetChannel.callback.apply(targetChannel.context, args);
			}

			var ct = 0;
			for (var i = 0, l = targetChannel.length; i < l; i++) {
				var subscription = targetChannel[i];
				subscription.callback.apply(subscription.context, args);
				ct++;
			}
			if (ct <= 0) {
				this.postError("err 1");
				//err_crit;

				return false;
			}

			return this;
		},

		destroy: function () {
			// void
		}

	});
});
