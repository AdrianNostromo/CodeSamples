/**
 * Created by Adrian on 11/21/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	"lib/pixijs/pixi",
	"app/util/tween/EaseFunctions",
	"app/main/view/util/ViewType",
	
	"app/util/VisualUtil",
	
	"app/main/view/ViewPreloader",
	"app/main/view/ViewMain",
	"app/main/view/ViewInGame",
	"app/main/view/ViewBackground",
	"app/main/view/ViewSideBushes",
	
	"app/main/view/ViewShop",
	"app/main/view/ViewLevelDefeat",
	"app/main/view/ViewLevelClear",
	"app/main/view/ViewInGamePaused",
	"app/main/view/ViewEdgePopup",
	
	"app/main/view/ViewDevConsole"
], function(declare, ModuleBase, PIXI, EaseFunctions, ViewType, 
		VisualUtil,
		ViewPreloader, ViewMain, ViewInGame, ViewBackground, ViewSideBushes, 
		ViewShop, ViewLevelDefeat, ViewLevelClear, ViewInGamePaused, ViewEdgePopup, 
		ViewDevConsole
	){
	"use strict";
	return declare("app.main.ViewController", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites:[
			ViewPreloader,
			ViewMain,
			ViewInGame,
			ViewBackground,
			ViewSideBushes,
			
			ViewShop,
			ViewLevelDefeat,
			ViewLevelClear,
			ViewInGamePaused,
			
			ViewEdgePopup,
			ViewDevConsole
		],
		subscribeListeners:[
		  {channel:"changeViewState", cbName:"onChangeViewState", isUnique:true},
		  {channel:"appLoopUserInterface", cbName:"onAppLoopUserInterface", isUnique:false},
		  {channel:"appSizeChanged",  cbName:"onAppSizeChanged",  isUnique:false}
		],
		publishMsg: {
			viewTransitionStep:{channel:"viewTransitionStep", listenerType:"list", mustExist:false},
			viewStateChange:{channel:"viewStateChange", listenerType:"list", mustExist:false},
			initializeView:{channel:"initializeView", listenerType:"list", mustExist:false},
			destroyView:{channel:"destroyView", listenerType:"list", mustExist:false},
			
			showViewMain:{channel:"showViewMain", listenerType:"any", mustExist:false},
			showViewPreloader:{channel:"showViewPreloader", listenerType:"any", mustExist:false},
			showViewInGame:{channel:"showViewInGame", listenerType:"any", mustExist:false},
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false},
			viewShowStateChanged:{channel:"viewShowStateChanged", listenerType:"any", mustExist:false},
			viewFrozenStateChanged:{channel:"viewFrozenStateChanged", listenerType:"any", mustExist:false}
		},
		
		//ASD_00x;// rename the "container" vElems node to "layerRoot";
		
		app: null,
		
		views: null,
		
		waitingCommands: null,
		
		dependantViewsShowState: null,
		
		cfg:{
			transitionSpeedS: 2
		},
		
		constructor: function() {
			this.views = {};
			this.waitingCommands = [];
			this.dependantViewsShowState = {};
		},
		
		onChangeViewState:function(viewID, stateEnd, doTransition, extraData, delayMS, cbcAnimationFinish, finishCommand) {
			if(!this.app) {
				this.app = this.publisher.publish(this.publishMsg.getApp);
			}
			
			if(delayMS > 0) {
				// Save as a command.
				
				this.waitingCommands.push({
					viewID: viewID,
					stateEnd: stateEnd,
					doTransition: doTransition,
					extraData: extraData,
					delayMS: delayMS,
					cbcAnimationFinish: cbcAnimationFinish,
					finishCommand: finishCommand
				});
				
				return ;
			}
			
			// Activate the view if not already exists.
			var viewCompound = this.views[viewID];
			
			var originalRawStateEnd = null;
			if(viewCompound) {
				originalRawStateEnd = viewCompound.state.stateEnd;
			}
			
			if(viewCompound && viewCompound.config.recreateOnShowRequest && stateEnd.indexOf("sIn") == 0) {
				this.publisher.publish(this.publishMsg.destroyView, viewID);
				
				viewCompound = null;
				this.views[viewID] = null;
			}
			if(!viewCompound) {
				var ret = {};
				
				this.publisher.publish(this.publishMsg.initializeView, viewID, ret);
				if(!ret.viewCompound) {
					this.postError("Error. ViewController.001.");
					
					return ;
				}
				
				viewCompound = ret.viewCompound;
				viewCompound.state = {stateStart: "sIn", stateEnd: "sOut", completionPercent: 1};
				viewCompound.isFrozen = true;
				
				this.views[viewID] = viewCompound;
			}
			
			var oldStateEnd = viewCompound.state.stateEnd;
			
			// Update the view direction.
			if(viewCompound.state.stateEnd != stateEnd) {
				viewCompound.state.stateStart = viewCompound.state.stateEnd;
				viewCompound.state.stateEnd = stateEnd;
				
				viewCompound.state.completionPercent = 1 - viewCompound.state.completionPercent;
				
				viewCompound.state.fDoUpdate = true;
			}
			
			viewCompound.finishCommand = finishCommand;
			viewCompound.cbcAnimationFinish = cbcAnimationFinish;
			if(viewCompound.state.stateEnd.indexOf("sIn") == 0) {
				if(extraData && extraData.cbcViewCloseAnimationFinish) {
					viewCompound.cbcViewCloseAnimationFinish = extraData.cbcViewCloseAnimationFinish;
				}else {
					viewCompound.cbcViewCloseAnimationFinish = null;
				}
			}
			
			// Set the initial state of the view.
			if(!doTransition) {
				viewCompound.state.completionPercent = 1;
				
				viewCompound.state.fDoUpdate = true;
			}
			
			if(viewCompound.state.stateEnd.indexOf("sIn") == 0 && viewCompound.cbcViewActivatedPre) {
				viewCompound.cbcViewActivatedPre.callback.call(viewCompound.cbcViewActivatedPre.context, extraData);
			}
			
			if((!originalRawStateEnd || originalRawStateEnd.indexOf("sOut") == 0) && viewCompound.state.stateEnd.indexOf("sIn") == 0) {
				this.publisher.publish(this.publishMsg.viewShowStateChanged, viewID, viewCompound.state.stateEnd);
			}else if(originalRawStateEnd.indexOf("sIn") == 0 && viewCompound.state.stateEnd.indexOf("sOut") == 0) {
				this.publisher.publish(this.publishMsg.viewShowStateChanged, viewID, viewCompound.state.stateEnd);
			}
			
			this.updateSingleView(viewID, viewCompound, true);
			
			if(viewCompound.state.stateEnd.indexOf("sIn") == 0 && viewCompound.cbcViewActivated) {
				viewCompound.cbcViewActivated.callback.call(viewCompound.cbcViewActivated.context, extraData);
			}
			
			if(viewCompound.state.completionPercent >= 1) {
				this.viewTransitionFinished(viewID, viewCompound);
			}
			
			if(viewCompound.state.stateEnd.indexOf("sOut") == 0 && viewCompound.state.completionPercent >= 1) {
				// View is deactivated.
				
				if(viewCompound.cbcViewDeactivated) {
					viewCompound.cbcViewDeactivated.callback.call(viewCompound.cbcViewDeactivated.context);
				}
			}
			
			if(viewCompound.dependantViews) {
				if((!originalRawStateEnd || originalRawStateEnd.indexOf("sOut") == 0) && viewCompound.state.stateEnd.indexOf("sIn") == 0) {
					// Step activate dependant views;
					
					this.changeDependantViewsShowIncrement(viewCompound.dependantViews, true, doTransition);
				}else if(originalRawStateEnd.indexOf("sIn") == 0 && viewCompound.state.stateEnd.indexOf("sOut") == 0) {
					// Step deactivate dependant views.
					
					this.changeDependantViewsShowIncrement(viewCompound.dependantViews, false, doTransition);
				}
			}
		},
		
		changeDependantViewsShowIncrement: function(dependantViews, showIncrement, doTransition) {
			if(!dependantViews) {
				return ;
			}
			
			for(var i=0;i<dependantViews.length;i++) {
				var viewID = dependantViews[i];
				if(viewID) {
					if(!(viewID in this.dependantViewsShowState)) {
						this.dependantViewsShowState[viewID] = 0;
					}
					
					if(showIncrement) {
						this.dependantViewsShowState[viewID]++;
					}else {
						this.dependantViewsShowState[viewID]--;
					}
					
					if(this.dependantViewsShowState[viewID] < 0) {
						this.postError("Error. ViewController.001.");
						
						return ;
					}
					
					if(this.dependantViewsShowState[viewID] == 1) {
						// Show view.
						
						this.publisher.publish(this.publishMsg.changeViewState, viewID, "sIn", doTransition, null, 0, null, null);
					}else if(this.dependantViewsShowState[viewID] == 0) {
						// Hide view.
						
						this.publisher.publish(this.publishMsg.changeViewState, viewID, "sOut", doTransition, null, 0, null, null);
					}
				}
			}
		},
		
		updateSingleView: function(viewID, viewCompound, updateStaticValues) {
			VisualUtil.updateLayout(viewCompound.vElems, viewCompound.layout, this.app.appSize, viewCompound.parentLayer, updateStaticValues, viewCompound.state, null);
			
			if(viewCompound.cbcViewLayoutUpdateFinished) {
				viewCompound.cbcViewLayoutUpdateFinished.callback.call(viewCompound.cbcViewLayoutUpdateFinished.context);
			}
			
			if(viewCompound.isFrozen && viewCompound.vElems.container.parent) {
				viewCompound.isFrozen = false;
				
				this.publisher.publish(this.publishMsg.viewFrozenStateChanged, viewID, viewCompound.isFrozen);
				
				if(viewCompound.cbcViewUnfrozen) {
					viewCompound.cbcViewUnfrozen.callback.call(viewCompound.cbcViewUnfrozen.context);
				}
			}else if(!viewCompound.isFrozen && !viewCompound.vElems.container.parent) {
				viewCompound.isFrozen = true;
				
				this.publisher.publish(this.publishMsg.viewFrozenStateChanged, viewID, viewCompound.isFrozen);
				
				if(viewCompound.cbcViewFrozen) {
					viewCompound.cbcViewFrozen.callback.call(viewCompound.cbcViewFrozen.context);
				}
			}
		},
		
		viewTransitionFinished: function(viewID, viewCompound) {
			if(viewCompound.cbcAnimationFinish) {
				viewCompound.cbcAnimationFinish.callback.call(viewCompound.cbcAnimationFinish.context);
				
				viewCompound.cbcAnimationFinish = null;
			}
			
			if(viewCompound.state.stateEnd.indexOf("sOut") == 0 && viewCompound.cbcViewCloseAnimationFinish) {
				viewCompound.cbcViewCloseAnimationFinish.callback.call(viewCompound.cbcViewCloseAnimationFinish.context);
				
				viewCompound.cbcViewCloseAnimationFinish = null;
			}
			
			if(viewCompound.finishCommand) {
				// Activate command;
				this.onChangeViewState(viewCompound.finishCommand.viewID, viewCompound.finishCommand.stateEnd, viewCompound.finishCommand.doTransition, viewCompound.finishCommand.extraData, viewCompound.finishCommand.delayMS, viewCompound.finishCommand.cbcAnimationFinish, viewCompound.finishCommand.finishCommand);
				
				viewCompound.finishCommand = null;
			}
			
			if(viewCompound.state.stateEnd.indexOf("sOut") == 0 && viewCompound.config.destroyOnOutAnimationFinish) {
				// Destroy the view.
				
				this.publisher.publish(this.publishMsg.destroyView, viewID);
				
				this.views[viewID] = null;
			}
		},
		
		onAppLoopUserInterface:function(appTime, deltaMS, deltaS) {
			// Animate each view that needs it.
			for (var viewID in this.views) {
				if (this.views.hasOwnProperty(viewID)) {
					var viewCompound = this.views[viewID];
					
					if(viewCompound && (viewCompound.state.completionPercent < 1 || viewCompound.state.fDoUpdate)) {
						viewCompound.state.fDoUpdate = false;
						
						viewCompound.state.completionPercent = Math.min(1, viewCompound.state.completionPercent + this.cfg.transitionSpeedS * deltaS);
						
						this.updateSingleView(viewID, viewCompound, false);
						
						if(viewCompound.state.completionPercent >= 1) {
							this.viewTransitionFinished(viewID, viewCompound);
							
							if(viewCompound.state.stateEnd.indexOf("sOut") == 0) {
								// View is deactivated.
								
								if(viewCompound.cbcViewDeactivated) {
									viewCompound.cbcViewDeactivated.callback.call(viewCompound.cbcViewDeactivated.context);
								}
							}
						}
					}
				}
			}
			
			for(var i=0;i<this.waitingCommands.length;i++) {
				var finishCommand = this.waitingCommands[i];
				finishCommand.delayMS = Math.max(0, finishCommand.delayMS - deltaMS);
				if(finishCommand.delayMS <= 0) {
					// Activate the command.
					this.onChangeViewState(finishCommand.viewID, finishCommand.stateEnd, finishCommand.doTransition, finishCommand.extraData, finishCommand.delayMS, finishCommand.cbcAnimationFinish, finishCommand.finishCommand);
					
					// Remove the command from the list;
					if(this.waitingCommands.length > 1 && i < this.waitingCommands.length - 1) {
						this.waitingCommands[i] = this.waitingCommands[this.waitingCommands.length - 1];
					}
					this.waitingCommands.pop();
				}
			}
		},
		
		onAppSizeChanged:function() {
			for (var viewID in this.views) {
				if (this.views.hasOwnProperty(viewID)) {
					var viewCompound = this.views[viewID];
					
					if(viewCompound && (viewCompound.state.stateEnd.indexOf("sIn") == 0 || viewCompound.state.completionPercent < 1)) {
						// View needs update.
						
						this.updateSingleView(viewID, viewCompound, true);
					}
				}
			}
		},
		
		destroy: function() {
			//void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});
