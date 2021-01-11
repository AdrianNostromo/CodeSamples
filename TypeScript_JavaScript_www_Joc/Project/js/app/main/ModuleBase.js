define([
	"dojo/_base/declare"
], function(declare){
	"use strict";
	return declare("app.main.ModuleBase", null, {
		subscribeListenersB:[
			{channel:"mListModules", cbName:"onMListModules", isUnique:false},
			{channel:"destroyModulesBranch", cbName:"onDestroyModulesBranch", isUnique:false}
		],
		publishMsgBase: {
			mDisconnectModule:{channel:"mDisconnectModule", listenerType:"unique", mustExist:true},
			postLog:{channel:"postLog", listenerType:"any", mustExist:false, noErrorHandling:true},
			postWarning:{channel:"postWarning", listenerType:"any", mustExist:false, noErrorHandling:true},
			postError:{channel:"postError", listenerType:"any", mustExist:false, noErrorHandling:true},
			postDevLog:{channel:"postDevLog", listenerType:"any", mustExist:false, noErrorHandling:true}
		},
		activeSubscriptionHooks:null,
		
		subscribe: undefined,
		unsubscribe: undefined,
		publisher: undefined,
		
		constructor: function() {
			//void
		},
		
		onMListModules:function(modulesList) {
			modulesList.push(this);
		},
		
		onDestroyModulesBranch:function(branchName) {
			if(this.moduleBranch === branchName) {
				this.destroy();
			}
		},
		
		postLog:function(msg) {
			console.log("Log: " + msg);
			this.publisher.publish(this.publishMsgBase.postLog, msg);
		},
		
		postWarning:function(msg) {
			console.log("Warning: " + msg);
			this.publisher.publish(this.publishMsgBase.postWarning, msg);
		},
		
		postError:function(msg) {
			console.log("Error: " + msg);
			this.publisher.publish(this.publishMsgBase.postError, msg);
		},
		
		postDevLog:function(msg) {
			this.publisher.publish(this.publishMsgBase.postDevLog, msg);
		},
		
		destroy: function() {
			this.publisher.publish(this.publishMsgBase.mDisconnectModule, this);
		}
		
	});
});
