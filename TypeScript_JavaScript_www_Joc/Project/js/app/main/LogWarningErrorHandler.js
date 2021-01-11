/**
 * Created by Adrian on 11/21/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase"
], function(declare, ModuleBase){
	"use strict";
	return declare("app.main.LogWarningErrorHandler", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"postLog", cbName:"onPostLog", isUnique:true},
			{channel:"postWarning", cbName:"onPostWarning", isUnique:true},
			{channel:"postError", cbName:"onPostError", isUnique:true},
			
			{channel:"initializeLogHandler", cbName:"onInitializeLogHandler", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},
		
		app:null,
		
		constructor: function() {
			//void
		},
		
		onPostLog:function(msg) {
			console.log("Log: " + msg);
		},
		
		onPostWarning:function(msg) {
			console.log("Warning: " + msg);
		},
		
		onPostError:function(msg) {
			console.log("Error: " + msg);
		},
		
		criticalError:function(msg) {
			// ASD_002; close app;
			console.log("Critical error: " + msg);
		},
		
		onInitializeLogHandler:function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.app.log = {};
			this.app.log.postLog = this.postLog.bind(this);
			this.app.log.postWarning = this.postWarning.bind(this);
			this.app.log.postError = this.postError.bind(this);
			this.app.log.criticalError = this.criticalError.bind(this);
		},
		
		destroy: function() {
			//void
			
			this.__proto__.__proto__.destroy.call(this);
		}

	});
});
