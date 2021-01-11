/**
 * Created by Adrian on 11/18/2014.
 */
define([
	"dojo/_base/declare",
	"app/main/ModuleBase",
	
	"lib/pixijs/pixi",
	"app/util/MathUtil",
	"app/util/SafeNumber"
], function(declare, ModuleBase, 
	PIXI, MathUtil, SafeNumber
){
	"use strict";
	return declare("app.main.PlayerProfileHandler", ModuleBase, {
		moduleBranch: "main",
		modulePrerequisites: undefined,
		subscribeListeners:[
			{channel:"initializeProfileData", cbName:"onInitializeProfileData", isUnique:false}
		],
		publishMsg: {
			profileDataChange:{channel:"profileDataChange", listenerType:"list", mustExist:false},
			
			profileDataDiamondsChange:{channel:"profileDataDiamondsChange", listenerType:"list", mustExist:false},
			profileDataStarsChange:{channel:"profileDataStarsChange", listenerType:"list", mustExist:false},
			profileDataGamesPlayedChange:{channel:"profileDataGamesPlayedChange", listenerType:"list", mustExist:false},
			profileDataMaxLevelChange:{channel:"profileDataMaxLevelChange", listenerType:"list", mustExist:false},
			
			getApp:{channel:"getApp", listenerType:"unique", mustExist:true}
		},
		
		app:null,
		
		rawProfileData: null,
		
		constructor: function() {
			//void
		},
		
		onInitializeProfileData: function () {
			this.app = this.publisher.publish(this.publishMsg.getApp);
			
			// ASD_DEV;
			var resetProfileOnLoad = true;
			
			var rawProfileString = window.localStorage.getItem("profileData561263587");
			if(resetProfileOnLoad || !rawProfileString) {
				var snDiamonds = new SafeNumber(50);
				var snStars = new SafeNumber(0);
				var snGamesPlayed = new SafeNumber(0);
				var snMaxLevel = new SafeNumber(0);
				var snLastFreeDiamondsTimestamp = new SafeNumber(0);
				
				this.rawProfileData = {
					_version: 1,
					
					_diamondsA: snDiamonds.valueA,
					_diamondsB: snDiamonds.valueB,
					
					_starsA: snStars.valueA,
					_starsB: snStars.valueB,
					
					_gamesPlayedA: snGamesPlayed.valueA,
					_gamesPlayedB: snGamesPlayed.valueB,
					
					_maxLevelA: snMaxLevel.valueA,
					_maxLevelB: snMaxLevel.valueB,
					
					_lastFreeDiamondsTimestampA: snLastFreeDiamondsTimestamp.valueA,
					_lastFreeDiamondsTimestampB: snLastFreeDiamondsTimestamp.valueB
				};
				
				rawProfileString = JSON.stringify(this.rawProfileData);
				
				window.localStorage.setItem("profileData561263587", rawProfileString);
			}else {
				this.rawProfileData = JSON.parse(rawProfileString);
			}
			
			this.app.profileData = {
				getDiamonds: this.getDiamonds.bind(this),
				addDiamonds: this.addDiamonds.bind(this),
				getStars: this.getStars.bind(this),
				addStars: this.addStars.bind(this),
				getGamesPlayed: this.getGamesPlayed.bind(this),
				incrementGamesPlayed: this.incrementGamesPlayed.bind(this),
				getMaxLevel: this.getMaxLevel.bind(this),
				setMaxLevel: this.setMaxLevel.bind(this),
				getLastFreeDiamondsTimestamp: this.getLastFreeDiamondsTimestamp.bind(this),
				setLastFreeDiamondsTimestamp: this.setLastFreeDiamondsTimestamp.bind(this)
			};
		},
		
		saveProfileData: function() {
			var rawProfileString = JSON.stringify(this.rawProfileData);
			
			window.localStorage.setItem("profileData561263587", rawProfileString);
		},
		
		getDiamonds: function() {
			return this.rawProfileData._diamondsA + this.rawProfileData._diamondsB;
		},
		
		addDiamonds: function(diamondsCount) {
			if(diamondsCount == 0) {
				return ;
			}
			
			var snValue = new SafeNumber(this.getDiamonds() + diamondsCount);
			
			this.rawProfileData._diamondsA = snValue.valueA;
			this.rawProfileData._diamondsB = snValue.valueB;
			
			this.saveProfileData();
			
			this.publisher.publish(this.publishMsg.profileDataChange);
			this.publisher.publish(this.publishMsg.profileDataDiamondsChange);
		},
		
		getStars: function() {
			return this.rawProfileData._starsA + this.rawProfileData._starsB;
		},
		
		addStars: function(starsCount) {
			if(starsCount == 0) {
				return ;
			}
			
			var snValue = new SafeNumber(this.getStars() + starsCount);
			
			this.rawProfileData._starsA = snValue.valueA;
			this.rawProfileData._starsB = snValue.valueB;
			
			this.saveProfileData();
			
			this.publisher.publish(this.publishMsg.profileDataChange);
			this.publisher.publish(this.publishMsg.profileDataStarsChange);
		},
		
		getGamesPlayed: function() {
			return this.rawProfileData._gamesPlayedA + this.rawProfileData._gamesPlayedB;
		},
		
		incrementGamesPlayed: function() {
			var snValue = new SafeNumber(this.getGamesPlayed() + 1);
			
			this.rawProfileData._gamesPlayedA = snValue.valueA;
			this.rawProfileData._gamesPlayedB = snValue.valueB;
			
			this.saveProfileData();
			
			this.publisher.publish(this.publishMsg.profileDataChange);
			this.publisher.publish(this.publishMsg.profileDataGamesPlayedChange);
		},
		
		getMaxLevel: function() {
			return this.rawProfileData._maxLevelA + this.rawProfileData._maxLevelB;
		},
		
		getLastFreeDiamondsTimestamp: function() {
			return this.rawProfileData._lastFreeDiamondsTimestampA + this.rawProfileData._lastFreeDiamondsTimestampB;
		},
		
		setLastFreeDiamondsTimestamp: function(lastFreeDiamondsTimestamp) {
			var snValue = new SafeNumber(lastFreeDiamondsTimestamp);
			
			this.rawProfileData._lastFreeDiamondsTimestampA = snValue.valueA;
			this.rawProfileData._lastFreeDiamondsTimestampB = snValue.valueB;
		},
		
		setMaxLevel: function(maxLevel) {
			if(this.getMaxLevel() == maxLevel) {
				return ;
			}
			var snValue = new SafeNumber(maxLevel);
			
			this.rawProfileData._maxLevelA = snValue.valueA;
			this.rawProfileData._maxLevelB = snValue.valueB;
			
			this.saveProfileData();
			
			this.publisher.publish(this.publishMsg.profileDataChange);
			this.publisher.publish(this.publishMsg.profileDataMaxLevelChange);
		},
		
		destroy: function() {
			// void
			
			this.__proto__.__proto__.destroy.call(this);
		}
		
	});
});