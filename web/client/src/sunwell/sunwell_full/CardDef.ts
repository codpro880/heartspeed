import {CardClass, CardSet, CardType, MultiClassGroup, Race, Rarity} from "./Enums";
import {cleanEnum} from "./helpers";

export default class CardDef {
	public attack: number;
	public armor: number;
	public cardClass: CardClass;
	public cardSet: CardSet;
	public collectionText: string;
	public cost: number;
	public costsHealth: boolean;
        public deathrattle: boolean;
        public divineShield: boolean;
        public hasTriggeredEffect: boolean;
	public elite: boolean;
	public health: number;
	public hideStats: boolean;
	public id: string;
	public name: string;
	public multiClassGroup: MultiClassGroup;
        public poisonous: boolean;
	public rarity: Rarity;
	public race: Race;
        public reborn: boolean;
	public silenced: boolean;
	public taunt: boolean;
	public text: string;
	public type: CardType;

	constructor(props: any) {
		this.attack = props.attack || 0;
		this.armor = props.armor || 0;
		this.cardClass = cleanEnum(props.cardClass, CardClass) as CardClass;
		this.cardSet = cleanEnum(props.set, CardSet) as CardSet;
		this.cost = props.cost || 0;
		this.costsHealth = props.costsHealth || false;
                this.deathrattle = props.deathrattle || false;
                this.divineShield = props.divineShield || false;
                this.hasTriggeredEffect = props.hasTriggeredEffect || false;
		this.elite = props.elite || false;
		this.health = props.health || 0;
		this.hideStats = props.hideStats || false;
		this.multiClassGroup = cleanEnum(props.multiClassGroup, MultiClassGroup) as MultiClassGroup;
		this.name = props.name || "";
                this.poisonous = props.poisonous || false;
		this.race = cleanEnum(props.race, Race) as Race;
		this.rarity = cleanEnum(props.rarity, Rarity) as Rarity;
                this.reborn = props.reborn || false;
		this.silenced = props.silenced || false;
		this.taunt = props.taunt || false;
		this.type = cleanEnum(props.type, CardType) as CardType;

		if (this.type === CardType.WEAPON && props.durability) {
			// Weapons alias health to durability
			this.health = props.durability;
		} else if (this.type === CardType.HERO && props.armor) {
			// Hero health gem is Armor
			this.health = props.armor;
		}

		this.collectionText = props.collectionText || "";
		this.text = props.text || "";
	}
}
