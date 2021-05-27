import Card from "./Card";
import {ICoords} from "./interfaces";

export default class MinionCardForBoardPicker extends Card {
	public premium = false;
	// public bodyTextColor = "black";
        public bodyTextColor = null;
	// public bodyTextCoords = {
	// 	dx: 130,
	// 	dy: 622,
	// 	dWidth: 408,
	// 	dHeight: 176,
	// 	sWidth: 408,
	// 	sHeight: 176,
	// };
        public bodyTextCoords = null;
	public cardFoundationAsset = null;
	public cardFoundationCoords = null;
	public baseCardFrameAsset = "inplay_minion_hide_stats";

        public getCardFrameAsset(): string {
                return "inplay_minion_hide_stats";
        }

        public baseCardFrameCoords = {
		sWidth: 528,
		sHeight: 793,
		dx: 35,
		dy: 25,
		dWidth: 528,
		dHeight: 793,
	};
	// public baseCardFrameCoords = {
	// 	sWidth: 528,
	// 	sHeight: 793,
	// 	dx: 70,
	// 	dy: 89,
	// 	dWidth: 528,
	// 	dHeight: 793,
	// };
	public baseRarityGemAsset = "rarity-minion-";
	public eliteDragonAsset = "elite-minion";
	public eliteDragonCoords = {
		dx: 188,
		dy: 52,
		dWidth: 436,
		dHeight: 325,
		sWidth: 436,
		sHeight: 325,
	};
	// public nameBannerAsset = "name-banner-minion";
        public nameBannerAsset = null; // Remove name banner
        public nameBannerCoords = null; // Remove name itself
	// public raceBannerAsset = "race-banner";
        public raceBannerAsset = null;
	// public rarityGemCoords = {dx: 263, dy: 532};
        public rarityGemCoords = null;
	// public nameBannerCoords = {
	// 	sWidth: 485,
	// 	sHeight: 113,
	// 	dx: 96,
	// 	dy: 469,
	// 	dWidth: 485,
	// 	dHeight: 113,
	// };
        public nameTextCurve = null;
	// public nameTextCurve = {
	// 	pathMiddle: 0.55,
	// 	maxWidth: 450,
	// 	curve: [{x: 0, y: 88}, {x: 98, y: 112}, {x: 294, y: 13}, {x: 460, y: 80}],
	// };
	public artCoords: ICoords = {
		sWidth: 461,
		sHeight: 461,
		dx: 105,
		dy: 100,
		dWidth: 461,
		dHeight: 461,
	};
	public artClipPolygon = [
		{x: 335, y: 102},
		{x: 292, y: 110},
		{x: 256, y: 131},
		{x: 222, y: 163},
		{x: 195, y: 203},
		{x: 171, y: 273},
		{x: 163, y: 330},
		{x: 170, y: 398},
		{x: 200, y: 474},
		{x: 266, y: 547},
		{x: 302, y: 563},
		{x: 343, y: 567},
		{x: 406, y: 544},
		{x: 449, y: 506},
		{x: 488, y: 432},
		{x: 505, y: 346},
		{x: 494, y: 255},
		{x: 460, y: 172},
		{x: 425, y: 135},
		{x: 385, y: 111},
	];

	// public getAttackGemCoords() {
	// 	return {
	// 		sWidth: 154,
	// 		sHeight: 173,
	// 		dx: 135,
	// 		dy: 420,
	// 		dWidth: 154,
	// 		dHeight: 173,
	// 	};
	// }

	// public getAttackTextCoords() {
        //         return {dx: 215, dy: 515};
	// }

        public getGemTextSize() : number {
               return 80;
        }

	// public getHealthGemCoords() {
	// 	return {
	// 		sWidth: 109,
	// 		sHeight: 164,
	// 		dx: 405,
	// 		dy: 420,
	// 		dWidth: 109,
	// 		dHeight: 164,
	// 	};
	// }

	// public getHealthTextCoords() {
	// 	// return {dx: 556, dy: 825};
        //         // return {dx: 455, dy: 515};
        //         return null;
	// }

	// public getAttackGemAsset() {
	// 	return "attack-minion";
	// }

	// public getHealthGemAsset() {
	// 	// return "health";
        //         return null;
	// }

        // Overload, removes mana/cost gem
        public getCostGemAsset(): string {
                return "";
        }

        // Overload, removes rarity gem
        public getRarityGemAsset(): string {
                return "";
        }

	public getCostTextCoords() : ICoords {
		return null;
	}

	public getWatermarkCoords() {
		let dy = 604;
		if (this.raceBanner) {
			dy -= 10; // Shift up
		}

		return {
			dx: 231,
			dy: dy,
			dWidth: 225,
			dHeight: 225,
		};
	}
}
