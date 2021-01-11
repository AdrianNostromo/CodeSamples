/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"app/main/view/util/ViewType"
], function(declare, ModuleBase, 
		ViewType
	){
	"use strict";
	return declare("app.main.view.ViewVictoryBonus", ModuleBase, {
		moduleBranch:"global",
		modulePrerequisites:null,
		subscribeListeners:[
			{channel:"initializeView", cbName:"onInitializeView", isUnique:false}
		],
		publishMsg: {
			getApp:{channel:"getApp",  listenerType:"unique", mustExist:true},
			changeViewState:{channel:"changeViewState", listenerType:"any", mustExist:false},
			activateInGame:{channel:"activateInGame", listenerType:"list", mustExist:false}
		},
		
		app: null,
		
		viewID: ViewType.VICTORY_BONUS,
		viewCompound: null,
		
		vElems: null,
		
		cfg: {
		},
		
		viewConfig: {
		},
		
		viewLayout: [
			{
				id:"container",
				params:[
					{prop:"y", val:{hasStates: true,
						sIn:{hPercent:0 / 960, targetID:"_app"},
						sInEaseName: "easeOutSine",
						sOut:{hPercent:960 / 960, targetID:"_app"},
						sOutEaseName: "easeOutSine"
					}}
				]
			},
			{
				id:"titlePanel",
				list:[
					{
						id:"container",
						params:[
							{prop:"y", val:{hPercent:57 / 960, targetID:"_app"}}
						]
					},
					{
						id:"bg",
						params:[
							{prop:"width", val:{wPercent:640 / 640, targetID:"_app"}},
							{prop:"height", val:{hPercent:212 / 960, targetID:"_app"}}
						]
					},
					{
						id:"subtitlePanel",
						list:[
							{
								id:"container",
								params:[
									{prop:"rotation", val:-6.5 * (Math.PI/180)},
									{prop:"x", val:{wPercent:320 / 640, targetID:"_app"}},
									{prop:"y", val:{hPercent:161 / 960, targetID:"_app"}}
								]
							},
							{
								id:"bg",
								params:[
									{prop:"width", val:{wPercent:504 / 640, targetID:"_app"}},
									{prop:"height", val:{hPercent:138 / 960, targetID:"_app"}}
								]
							},
							{
								id:"iconStars",
								params:[
									{prop:"_widthHeight", val:{wPercent:80 / 640, targetID:"_app"}},
									{prop:"x", val:{wPercent:-103 / 640, targetID:"_app"}},
									{prop:"y", val:{hPercent:2 / 960, targetID:"_app"}}
								]
							},
							{
								id:"lblStars",
								_refreshTextStyle: true,
								params:[
									{prop:["style", "font"], val:{add: [{value:"800 "}, {wPercent:77 / 640, roundType: "ceil", targetID:"_app"}, {value:"px 'Dosis-ExtraBold'"}]}},
									{prop:"x", val:{wPercent:46 / 640, targetID:"_app"}},
									{prop:"y", val:{hPercent:0 / 960, targetID:"_app"}}
								]
							}
						]
					},
					{
						id:"imgTitle",
						params:[
							{prop:"width", val:{wPercent:290 / 640, targetID:"_app"}},
							{prop:"height", val:{hPercent:113 / 960, targetID:"_app"}},
							{prop:"x", val:{wPercent:320 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:66 / 960, targetID:"_app"}}
						]
					},
					{
						id:"btnSlotsPayout",
						params:[
							{prop:"_widthHeight", val:{wPercent:77 / 640, targetID:"_app"}},
							{prop:"x", val:{wPercent:534 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:-12 / 960, targetID:"_app"}}
						]
					}
				]
			},
			{
				id:"slotsPanel",
				list:[
					{
						id:"container",
						params:[
							{prop:"y", val:{hPercent:315 / 960, targetID:"_app"}}
						]
					},
					{
						id:"bg",
						params:[
							{prop:"width", val:{wPercent:640 / 640, targetID:"_app"}},
							{prop:"height", val:{hPercent:610 / 960, targetID:"_app"}}
						]
					},
					{
						id:"slotsBg",
						params:[
							{prop:"width", val:{wPercent:546 / 640, targetID:"_app"}},
							{prop:"height", val:{hPercent:337 / 960, targetID:"_app"}},
							{prop:"x", val:{wPercent:47 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:24 / 960, targetID:"_app"}}
						]
					},
					{
						id:"slotsFront",
						params:[
							{prop:"width", val:{wPercent:556 / 640, targetID:"_app"}},
							{prop:"height", val:{hPercent:347 / 960, targetID:"_app"}},
							{prop:"x", val:{wPercent:42 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:19 / 960, targetID:"_app"}}
						]
					},
					{
						id:"reelMissedB",
						params:[
							{prop:"width", val:{wPercent:153 / 640, targetID:"_app"}},
							{prop:"height", val:{hPercent:294 / 960, targetID:"_app"}},
							{prop:"x", val:{wPercent:320 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:193 / 960, targetID:"_app"}}
						]
					},
					{
						id:"reelMissedC",
						params:[
							{prop:"width", val:{wPercent:153 / 640, targetID:"_app"}},
							{prop:"height", val:{hPercent:294 / 960, targetID:"_app"}},
							{prop:"x", val:{wPercent:500 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:193 / 960, targetID:"_app"}}
						]
					},
					{
						id:"currentBonusBg",
						params:[
							{prop:"_widthHeight", val:{wPercent:156 / 640, targetID:"_app"}},
							{prop:"x", val:{wPercent:142 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:415 / 960, targetID:"_app"}}
						]
					},
					{
						id:"lblCurrentBonus",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {wPercent:40 / 640, roundType: "ceil", targetID:"_app"}, {value:"px 'Arial'"}]}},
							{prop:"x", val:{wPercent:142 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:415 / 960, targetID:"_app"}}
						]
					},
					{
						id:"nextBonusBg",
						params:[
							{prop:"_widthHeight", val:{wPercent:156 / 640, targetID:"_app"}},
							{prop:"x", val:{wPercent:498 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:415 / 960, targetID:"_app"}}
						]
					},
					{
						id:"lblNextBonus",
						_refreshTextStyle: true,
						params:[
							{prop:["style", "font"], val:{add: [{value:"800 "}, {wPercent:40 / 640, roundType: "ceil", targetID:"_app"}, {value:"px 'Arial'"}]}},
							{prop:"x", val:{wPercent:498 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:415 / 960, targetID:"_app"}}
						]
					},
					{
						id:"iconBonusArrow",
						params:[
							{prop:"_widthHeight", val:{wPercent:140 / 640, targetID:"_app"}},
							{prop:"x", val:{wPercent:320 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:415 / 960, targetID:"_app"}}
						]
					},
					{
						id:"btnSpinReels",
						params:[
							{prop:"_widthHeight", val:{wPercent:255 / 640, targetID:"_app"}},
							{prop:"x", val:{wPercent:320 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:534 / 960, targetID:"_app"}}
						]
					},
					{
						id:"btnStopReel",
						params:[
							{prop:"_widthHeight", val:{wPercent:255 / 640, targetID:"_app"}},
							{prop:"x", val:{wPercent:320 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:534 / 960, targetID:"_app"}}
						]
					},
					{
						id:"btnClaimStars",
						params:[
							{prop:"_widthHeight", val:{wPercent:500 / 640, targetID:"_app"}},
							{prop:"x", val:{wPercent:320 / 640, targetID:"_app"}},
							{prop:"y", val:{hPercent:534 / 960, targetID:"_app"}}
						]
					}
				]
			}
		],
		
		constructor: function() {
			//void
		},
		
		onInitializeView: function(viewID, ret) {
			if(viewID === this.viewID) {
				this.createVisuals();
				
				this.viewCompound = {
					config: this.viewConfig,
					layout: this.viewLayout,
					vElems: this.vElems,
					parentLayer: this.app.layers.views,
					dependantViews: [
						ViewType.BACKGROUND,
						ViewType.SIDE_BUSHES
					],
					cbcViewLayoutUpdateFinished: null,
					cbcViewActivated: {callback: this.onViewActivated, context: this}
				};
				
				ret.viewCompound = this.viewCompound;
			}
		},
		
		createVisuals:function() {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			this.vElems = {};
			
			this.createVisuals_base(this.vElems);
			this.createVisuals_titlePanel(this.vElems);
			this.createVisuals_slotsPanel(this.vElems);
		},
		
		onViewActivated: function(extraData) {
			this.updateData(extraData);
		},
		
		createVisuals_base:function(parentVElems) {
			var vElemsLocal = parentVElems;
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
		},
		
		createVisuals_titlePanel:function(parentVElems) {
			var vElemsLocal = parentVElems.titlePanel = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			vElemsLocal.bg = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plank2X");
			vElemsLocal.bg.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bg);
			
			this.createVisuals_titlePanel_subtitlePanel(vElemsLocal);
			
			vElemsLocal.imgTitle = this.app.popAsset(this.moduleBranch, "sprite", "viewVictoryBonus/title");
			vElemsLocal.imgTitle.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.imgTitle);
			
			vElemsLocal.btnSlotsPayout = this.app.popAsset(this.moduleBranch, "spriteButton", "uiGeneral/btnHelp_");
			vElemsLocal.btnSlotsPayout.anchor.set(0.5, 0.5);
			vElemsLocal.btnSlotsPayout.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnSlotsPayout);
		},
		
		createVisuals_titlePanel_subtitlePanel: function(parentVElems) {
			var vElemsLocal = parentVElems.subtitlePanel = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			vElemsLocal.bg = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/subtitleBg");
			vElemsLocal.bg.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.bg);
			
			vElemsLocal.iconStars = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/iconStars");
			vElemsLocal.iconStars.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.iconStars);
			
			vElemsLocal.lblStars = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "000",
				style: {
					font: "800 77px 'Dosis-ExtraBold'",
					fill: "#FFFFFF",
					align: "center"
				}
			});
			vElemsLocal.lblStars.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblStars);
		},
		
		updateData: function(extraData) {
			//asd_001;// update the games played and max lvl;
		},
		
		createVisuals_slotsPanel:function(parentVElems) {
			var vElemsLocal = parentVElems.slotsPanel = {isVContainer: true};
			
			vElemsLocal.container = this.app.popAsset(this.moduleBranch, "container");
			parentVElems.container.addChild(vElemsLocal.container);
			
			vElemsLocal.bg = this.app.popAsset(this.moduleBranch, "sprite", "uiGeneral/plankLarge");
			vElemsLocal.bg.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.bg);
			
			vElemsLocal.slotsBg = this.app.popAsset(this.moduleBranch, "sprite", "uiSlots/slotsBg");
			vElemsLocal.slotsBg.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.slotsBg);
			
			vElemsLocal.slotsFront = this.app.popAsset(this.moduleBranch, "sprite", "uiSlots/slotsFront");
			vElemsLocal.slotsFront.anchor.set(0, 0);
			vElemsLocal.container.addChild(vElemsLocal.slotsFront);
			
			vElemsLocal.reelMissedB = this.app.popAsset(this.moduleBranch, "sprite", "viewVictoryBonus/iconReelMissed");
			vElemsLocal.reelMissedB.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.reelMissedB);
			
			vElemsLocal.reelMissedC = this.app.popAsset(this.moduleBranch, "sprite", "viewVictoryBonus/iconReelMissed");
			vElemsLocal.reelMissedC.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.reelMissedC);
			
			vElemsLocal.currentBonusBg = this.app.popAsset(this.moduleBranch, "sprite", "viewVictoryBonus/bonusBg");
			vElemsLocal.currentBonusBg.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.currentBonusBg);
			
			vElemsLocal.lblCurrentBonus = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "+00%",
				style: {
					font: "800 40px 'Arial'",
					fill: "#FFFFFF",
					align: "center"
				}
			});
			vElemsLocal.lblCurrentBonus.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblCurrentBonus);
			
			vElemsLocal.nextBonusBg = this.app.popAsset(this.moduleBranch, "sprite", "viewVictoryBonus/bonusBg");
			vElemsLocal.nextBonusBg.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.nextBonusBg);
			
			vElemsLocal.lblNextBonus = this.app.popAsset(this.moduleBranch, "text", null, {
				text: "+00%",
				style: {
					font: "800 40px 'Arial'",
					fill: "#FFFFFF",
					align: "center"
				}
			});
			vElemsLocal.lblNextBonus.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.lblNextBonus);
			
			vElemsLocal.iconBonusArrow = this.app.popAsset(this.moduleBranch, "sprite", "viewVictoryBonus/arrowRight");
			vElemsLocal.iconBonusArrow.anchor.set(0.5, 0.5);
			vElemsLocal.container.addChild(vElemsLocal.iconBonusArrow);
			
			vElemsLocal.btnSpinReels = this.app.popAsset(this.moduleBranch, "spriteButton", "viewVictoryBonus/btnSpinReels_");
			vElemsLocal.btnSpinReels.anchor.set(0.5, 0.5);
			vElemsLocal.btnSpinReels.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnSpinReels);
			
			vElemsLocal.btnStopReel = this.app.popAsset(this.moduleBranch, "spriteButton", "viewVictoryBonus/btnStopReel_");
			vElemsLocal.btnStopReel.anchor.set(0.5, 0.5);
			vElemsLocal.btnStopReel.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnStopReel);
			
			vElemsLocal.btnClaimStars = this.app.popAsset(this.moduleBranch, "spriteButton", "viewVictoryBonus/btnClaimStars_");
			vElemsLocal.btnClaimStars.anchor.set(0.5, 0.5);
			vElemsLocal.btnClaimStars.cbcClick = {callback: this.onBtnClick, context: this};
			vElemsLocal.container.addChild(vElemsLocal.btnClaimStars);
		},
		
		onBtnClick:function(event) {
			if(!this.viewCompound || this.viewCompound.state.stateEnd.indexOf("sIn") != 0) {
				return ;
			}
			
			if(event.target === this.vElems.titlePanel.btnSlotsPayout) {
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.SLOTS_PAYOUT, "sIn", true, null, 0, null, null);
			}else if(event.target === this.vElems.slotsPanel.btnSpinReels) {
				console.log("asd dev btnClick btnSpinReel");
			}else if(event.target === this.vElems.slotsPanel.btnStopReel) {
				console.log("asd dev btnClick btnStopReel");
			}else if(event.target === this.vElems.slotsPanel.btnClaimStars) {
				console.log("asd dev btnClick btnClaimStars");
				
				this.publisher.publish(this.publishMsg.changeViewState, ViewType.VICTORY_BONUS, "sOut", true, null, 0, null, {
					viewID: ViewType.MAIN,
					stateEnd: "sIn",
					doTransition: true,
					extraData: null,
					delayMS: 0,
					cbcAnimationFinish: null,
					finishCommand: null
				});
			}else {
				this.postError("Err 6.");
				
				return null;
			}
		},
		
		destroy: function() {
			if(this.vElems) {
				this.app.cache.releaseVElems(this.moduleBranch, this.vElems);
				
				this.vElems = null;
			}
			
			this.__proto__.__proto__.destroy.call(this);
		}

	});
});
