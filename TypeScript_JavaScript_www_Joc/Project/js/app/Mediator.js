define([
	"dojo/_base/declare"
], function(declare
	){
	"use strict";
	return declare("app.main.Mediator", null, {
		subscribeListeners:[
			{channel:"mCreateModule", cbName:"onMCreateModule", isUnique:true},
			{channel:"mCreateModulesList", cbName:"onMCreateModulesList", isUnique:true},
			{channel:"mDisconnectModule", cbName:"onMDisconnectModule", isUnique:true},
			{channel:"mCheckEmptySubscribers", cbName:"onMCheckEmptySubscribers", isUnique:true}
		],
		publishMsg: {
			mCreateModule:{channel:"mCreateModule", listenerType:"unique", mustExist:true},
			mCreateModulesList:{channel:"mCreateModulesList", listenerType:"unique", mustExist:true},
			mDisconnectModule:{channel:"mDisconnectModule", listenerType:"unique", mustExist:true},
			
			postLog:{channel:"postLog", listenerType:"unique", mustExist:true, noErrorHandling:true},
			postWarning:{channel:"postWarning", listenerType:"unique", mustExist:true, noErrorHandling:true},
			postError:{channel:"postError", listenerType:"unique", mustExist:true, noErrorHandling:true},
			
			mListModules:{channel:"mListModules", listenerType:"list", mustExist:false}
		},
		activeSubscriptionHooks:null,
		
		channels: null,
		
		isDebug_ignoreMissingMandatoryListeners:false,
		
		constructor: function() {
			this.channels = {};
			
			this.subscribeObjectListenersList(this, this.subscribeListeners);
			
			if(typeof window.App === 'undefined') {
				window.App = {};
			}
			if(!window.App.Global) {
				window.App.Global = {};
			}
			window.App.Global.Mediator = this;
		},
		
		postLog:function(msg) {
			console.log("Log: " + msg);
			this.publish(this.publishMsg.postLog, msg);
		},
		
		postWarning:function(msg) {
			console.log("Warning: " + msg);
			this.publish(this.publishMsg.postWarning, msg);
		},
		
		postError:function(msg) {
			console.log("Error: " + msg);
			this.publish(this.publishMsg.postError, msg);
		},
		
		installTo: function (obj) {
			obj.subscribe = this.subscribe;
			obj.unsubscribe = this.unsubscribe;
			obj.publisher = this;
			
			if(obj.subscribeListenersB) {
				this.subscribeObjectListenersList(obj, obj.subscribeListenersB);
			}
			if(obj.subscribeListenersC) {
				this.subscribeObjectListenersList(obj, obj.subscribeListenersC);
			}
			if(obj.subscribeListeners) {
				this.subscribeObjectListenersList(obj, obj.subscribeListeners);
			}
			
			if(obj.moduleConnectOccured) {
				obj.moduleConnectOccured();
			}
		},
		
		onMDisconnectModule: function(targetModule) {
			var channelItem;
			
			if(targetModule.subscribeListenersB) {
				this.unsubscribeAllListeners(targetModule);
			}
			
			targetModule.subscribe = undefined;
			targetModule.unsubscribe = undefined;
			targetModule.publisher = undefined;
		},
		
		subscribeObjectListenersList:function(obj, subscribeListeners) {
			if(!obj.activeSubscriptionHooks) {
				obj.activeSubscriptionHooks = {};
			}
			
			for(var i=0;i<subscribeListeners.length;i++) {
				var listenerConfig = subscribeListeners[i];
				
				this.subscribe(obj, listenerConfig);
			}
		},
		
		subscribe: function (obj, listenerConfig) {
			if(!listenerConfig || typeof listenerConfig !== 'object') {
				this.postError("err 6");
				
				return false;
			}
			
			if(!("channel" in listenerConfig) || !("cbName" in listenerConfig) || !("isUnique" in listenerConfig)) {
				this.postError("err 6");
				
				return false;
			}

			var channel = listenerConfig.channel,
				cbName = listenerConfig.cbName,
				isUnique = listenerConfig.isUnique;
			
			if(obj.activeSubscriptionHooks[channel]) {
				this.postError("err 6");
				
				return false;
			}
			
			if(!obj[cbName]) {
				this.postError("err 6");
				
				return false;
			}
			
			if (isUnique) {
				if (this.channels[channel]) {
					this.postError("err 6");
					
					return false;
				}
				
				var subscription = {context: obj, callback: obj[cbName]};
				this.channels[channel] = subscription;
				
				obj.activeSubscriptionHooks[channel] = subscription;
			} else {
				if (!this.channels[channel]) {
					this.channels[channel] = [];
				} else if (!(this.channels[channel] instanceof Array)) {
					// there is a unique listener on that channel, it must be a array at this location;
					
					this.postError("err 7");
					
					return false;
				}
				
				var subscription = {context: obj, callback: obj[cbName]};
				this.channels[channel].push(subscription);
				
				obj.activeSubscriptionHooks[channel] = subscription;
			}
			
			return this;
		},
		
		unsubscribeAllListeners:function(obj) {
			if(!obj.activeSubscriptionHooks) {
				this.postError("err 6");
				
				return false;
			}
			
			for (var channel in obj.activeSubscriptionHooks) {
				if (obj.activeSubscriptionHooks.hasOwnProperty(channel)) {
					//var listenerConfig = obj.activeSubscriptionHooks[channel];
					
					this.unsubscribe(obj, channel);
				}
			}
			
			obj.activeSubscriptionHooks = null;
		},
		
		unsubscribe: function (obj, channel) {
			if(!obj || !channel) {
				this.postError("err 6");
				
				return false;
			}
			
			var subscriptionHook = obj.activeSubscriptionHooks[channel];
			if(!subscriptionHook) {
				// listener is not hooked;
				
				this.postError("err 6");
				
				return false;
			}
			
			var targetChannel = this.channels[channel];
			if(!targetChannel) {
				this.postError("err 6");
				
				return false;
			}
			
			if((targetChannel instanceof Array)) {
				// is a list;
				
				var i = targetChannel.indexOf(subscriptionHook);
				if(i < 0) {
					this.postError("err 6");
					
					return false;
				}
				
				if(targetChannel.length > 1 && i < targetChannel.length - 1) {
					targetChannel[i] = targetChannel[targetChannel.length - 1];
				}
				targetChannel.pop();
			}else {
				// is a unique listener;
				
				this.channels[channel] = null;
			}
			
			obj.activeSubscriptionHooks[channel] = null;
			
			return true;
		},
		
		publish: function (channelCompound) {
			if (!channelCompound || typeof channelCompound !== 'object') {
				this.postError("err 13");
				//err_crit;

				return null;
			}
			if (!("channel" in channelCompound) || !("listenerType" in channelCompound) || !("mustExist" in channelCompound)) {
				this.postError("err 13");
				//err_crit;
				
				return null;
			}
			
			var channel = channelCompound.channel, 
				listenerType = channelCompound.listenerType, 
				mustExist = channelCompound.mustExist;
			
			var targetChannel = this.channels[channel];
			if(!targetChannel) {
				if(mustExist && !this.isDebug_ignoreMissingMandatoryListeners) {
					if(channelCompound.noErrorHandling) {
						console.log("Mediator args error.");
					}else {
						this.postError("Mediator args error.");
					}
					
					return null;
				}
				
				return false;
			}
			
			var args = Array.prototype.slice.call(arguments, 1);
			
			if (targetChannel instanceof Array) {
				if(listenerType !== "list" && listenerType !== "any") {
					if(channelCompound.noErrorHandling) {
						console.log("A unique listener is required.");
					}else {
						this.postError("A unique listener is required.");
					}
					
					return false;
				}
				
				var ct = 0;
				
				for(var i=targetChannel.length - 1;i>=0;i--) {
					var subscription = targetChannel[i];
					
					if(subscription.callback.length != args.length) {
						if(channelCompound.noErrorHandling) {
							console.log("The number of parameter on the listener does not equal the number of parameters suplied.");
						}else {
							this.postError("The number of parameter on the listener does not equal the number of parameters suplied.");
						}
						
						return null;
					}
					
					subscription.callback.apply(subscription.context, args);
					
					ct++;
				}
				
				if (ct <= 0 && mustExist && !this.isDebug_ignoreMissingMandatoryListeners) {
					if(channelCompound.noErrorHandling) {
						console.log("A listener must exist for this callback.");
					}else {
						this.postError("A listener must exist for this callback.");
					}
					
					return null;
				}
				
				return true;
			}
			
			// is not a list;
			if(listenerType !== "unique" && listenerType !== "any") {
				if(channelCompound.noErrorHandling) {
					console.log("A list of listeners is required.");
				}else {
					this.postError("A list of listeners is required.");
				}
				
				return false;
			}
			
			if(targetChannel.callback.length != args.length) {
				if(channelCompound.noErrorHandling) {
					console.log("The number of parameter on the listener does not equal the number of parameters suplied.");
				}else {
					this.postError("The number of parameter on the listener does not equal the number of parameters suplied.");
				}
				
				return null;
			}
			return targetChannel.callback.apply(targetChannel.context, args);
		},

		onMCreateModule: function (moduleClass) {
			if(!moduleClass || typeof moduleClass !== "function") {
				this.postError("err 16");
				
				return false;
			}
			var newModule = new moduleClass();

			if (newModule.modulePrerequisites) {
				this.onMCreateModulesList(newModule.modulePrerequisites);
			}

			this.installTo(newModule);
		},

		onMCreateModulesList: function (moduleClassesList) {
			for (var i = 0; i < moduleClassesList.length; i++) {
				this.onMCreateModule(moduleClassesList[i]);
			}
		},
		
		onMCheckEmptySubscribers:function() {
			var result = "";
			
			var modulesList = [];
			
			this.publish(this.publishMsg.mListModules, modulesList);
			
			var currentModuleUniqueSubscribers = [],
				currentModuleUniquePublishers = [];
				
			// []:String
			var uniqueSubscribersList = [];
			this.processEntriesContainer(uniqueSubscribersList, currentModuleUniqueSubscribers, this.subscribeListeners);
			
			var uniquePublishMessages = [];
			this.processEntriesContainer(uniquePublishMessages, currentModuleUniquePublishers, this.publishMsg);
			
			var duplicateEntries = this.checkIfDuplicatesSubscribePublishEntries(currentModuleUniqueSubscribers, currentModuleUniquePublishers);
			if(duplicateEntries) {
				result = result + "\nSTART - Module:(" + this.__proto__.declaredClass + ") has subscription for his own publis messages:\n" + duplicateEntries + "\n";
			}
			
			for(var i=0;i<modulesList.length;i++) {
				var entry = modulesList[i];
				if(entry) {
					currentModuleUniqueSubscribers = [];
					currentModuleUniquePublishers = [];
					
					if(entry.subscribeListenersB) {
						this.processEntriesContainer(uniqueSubscribersList, currentModuleUniqueSubscribers, entry.subscribeListenersB);
					}
					if(entry.subscribeListenersC) {
						this.processEntriesContainer(uniqueSubscribersList, currentModuleUniqueSubscribers, entry.subscribeListenersC);
					}
					if(entry.subscribeListeners) {
						this.processEntriesContainer(uniqueSubscribersList, currentModuleUniqueSubscribers, entry.subscribeListeners);
					}
					
					this.processEntriesContainer(uniquePublishMessages, currentModuleUniquePublishers, entry.publishMsgBase);
					this.processEntriesContainer(uniquePublishMessages, currentModuleUniquePublishers, entry.publishMsg);
					
					duplicateEntries = this.checkIfDuplicatesSubscribePublishEntries(currentModuleUniqueSubscribers, currentModuleUniquePublishers);
					if(duplicateEntries) {
						result = result + "\nSTART - Module:(" + entry.__proto__.declaredClass + ") has subscription for his own publis messages:\n" + duplicateEntries + "\n";
					}
				}
			}
			
			duplicateEntries = this.checkForUnusedEntriesAInB(uniqueSubscribersList, uniquePublishMessages);
			if(duplicateEntries) {
				result = result + "\nSTART - Subscribers that are never called:\n" + duplicateEntries + "\n";
			}
			
			duplicateEntries = this.checkForUnusedEntriesAInB(uniquePublishMessages, uniqueSubscribersList);
			if(duplicateEntries) {
				result = result + "\nSTART - Publishers that are not listened for:\n" + duplicateEntries + "\n";
			}
			
			result = result + "\nSTART - All subscribers:\n" + uniqueSubscribersList + "\n";
			result = result + "\nSTART - All publishers:\n" + uniquePublishMessages + "\n";
			
			return result;
		},
		
		checkIfDuplicatesSubscribePublishEntries:function(stringArrayA, stringArrayB) {
			var ret = null;
			for(var i=0;i<stringArrayA.length;i++) {
				var entry = stringArrayA[i];
				if(entry) {
					var j = stringArrayB.indexOf(entry);
					if(j >= 0) {
						if(!ret) {
							ret = [];
						}
						ret.push(entry);
					}
				}
			}
			
			return ret;
		},
		
		checkForUnusedEntriesAInB:function(stringArrayA, stringArrayB) {
			var ret = null;
			
			for(var i=0;i<stringArrayA.length;i++) {
				var entry = stringArrayA[i];
				if(entry) {
					var j = stringArrayB.indexOf(entry);
					if(j < 0) {
						if(!ret) {
							ret = [];
						}
						ret.push(entry);
					}
				}
			}
			
			return ret;
		},
		
		processEntriesContainer:function(targetArray, targetArrayB, sourceContainer) {
			if(sourceContainer) {
				for (var key in sourceContainer) {
					if (sourceContainer.hasOwnProperty(key)) {
						var value = sourceContainer[key];
						
						if(value && targetArray.indexOf(key) < 0) {
							targetArray.push(key);
						}
						
						if(value && targetArrayB.indexOf(key) < 0) {
							targetArrayB.push(key);
						}
					}
				}
			}
		}
		
	});
});
