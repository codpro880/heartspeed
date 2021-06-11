import React, { useReducer, useRef } from 'react';

import { Container, Stage, Sprite, useTick, } from '@inlet/react-pixi';
import Sunwell from "../sunwell/sunwell_full/Sunwell.ts"

// import './Rollout.css';

// I'm thinking theres a better way to do this...
// TODO: Reactify these constants?
const BOARD_WIDTH = 600 * 2;
const BOARD_HEIGHT = 350 * 2;
const BOARD_WIDTH_FUDGE = -BOARD_WIDTH / 20.0; // -30 at 600x350
const TOP_BOARD_HEIGHT_FUDGE = -BOARD_HEIGHT / 12.0; // -25 at 600x350
const BOTTOM_BOARD_HEIGHT_FUDGE = -BOARD_HEIGHT / 4.5; // -70 at 600x350
const CENTERING_FUDGE_FACTOR = -BOARD_WIDTH / 40.0; // Board image is not quite symetric (e.g. hero portrait not quite centered)
const CARD_WIDTH_DELTA = BOARD_WIDTH / 10.0;
const MAX_NUM_CARDS = 7;

const PICKER_WIDTH = 200;
const PICKER_HEIGHT = BOARD_HEIGHT;

function getBgCardJson() {
    var json = [
    {
        "name": "Acolyte of C'Thun",
        "tier": 1,
        "race": "NEUTRAL"
    },
    {
        "name": "Alleycat",
        "tier": 1,
        "race": "BEAST"
    },
    {
        "name": "Amalgam",
        "tier": 1,
        "race": "ALL"
    },
    {
        "name": "Deck Swabbie",
        "tier": 1,
        "race": "PIRATE"
    },
    {
        "name": "Dragonspawn Lieutenant",
        "tier": 1,
        "race": "DRAGON"
    },
    {
        "name": "Fiendish Servant",
        "tier": 1,
        "race": "DEMON"
    },
    {
        "name": "Fish of N'Zoth",
        "tier": 1,
        "race": "BEAST"
    },
    {
        "name": "Micro Machine",
        "tier": 1,
        "race": "MECHANICAL"
    },
    {
        "name": "Micro Mummy",
        "tier": 1,
        "race": "MECHANICAL"
    },
    {
        "name": "Murloc Scout",
        "tier": 1,
        "race": "MURLOC"
    },
    {
        "name": "Murloc Tidecaller",
        "tier": 1,
        "race": "MURLOC"
    },
    {
        "name": "Murloc Tidehunter",
        "tier": 1,
        "race": "MURLOC"
    },
    {
        "name": "Razorfen Geomancer",
        "tier": 1,
        "race": "QUILBOAR"
    },
    {
        "name": "Red Whelp",
        "tier": 1,
        "race": "DRAGON"
    },
    {
        "name": "Refreshing Anomaly",
        "tier": 1,
        "race": "ELEMENTAL"
    },
    {
        "name": "Rockpool Hunter",
        "tier": 1,
        "race": "MURLOC"
    },
    {
        "name": "Scallywag",
        "tier": 1,
        "race": "PIRATE"
    },
    {
        "name": "Scavenging Hyena",
        "tier": 1,
        "race": "BEAST"
    },
    {
        "name": "Sellemental",
        "tier": 1,
        "race": "ELEMENTAL"
    },
    {
        "name": "Sun-Bacon Relaxer",
        "tier": 1,
        "race": "QUILBOAR"
    },
    {
        "name": "Vulgar Homunculus",
        "tier": 1,
        "race": "DEMON"
    },
    {
        "name": "Wrath Weaver",
        "tier": 1,
        "race": "NEUTRAL"
    },
    {
        "name": "Freedealing Gambler",
        "tier": 2,
        "race": "PIRATE"
    },
    {
        "name": "Glyph Guardian",
        "tier": 2,
        "race": "DRAGON"
    },
    {
        "name": "Harvest Golem",
        "tier": 2,
        "race": "MECHANICAL"
    },
    {
        "name": "Imprisoner",
        "tier": 2,
        "race": "DEMON"
    },
    {
        "name": "Kaboom Bot",
        "tier": 2,
        "race": "MECHANICAL"
    },
    {
        "name": "Menagerie Mug",
        "tier": 2,
        "race": "NEUTRAL"
    },
    {
        "name": "Metaltooth Leaper",
        "tier": 2,
        "race": "MECHANICAL"
    },
    {
        "name": "Molten Rock",
        "tier": 2,
        "race": "ELEMENTAL"
    },
    {
        "name": "Murloc Warleader",
        "tier": 2,
        "race": "MURLOC"
    },
    {
        "name": "Nathrezim Overseer",
        "tier": 2,
        "race": "DEMON"
    },
    {
        "name": "Old Murk-Eye",
        "tier": 2,
        "race": "MURLOC"
    },
    {
        "name": "Pack Leader",
        "tier": 2,
        "race": "NEUTRAL"
    },
    {
        "name": "Party Elemental",
        "tier": 2,
        "race": "ELEMENTAL"
    },
    {
        "name": "Prophet of the Boar",
        "tier": 2,
        "race": "NEUTRAL"
    },
    {
        "name": "Rabid Saurolisk",
        "tier": 2,
        "race": "BEAST"
    },
    {
        "name": "Roadboar",
        "tier": 2,
        "race": "QUILBOAR"
    },
    {
        "name": "Selfless Hero",
        "tier": 2,
        "race": "NEUTRAL"
    },
    {
        "name": "Southsea Captain",
        "tier": 2,
        "race": "PIRATE"
    },
    {
        "name": "Spawn of N'Zoth",
        "tier": 2,
        "race": "NEUTRAL"
    },
    {
        "name": "Steward of Time",
        "tier": 2,
        "race": "DRAGON"
    },
    {
        "name": "Tormented Ritualist",
        "tier": 2,
        "race": "NEUTRAL"
    },
    {
        "name": "Tough Tusk",
        "tier": 2,
        "race": "QUILBOAR"
    },
    {
        "name": "Unstable Ghoul",
        "tier": 2,
        "race": "NEUTRAL"
    },
    {
        "name": "Waxrider Togwaggle",
        "tier": 2,
        "race": "NEUTRAL"
    },
    {
        "name": "Yo-Ho-Ogre",
        "tier": 2,
        "race": "PIRATE"
    },
    {
        "name": "Arcane Assistant",
        "tier": 3,
        "race": "ELEMENTAL"
    },
    {
        "name": "Arm of the Empire",
        "tier": 3,
        "race": "NEUTRAL"
    },
    {
        "name": "Bannerboar",
        "tier": 3,
        "race": "QUILBOAR"
    },
    {
        "name": "Bloodsail Cannoneer",
        "tier": 3,
        "race": "PIRATE"
    },
    {
        "name": "Bristleback Brute",
        "tier": 3,
        "race": "QUILBOAR"
    },
    {
        "name": "Bronze Warden",
        "tier": 3,
        "race": "DRAGON"
    },
    {
        "name": "Coldlight Seer",
        "tier": 3,
        "race": "MURLOC"
    },
    {
        "name": "Crackling Cyclone",
        "tier": 3,
        "race": "ELEMENTAL"
    },
    {
        "name": "Crystalweaver",
        "tier": 3,
        "race": "NEUTRAL"
    },
    {
        "name": "Deflect-o-Bot",
        "tier": 3,
        "race": "MECHANICAL"
    },
    {
        "name": "Felfin Navigator",
        "tier": 3,
        "race": "MURLOC"
    },
    {
        "name": "Hangry Dragon",
        "tier": 3,
        "race": "DRAGON"
    },
    {
        "name": "Houndmaster",
        "tier": 3,
        "race": "NEUTRAL"
    },
    {
        "name": "Imp Gang Boss",
        "tier": 3,
        "race": "DEMON"
    },
    {
        "name": "Infested Wolf",
        "tier": 3,
        "race": "BEAST"
    },
    {
        "name": "Iron Sensei",
        "tier": 3,
        "race": "MECHANICAL"
    },
    {
        "name": "Khadgar",
        "tier": 3,
        "race": "NEUTRAL"
    },
    {
        "name": "Monstrous Macaw",
        "tier": 3,
        "race": "BEAST"
    },
    {
        "name": "Necrolyte",
        "tier": 3,
        "race": "QUILBOAR"
    },
    {
        "name": "Rat Pack",
        "tier": 3,
        "race": "BEAST"
    },
    {
        "name": "Replicating Menace",
        "tier": 3,
        "race": "MECHANICAL"
    },
    {
        "name": "Salty Looter",
        "tier": 3,
        "race": "PIRATE"
    },
    {
        "name": "Screwjank Clunker",
        "tier": 3,
        "race": "MECHANICAL"
    },
    {
        "name": "Soul Devourer",
        "tier": 3,
        "race": "DEMON"
    },
    {
        "name": "Soul Juggler",
        "tier": 3,
        "race": "NEUTRAL"
    },
    {
        "name": "Southsea Strongarm",
        "tier": 3,
        "race": "PIRATE"
    },
    {
        "name": "Stasis Elemental",
        "tier": 3,
        "race": "ELEMENTAL"
    },
    {
        "name": "Thorncaller",
        "tier": 3,
        "race": "QUILBOAR"
    },
    {
        "name": "Twilight Emissary",
        "tier": 3,
        "race": "DRAGON"
    },
    {
        "name": "Warden of Old",
        "tier": 3,
        "race": "NEUTRAL"
    },
    {
        "name": "Annoy-o-Module",
        "tier": 4,
        "race": "MECHANICAL"
    },
    {
        "name": "Bigfernal",
        "tier": 4,
        "race": "DEMON"
    },
    {
        "name": "Bolvar, Fireblood",
        "tier": 4,
        "race": "NEUTRAL"
    },
    {
        "name": "Bonker",
        "tier": 4,
        "race": "QUILBOAR"
    },
    {
        "name": "Cave Hydra",
        "tier": 4,
        "race": "BEAST"
    },
    {
        "name": "Champion of Y'Shaarj",
        "tier": 4,
        "race": "NEUTRAL"
    },
    {
        "name": "Cobalt Scalebane",
        "tier": 4,
        "race": "DRAGON"
    },
    {
        "name": "Defender of Argus",
        "tier": 4,
        "race": "NEUTRAL"
    },
    {
        "name": "Drakonid Enforcer",
        "tier": 4,
        "race": "DRAGON"
    },
    {
        "name": "Dynamic Duo",
        "tier": 4,
        "race": "QUILBOAR"
    },
    {
        "name": "Goldgrubber",
        "tier": 4,
        "race": "PIRATE"
    },
    {
        "name": "Groundshaker",
        "tier": 4,
        "race": "QUILBOAR"
    },
    {
        "name": "Herald of Flame",
        "tier": 4,
        "race": "DRAGON"
    },
    {
        "name": "Junkbot",
        "tier": 4,
        "race": "MECHANICAL"
    },
    {
        "name": "Majordomo Executus",
        "tier": 4,
        "race": "NEUTRAL"
    },
    {
        "name": "Mechano-Egg",
        "tier": 4,
        "race": "MECHANICAL"
    },
    {
        "name": "Menagerie Jug",
        "tier": 4,
        "race": "NEUTRAL"
    },
    {
        "name": "Primalfin",
        "tier": 4,
        "race": "MURLOC"
    },
    {
        "name": "Primalfin Lookout",
        "tier": 4,
        "race": "MURLOC"
    },
    {
        "name": "Qiraji Harbinger",
        "tier": 4,
        "race": "NEUTRAL"
    },
    {
        "name": "Ring Matron",
        "tier": 4,
        "race": "DEMON"
    },
    {
        "name": "Ripsnarl Captain",
        "tier": 4,
        "race": "PIRATE"
    },
    {
        "name": "Savannah Highmane",
        "tier": 4,
        "race": "BEAST"
    },
    {
        "name": "Security Rover",
        "tier": 4,
        "race": "MECHANICAL"
    },
    {
        "name": "Siegebreaker",
        "tier": 4,
        "race": "DEMON"
    },
    {
        "name": "Toxfin",
        "tier": 4,
        "race": "MURLOC"
    },
    {
        "name": "Virmen Sensei",
        "tier": 4,
        "race": "NEUTRAL"
    },
    {
        "name": "Wildfire Elemental",
        "tier": 4,
        "race": "ELEMENTAL"
    },
    {
        "name": "Agamaggan, the Great Boar",
        "tier": 5,
        "race": "BEAST"
    },
    {
        "name": "Aggem Thorncurse",
        "tier": 5,
        "race": "QUILBOAR"
    },
    {
        "name": "Annihilan Battlemaster",
        "tier": 5,
        "race": "DEMON"
    },
    {
        "name": "Baron Rivendare",
        "tier": 5,
        "race": "NEUTRAL"
    },
    {
        "name": "Brann Bronzebeard",
        "tier": 5,
        "race": "NEUTRAL"
    },
    {
        "name": "Bristleback Knight",
        "tier": 5,
        "race": "QUILBOAR"
    },
    {
        "name": "Cap'n Hoggarr",
        "tier": 5,
        "race": "PIRATE"
    },
    {
        "name": "Deadly Spore",
        "tier": 5,
        "race": "NEUTRAL"
    },
    {
        "name": "Faceless Taverngoer",
        "tier": 5,
        "race": "NEUTRAL"
    },
    {
        "name": "Ironhide Direhorn",
        "tier": 5,
        "race": "BEAST"
    },
    {
        "name": "King Bagurgle",
        "tier": 5,
        "race": "MURLOC"
    },
    {
        "name": "Lightfang Enforcer",
        "tier": 5,
        "race": "NEUTRAL"
    },
    {
        "name": "Mal'Ganis",
        "tier": 5,
        "race": "DEMON"
    },
    {
        "name": "Mama Bear",
        "tier": 5,
        "race": "BEAST"
    },
    {
        "name": "Murozond",
        "tier": 5,
        "race": "DRAGON"
    },
    {
        "name": "Mythrax the Unraveler",
        "tier": 5,
        "race": "NEUTRAL"
    },
    {
        "name": "Nat Pagle, Extreme Angler",
        "tier": 5,
        "race": "PIRATE"
    },
    {
        "name": "Nomi, Kitchen Nightmare",
        "tier": 5,
        "race": "NEUTRAL"
    },
    {
        "name": "Razorgore, the Untamed",
        "tier": 5,
        "race": "DRAGON"
    },
    {
        "name": "Seabreaker Goliath",
        "tier": 5,
        "race": "PIRATE"
    },
    {
        "name": "Sneed's Old Shredder",
        "tier": 5,
        "race": "MECHANICAL"
    },
    {
        "name": "Strongshell Scavenger",
        "tier": 5,
        "race": "NEUTRAL"
    },
    {
        "name": "Tavern Tempest",
        "tier": 5,
        "race": "ELEMENTAL"
    },
    {
        "name": "Voidlord",
        "tier": 5,
        "race": "DEMON"
    },
    {
        "name": "Amalgadon",
        "tier": 6,
        "race": "ALL"
    },
    {
        "name": "Captain Flat Tusk",
        "tier": 6,
        "race": "QUILBOAR"
    },
    {
        "name": "Charlga",
        "tier": 6,
        "race": "QUILBOAR"
    },
    {
        "name": "Dread Admiral Eliza",
        "tier": 6,
        "race": "PIRATE"
    },
    {
        "name": "Foe Reaper 4000",
        "tier": 6,
        "race": "MECHANICAL"
    },
    {
        "name": "Gentle Djinni",
        "tier": 6,
        "race": "ELEMENTAL"
    },
    {
        "name": "Ghastcoiler",
        "tier": 6,
        "race": "BEAST"
    },
    {
        "name": "Goldrinn, the Great Wolf",
        "tier": 6,
        "race": "BEAST"
    },
    {
        "name": "Imp Mama",
        "tier": 6,
        "race": "DEMON"
    },
    {
        "name": "Kalecgos, Arcane Aspect",
        "tier": 6,
        "race": "DRAGON"
    },
    {
        "name": "Kangor's Apprentice",
        "tier": 6,
        "race": "NEUTRAL"
    },
    {
        "name": "Lieutenant Garr",
        "tier": 6,
        "race": "ELEMENTAL"
    },
    {
        "name": "Lil' Rag",
        "tier": 6,
        "race": "ELEMENTAL"
    },
    {
        "name": "Maexxna",
        "tier": 6,
        "race": "BEAST"
    },
    {
        "name": "Nadina the Red",
        "tier": 6,
        "race": "NEUTRAL"
    },
    {
        "name": "The Tide Razor",
        "tier": 6,
        "race": "NEUTRAL"
    },
    {
        "name": "Zapp Slywick",
        "tier": 6,
        "race": "NEUTRAL"
    }
]
    return json;
}
 
// Glyph gaurdian reborn from Deathknight hero power
function getTestCardFrames() {
    var json = [
    {
        "attacker_pos": 0,
        "b1": [
            {
                "attack": 6,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 10,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            },
            {
                "attack": 13,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 9,
                "mechanics": "",
                "name": "Wildfire Elemental",
                "race": "ELEMENTAL",
                "rarity": "",
                "tavern_tier": 4,
                "type": "MINION"
            },
            {
                "attack": 7,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Wildfire Elemental",
                "race": "ELEMENTAL",
                "rarity": "",
                "tavern_tier": 4,
                "type": "MINION"
            },
            {
                "attack": 4,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 4,
                "mechanics": "",
                "name": "Lil' Rag",
                "race": "ELEMENTAL",
                "rarity": "",
                "tavern_tier": 6,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 4,
                "card_class": "NEUTRAL",
                "cost": 7,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 4,
                "mechanics": "",
                "name": "Nomi, Kitchen Nightmare",
                "race": "",
                "rarity": "",
                "tavern_tier": 5,
                "type": "MINION"
            },
            {
                "attack": 10,
                "card_class": "NEUTRAL",
                "cost": 5,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 10,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Hangry Dragon (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "b1_turn": 1,
        "b2": [
            {
                "attack": 8,
                "card_class": "NEUTRAL",
                "cost": 2,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 11,
                "mechanics": "['BATTLECRY']",
                "name": "Rockpool Hunter (Golden)",
                "race": "MURLOC",
                "rarity": "COMMON",
                "tavern_tier": 1,
                "type": "MINION"
            },
            {
                "attack": 13,
                "card_class": "ROGUE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 10,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Salty Looter",
                "race": "PIRATE",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 4,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 9,
                "card_class": "ROGUE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 6,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Salty Looter",
                "race": "PIRATE",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Lightfang Enforcer",
                "race": "",
                "rarity": "EPIC",
                "tavern_tier": 5,
                "type": "MINION"
            },
            {
                "attack": 7,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": true,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 9,
                "mechanics": "['TRIGGER_VISUAL', 'TAUNT']",
                "name": "Yo-Ho-Ogre",
                "race": "PIRATE",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "defender_pos": 6
    },
    {
        "attacker_pos": 0,
        "b1": [
            {
                "attack": 18,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            },
            {
                "attack": 13,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 9,
                "mechanics": "",
                "name": "Wildfire Elemental",
                "race": "ELEMENTAL",
                "rarity": "",
                "tavern_tier": 4,
                "type": "MINION"
            },
            {
                "attack": 7,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Wildfire Elemental",
                "race": "ELEMENTAL",
                "rarity": "",
                "tavern_tier": 4,
                "type": "MINION"
            },
            {
                "attack": 4,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 4,
                "mechanics": "",
                "name": "Lil' Rag",
                "race": "ELEMENTAL",
                "rarity": "",
                "tavern_tier": 6,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 4,
                "card_class": "NEUTRAL",
                "cost": 7,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 4,
                "mechanics": "",
                "name": "Nomi, Kitchen Nightmare",
                "race": "",
                "rarity": "",
                "tavern_tier": 5,
                "type": "MINION"
            },
            {
                "attack": 10,
                "card_class": "NEUTRAL",
                "cost": 5,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 10,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Hangry Dragon (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "b1_turn": 0,
        "b2": [
            {
                "attack": 8,
                "card_class": "NEUTRAL",
                "cost": 2,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 11,
                "mechanics": "['BATTLECRY']",
                "name": "Rockpool Hunter (Golden)",
                "race": "MURLOC",
                "rarity": "COMMON",
                "tavern_tier": 1,
                "type": "MINION"
            },
            {
                "attack": 13,
                "card_class": "ROGUE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 10,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Salty Looter",
                "race": "PIRATE",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 4,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 9,
                "card_class": "ROGUE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 6,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Salty Looter",
                "race": "PIRATE",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Lightfang Enforcer",
                "race": "",
                "rarity": "EPIC",
                "tavern_tier": 5,
                "type": "MINION"
            }
        ],
        "defender_pos": 3
    },
    {
        "attacker_pos": 1,
        "b1": [
            {
                "attack": 18,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            },
            {
                "attack": 13,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 9,
                "mechanics": "",
                "name": "Wildfire Elemental",
                "race": "ELEMENTAL",
                "rarity": "",
                "tavern_tier": 4,
                "type": "MINION"
            },
            {
                "attack": 7,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Wildfire Elemental",
                "race": "ELEMENTAL",
                "rarity": "",
                "tavern_tier": 4,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 4,
                "card_class": "NEUTRAL",
                "cost": 7,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 4,
                "mechanics": "",
                "name": "Nomi, Kitchen Nightmare",
                "race": "",
                "rarity": "",
                "tavern_tier": 5,
                "type": "MINION"
            },
            {
                "attack": 10,
                "card_class": "NEUTRAL",
                "cost": 5,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 10,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Hangry Dragon (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "b1_turn": 1,
        "b2": [
            {
                "attack": 8,
                "card_class": "NEUTRAL",
                "cost": 2,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 7,
                "mechanics": "['BATTLECRY']",
                "name": "Rockpool Hunter (Golden)",
                "race": "MURLOC",
                "rarity": "COMMON",
                "tavern_tier": 1,
                "type": "MINION"
            },
            {
                "attack": 13,
                "card_class": "ROGUE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 10,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Salty Looter",
                "race": "PIRATE",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 4,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 9,
                "card_class": "ROGUE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 6,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Salty Looter",
                "race": "PIRATE",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Lightfang Enforcer",
                "race": "",
                "rarity": "EPIC",
                "tavern_tier": 5,
                "type": "MINION"
            }
        ],
        "defender_pos": 0
    },
    {
        "attacker_pos": 0,
        "b1": [
            {
                "attack": 18,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            },
            {
                "attack": 13,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 1,
                "mechanics": "",
                "name": "Wildfire Elemental",
                "race": "ELEMENTAL",
                "rarity": "",
                "tavern_tier": 4,
                "type": "MINION"
            },
            {
                "attack": 7,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Wildfire Elemental",
                "race": "ELEMENTAL",
                "rarity": "",
                "tavern_tier": 4,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 4,
                "card_class": "NEUTRAL",
                "cost": 7,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 4,
                "mechanics": "",
                "name": "Nomi, Kitchen Nightmare",
                "race": "",
                "rarity": "",
                "tavern_tier": 5,
                "type": "MINION"
            },
            {
                "attack": 10,
                "card_class": "NEUTRAL",
                "cost": 5,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 10,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Hangry Dragon (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "b1_turn": 0,
        "b2": [
            {
                "attack": 13,
                "card_class": "ROGUE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 4,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Salty Looter",
                "race": "PIRATE",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 4,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 9,
                "card_class": "ROGUE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 6,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Salty Looter",
                "race": "PIRATE",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Lightfang Enforcer",
                "race": "",
                "rarity": "EPIC",
                "tavern_tier": 5,
                "type": "MINION"
            }
        ],
        "defender_pos": 1
    },
    {
        "attacker_pos": 1,
        "b1": [
            {
                "attack": 18,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            },
            {
                "attack": 7,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Wildfire Elemental",
                "race": "ELEMENTAL",
                "rarity": "",
                "tavern_tier": 4,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 4,
                "card_class": "NEUTRAL",
                "cost": 7,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 4,
                "mechanics": "",
                "name": "Nomi, Kitchen Nightmare",
                "race": "",
                "rarity": "",
                "tavern_tier": 5,
                "type": "MINION"
            },
            {
                "attack": 10,
                "card_class": "NEUTRAL",
                "cost": 5,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 10,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Hangry Dragon (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "b1_turn": 1,
        "b2": [
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 4,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 9,
                "card_class": "ROGUE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 6,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Salty Looter",
                "race": "PIRATE",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Lightfang Enforcer",
                "race": "",
                "rarity": "EPIC",
                "tavern_tier": 5,
                "type": "MINION"
            }
        ],
        "defender_pos": 0
    },
    {
        "attacker_pos": 0,
        "b1": [
            {
                "attack": 18,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 4,
                "card_class": "NEUTRAL",
                "cost": 7,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 4,
                "mechanics": "",
                "name": "Nomi, Kitchen Nightmare",
                "race": "",
                "rarity": "",
                "tavern_tier": 5,
                "type": "MINION"
            },
            {
                "attack": 10,
                "card_class": "NEUTRAL",
                "cost": 5,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 10,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Hangry Dragon (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "b1_turn": 0,
        "b2": [
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 4,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 9,
                "card_class": "ROGUE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 6,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Salty Looter",
                "race": "PIRATE",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Lightfang Enforcer",
                "race": "",
                "rarity": "EPIC",
                "tavern_tier": 5,
                "type": "MINION"
            }
        ],
        "defender_pos": 3
    },
    {
        "attacker_pos": 1,
        "b1": [
            {
                "attack": 18,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 4,
                "card_class": "NEUTRAL",
                "cost": 7,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 4,
                "mechanics": "",
                "name": "Nomi, Kitchen Nightmare",
                "race": "",
                "rarity": "",
                "tavern_tier": 5,
                "type": "MINION"
            },
            {
                "attack": 10,
                "card_class": "NEUTRAL",
                "cost": 5,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 4,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Hangry Dragon (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "b1_turn": 1,
        "b2": [
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 1,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 4,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 9,
                "card_class": "ROGUE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 6,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Salty Looter",
                "race": "PIRATE",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Lightfang Enforcer",
                "race": "",
                "rarity": "EPIC",
                "tavern_tier": 5,
                "type": "MINION"
            }
        ],
        "defender_pos": 0
    },
    {
        "attacker_pos": 1,
        "b1": [
            {
                "attack": 18,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 4,
                "card_class": "NEUTRAL",
                "cost": 7,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 4,
                "mechanics": "",
                "name": "Nomi, Kitchen Nightmare",
                "race": "",
                "rarity": "",
                "tavern_tier": 5,
                "type": "MINION"
            },
            {
                "attack": 10,
                "card_class": "NEUTRAL",
                "cost": 5,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 4,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Hangry Dragon (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "b1_turn": 0,
        "b2": [
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 1,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 4,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 9,
                "card_class": "ROGUE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 6,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Salty Looter",
                "race": "PIRATE",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Lightfang Enforcer",
                "race": "",
                "rarity": "EPIC",
                "tavern_tier": 5,
                "type": "MINION"
            }
        ],
        "defender_pos": 2
    },
    {
        "attacker_pos": 2,
        "b1": [
            {
                "attack": 18,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 10,
                "card_class": "NEUTRAL",
                "cost": 5,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 4,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Hangry Dragon (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "b1_turn": 1,
        "b2": [
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 1,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 4,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 9,
                "card_class": "ROGUE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 6,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Salty Looter",
                "race": "PIRATE",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Lightfang Enforcer",
                "race": "",
                "rarity": "EPIC",
                "tavern_tier": 5,
                "type": "MINION"
            }
        ],
        "defender_pos": 0
    },
    {
        "attacker_pos": 1,
        "b1": [
            {
                "attack": 18,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            },
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 5,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 10,
                "card_class": "NEUTRAL",
                "cost": 5,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Hangry Dragon (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "b1_turn": 0,
        "b2": [
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 4,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 9,
                "card_class": "ROGUE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 6,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Salty Looter",
                "race": "PIRATE",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Lightfang Enforcer",
                "race": "",
                "rarity": "EPIC",
                "tavern_tier": 5,
                "type": "MINION"
            }
        ],
        "defender_pos": 1
    },
    {
        "attacker_pos": 1,
        "b1": [
            {
                "attack": 18,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 1,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 10,
                "card_class": "NEUTRAL",
                "cost": 5,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Hangry Dragon (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "b1_turn": 1,
        "b2": [
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 4,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Lightfang Enforcer",
                "race": "",
                "rarity": "EPIC",
                "tavern_tier": 5,
                "type": "MINION"
            }
        ],
        "defender_pos": 0
    },
    {
        "attacker_pos": 1,
        "b1": [
            {
                "attack": 18,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 3,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 1,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 10,
                "card_class": "NEUTRAL",
                "cost": 5,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Hangry Dragon (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "b1_turn": 0,
        "b2": [
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 6,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Lightfang Enforcer",
                "race": "",
                "rarity": "EPIC",
                "tavern_tier": 5,
                "type": "MINION"
            }
        ],
        "defender_pos": 0
    },
    {
        "attacker_pos": 2,
        "b1": [
            {
                "attack": 18,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 1,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 1,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            },
            {
                "attack": 10,
                "card_class": "NEUTRAL",
                "cost": 5,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['TRIGGER_VISUAL']",
                "name": "Hangry Dragon (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "b1_turn": 1,
        "b2": [
            {
                "attack": 6,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 2,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "defender_pos": 0
    },
    {
        "attacker_pos": 0,
        "b1": [
            {
                "attack": 18,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": true,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 1,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 1,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "b1_turn": 0,
        "b2": [
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": true,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 1,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "defender_pos": 0
    },
    {
        "attacker_pos": 1,
        "b1": [
            {
                "attack": 4,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 1,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            },
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 1,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "b1_turn": 1,
        "b2": [
            {
                "attack": 2,
                "card_class": "NEUTRAL",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": false,
                "has_windfury": false,
                "health": 1,
                "mechanics": "['DIVINE_SHIELD', 'REBORN']",
                "name": "Bronze Warden",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 3,
                "type": "MINION"
            }
        ],
        "defender_pos": 0
    },
    {
        "attacker_pos": 1096110921,
        "b1": [
            {
                "attack": 4,
                "card_class": "MAGE",
                "cost": 4,
                "has_deathrattle": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_triggered_effect": true,
                "has_windfury": false,
                "health": 1,
                "mechanics": "",
                "name": "Glyph Guardian (Golden)",
                "race": "DRAGON",
                "rarity": "",
                "tavern_tier": 2,
                "type": "MINION"
            }
        ],
        "b1_turn": 0,
        "defender_pos": 1562837059
    }
];
  return json;
}

function get_board_picker_card(card_name) {
    var img = new Image();
    let asset_folder = process.env.PUBLIC_URL + "/assets/";
    let sunwell = new Sunwell({
        assetFolder: asset_folder,
      	titleFont: "belwe_fsextrabold",
      	bodyFontRegular: "franklin_gothic_fsMdCn",
      	bodyFontItalic: "franklin_gothic_fsMdCnIt",
      	bodyFontBold: "franklin_gothic_fsDemiCn",
      	bodyFontBoldItalic: "franklin_gothic_fsDemiCn",
      	bodyFontSize: 38,
      	bodyLineHeight: 40,
      	bodyFontOffset: {x: 0, y: 26},
      	debug: true
    });

    // var card_name_raw = card_json['name'];
    // var card_name = card_name_raw.replace(" (Golden)", "");

    // var is_golden = card_name_raw !== card_name;
    var is_golden = false;

    sunwell.createCard({
      	"name": card_name,
      	// "attack": card_json['attack'],
      	// "cardClass": "MURLOC",
        // "cardFrame": null,
      	//"collectible": true,
      	"cost": 1,
      	// "elite": true,
      	// "faction": "ALLIANCE",
      	// "health": card_json['health'],
        // "deathrattle": card_json['has_deathrattle'],
        // "divineShield": card_json['has_divine_shield'],
        // "hasTriggeredEffect": card_json['has_triggered_effect'],
      	// "mechanics": [
      	// 	"BATTLECRY",
      	// 	"CHARGE"
      	// ],
        // "poisonous": card_json['has_poison'],
      	// "rarity": "COMMON",
      	// "set": "EXPERT1",
      	"type": "MINION_FOR_BOARD_PICKER",
        // "reborn": card_json['has_reborn'],
        // "silenced": true,
      	// "texture": "../assets/" + card_name + ".jpg"
        // "texture": process.env.PUBLIC_URL + "/assets/Mecharoo.png"
        "texture": asset_folder + card_name.replace("'", "") + ".jpg"
    }, 256, is_golden, img, function() {
      	console.log('done');
    });

    return img;
}

function get_card(card_json) {    
    var img = new Image();
    let asset_folder = process.env.PUBLIC_URL + "/assets/";
    let sunwell = new Sunwell({
        assetFolder: asset_folder,
      	titleFont: "belwe_fsextrabold",
      	bodyFontRegular: "franklin_gothic_fsMdCn",
      	bodyFontItalic: "franklin_gothic_fsMdCnIt",
      	bodyFontBold: "franklin_gothic_fsDemiCn",
      	bodyFontBoldItalic: "franklin_gothic_fsDemiCn",
      	bodyFontSize: 38,
      	bodyLineHeight: 40,
      	bodyFontOffset: {x: 0, y: 26},
      	debug: true
    });

    var card_name_raw = card_json['name'];
    var card_name = card_name_raw.replace(" (Golden)", "");

    var is_golden = card_name_raw !== card_name;

    sunwell.createCard({
      	//"id": "EX1_116",
      	//"dbfId": 559,
      	"name": card_name,
      	// "text": "<b>Charge</b>. <b>Battlecry:</b> Summon two 1/1 Whelps for your opponent.",
      	// "flavor": "At least he has Angry Chicken.",
      	// "artist": "Gabe from Penny Arcade",
      	"attack": card_json['attack'],
      	"cardClass": "MURLOC",
        "cardFrame": null,
      	//"collectible": true,
      	"cost": 1,
      	// "elite": true,
      	"faction": "ALLIANCE",
      	"health": card_json['health'],
        "deathrattle": card_json['has_deathrattle'],
        "divineShield": card_json['has_divine_shield'],
        "hasTriggeredEffect": card_json['has_triggered_effect'],
      	// "mechanics": [
      	// 	"BATTLECRY",
      	// 	"CHARGE"
      	// ],
        "poisonous": card_json['has_poison'],
      	"rarity": "COMMON",
      	// "set": "EXPERT1",
      	"type": "MINION_ON_BOARD",
        "reborn": card_json['has_reborn'],
        // "silenced": true,
      	// "texture": "../assets/" + card_name + ".jpg"
        // "texture": process.env.PUBLIC_URL + "/assets/Mecharoo.png"
        "texture": asset_folder + card_name.replace("'", "") + ".jpg"
    }, 256, is_golden, img, function() {
      	console.log('done');
    });

    return img;
}

class TavernTier extends React.Component {
  constructor(props){
    super(props);
    this.state = {
      scale: { x: .25, y: .25}
    }    
  }

  // makeBig = () => {
  //     if (this.state.scale.x === .25) {
  //         this.setState({scale: {x: this.state.scale.x * 1.25, y: this.state.scale.y * 1.25}});
  //     }
  // }

  // makeSmall = () => {
  //     this.setState({scale: {x: .25, y: .25}});
  // }
  

  render(){

    const DrawTavTier = () => {

      return (
              <Sprite                
                x={this.props.xStart}
                y={this.props.yStart}
                anchor={[0.25, 0.25]}
                interactive={true}
                scale={this.state.scale}
                image={process.env.PUBLIC_URL + "/assets/tier-" + this.props.tier + ".png"}
                pointerdown={() => {
                  console.log("click...button pushed...?");
                  this.props.buttonPushed(this.props.tier);                  
                }}
             />
      )
    }

    return (
      <DrawTavTier />
        )
    }
}

class BoardPickerCard extends React.Component {
  constructor(props){
    super(props);
    this.state = {
      scale: { x: .25, y: .2}
    }
  }

  render(){

    const DrawBoardPickerCard = () => {

      return (
              <Sprite                
                x={this.props.xStart}
                y={this.props.yStart}
                anchor={[0.25, 0.25]}
                interactive={true}
                scale={this.state.scale}
                image={get_board_picker_card(this.props.name)}
                pointerdown={() => {
                  console.log("click");
                  if (this.state.scale.x === .25) {
                      this.setState({scale: {x: this.state.scale.x * 1.25, y: this.state.scale.y * 1.25}});
                  }
                  else {
                      this.setState({scale: {x: .25, y: .25}});
                  }
                }}
             />
      )
    }

    return (
      <DrawBoardPickerCard />
        )
    }
}


class BoardBuilder extends React.Component {
  constructor(props){
    super(props);
    this.state = {
      tierToDisplay: 1
    }
    this.createTavernTiers = this.createTavernTiers.bind(this);
    this.createCards = this.createCards.bind(this);
    this.tierButtonPushed = this.tierButtonPushed.bind(this);
  }

  tierButtonPushed(tier) { this.setState({tierToDisplay: tier}); console.log("POOSHED"); }

  createTavernTiers(start) {
      let spacing = 30;
      let res = [
        <TavernTier tier={1} xStart={start + 0 * spacing} yStart={50} buttonPushed={this.tierButtonPushed} />,
        <TavernTier tier={2} xStart={start + 1 * spacing} yStart={50} buttonPushed={this.tierButtonPushed} />,
        <TavernTier tier={3} xStart={start + 2 * spacing} yStart={50} buttonPushed={this.tierButtonPushed} />,
        <TavernTier tier={4} xStart={start + 3 * spacing} yStart={50} buttonPushed={this.tierButtonPushed} />,
        <TavernTier tier={5} xStart={start + 4 * spacing} yStart={50} buttonPushed={this.tierButtonPushed} />,
        <TavernTier tier={6} xStart={start + 5 * spacing} yStart={50} buttonPushed={this.tierButtonPushed} />,
      ]
      return res;
  }

  createCards(start) {
    let spacing = 40;
    let all_card_json = getBgCardJson();
    const card_json = all_card_json.filter(card => card["tier"] == this.state.tierToDisplay)
    let cards_per_row = 5;
    var res = []
    for (var i = 0; i < card_json.length; i++) {
        let card = card_json[i];                
        res.push( <BoardPickerCard
                  name={card["name"]}
                  type={card["race"]}
                  tier={card["tier"]}
                  xStart={start + spacing * (i % cards_per_row) - 20}
                  yStart={100 + spacing * Math.floor(i / cards_per_row)} /> );
    }
    return res;
  }

  render(){
    let start = BOARD_WIDTH + 20;
    let tav_tiers = this.createTavernTiers(start);
    let cards = this.createCards(start);
    let res = tav_tiers.concat(cards);
    return (
      res
        )
  }
  
}

class Card extends React.Component {

  // Would be nice if there was an easy way to define required props
  // Looks like it's sort of possible w/ PropTypes but seems like too much for now
  constructor(props) {
    super(props);
  }

  render() {
    const reducer = (_, { data }) => data;  

    const DrawCard = () => {
      const [motion, update] = useReducer(reducer);
      const iter = useRef(0);
      useTick(delta => {
        const i = (iter.current += 0.05 * delta);
        const num_ticks = 20;
        if (i > num_ticks) {
          // I have the feeling this is wasteful
          // TODO: Figure out how to stop calling the useTick hook
          //       after animation ends
          return;
        }
        if (this.props.toDraw) {
          var slopeX = (this.props.endX - this.props.startX) / num_ticks;
          var slopeY = (this.props.endY - this.props.startY) / num_ticks;
          update({
            type: 'update',
            data: {
              x: this.props.startX + slopeX * i,
              y: this.props.startY + slopeY * i,
            },
          })
        }
        else {
          update({
            type: 'update',
            data: {
              x: this.props.startX,
              y: this.props.startY,
            },
          })
        }
      })

      return (
              <Sprite
                image={this.props.img}
                height={225} // 70
                width={150} // 46
                {...motion}
             />
      )
    }
  return (
    <DrawCard />
    )
  }
}

class Rollout extends React.Component {

  constructor(props) {
    super(props);
    this.state = {
      toDraw: false,
      backgroundImage: process.env.PUBLIC_URL + "/assets/hearthstone_battle_phase_background.jpeg",
      frames: getTestCardFrames(), // TODO: Get this dynamically
      frameNum: 0,
    };

    // Are you serious, react...?
    this.toggleAnimation = this.toggleAnimation.bind(this);
    this.createCards = this.createCards.bind(this);
    this.createBoard = this.createBoard.bind(this);
    this.getAbsoluteXandY = this.getAbsoluteXandY.bind(this);
    this.getEndXAndEndY = this.getEndXAndEndY.bind(this);
    this.getNextFrame = this.getNextFrame.bind(this);
    this.getPreviousFrame = this.getPreviousFrame.bind(this);
    this.clearBoard = this.clearBoard.bind(this);  
  }

  isBoardEmpty = () => { return this.state.frameNum === -1; }

  toggleAnimation() {
    this.setState({toDraw: !this.state.toDraw});
  }

  getNextFrame() {
    this.setState({frameNum: Math.min(this.state.frameNum + 1, this.state.frames.length - 1)});
  }

  getPreviousFrame() {
    if (this.isBoardEmpty()) {
        this.setState({frameNum: Math.max(this.state.frameNum - 1, 0)});
    }
  }

  clearBoard() {
    this.setState({frameNum: -1, frames: []});
  }

  createBoard(frame, top_or_bot) {
    var board_json = top_or_bot == "TOP" ? frame["b2"] : frame["b1"];
    if (board_json == null) return [];
    var card_arr = [];
    for (var j = 0; j < board_json.length; j++) {
      var card1_json = board_json[j];
      const [startX, startY] = this.getAbsoluteXandY(board_json, j, top_or_bot);
      const end_x_and_y = this.getEndXAndEndY(frame, top_or_bot, j);
      if (end_x_and_y === null) {
        var [endX, endY] = [startX, startY];
      }
      else {
        var [endX, endY] = end_x_and_y;
      }
      // debugger;
      var card1 = <Card key={"card1"}
                  toDraw={this.state.toDraw}
                  img={get_card(card1_json)}
                  startX={startX}
                  startY={startY}
                  endX={endX}
                  endY={endY}
                >
                </Card>;
      card_arr.push(card1);
    }
    return card_arr;
  }

  getEndXAndEndY(frame, top_or_bot, card_pos) {
    // If it's not the attacker, no animation occurs (for now),
    // so return original position and get out
    if (frame['attacker_pos'] !== card_pos) return null;
    if (top_or_bot === "TOP" && frame["b1_turn"]) return null;
    if (top_or_bot === "BOT" && !frame["b1_turn"]) return null;

    const def_board_str = frame["b1_turn"] ? "b2" : "b1";
    var defender_board = frame[def_board_str];
    const defender_top_or_bot = top_or_bot == "BOT" ? "TOP" : "BOT";
    const [endX, endY] = this.getAbsoluteXandY(defender_board, frame['defender_pos'], defender_top_or_bot);
    // debugger;
    return [endX, endY];
  }

  getAbsoluteXandY(board_json, pos, top_or_bot) {
    const HEIGHT_OFFSET = top_or_bot == "TOP" ? 1 : 2;
    const BOARD_HEIGHT_FUDGE = top_or_bot == "TOP" ? TOP_BOARD_HEIGHT_FUDGE : BOTTOM_BOARD_HEIGHT_FUDGE;
    var width_coord_start = -Math.floor(board_json.length / 2) * CARD_WIDTH_DELTA + CENTERING_FUDGE_FACTOR;
    // Adjust start for even/odd # of cards
    const WIDTH_COORD_START = (board_json.length % 2 === 0) ? (width_coord_start + CARD_WIDTH_DELTA / 2) : (width_coord_start);
    const width_coord_offset = WIDTH_COORD_START + CARD_WIDTH_DELTA * pos + BOARD_WIDTH_FUDGE;
    const startX = BOARD_WIDTH / 2.0 + width_coord_offset;
    const startY = HEIGHT_OFFSET * BOARD_HEIGHT / 3.0 + BOARD_HEIGHT_FUDGE;
    return [startX, startY];
  }

  createCards(frame) {
    // debugger;    
    var top_board = this.createBoard(frame, "TOP");    
    var bot_board = this.createBoard(frame, "BOT");
    var card_arr = top_board.concat(bot_board);
    return card_arr;
  }

  render() {
    // debugger;
    var frame;
    if (this.state.frameNum >= 0) {
        frame = this.state.frames[this.state.frameNum];
    }
    else {
        frame = [];
    }
    // const frame = getTestCardFrames()[0];
    const card_arr = this.createCards(frame);

      console.log("Rendering...");
    
    return (
      <>
      <button onClick={this.toggleAnimation} disabled={this.isBoardEmpty()}>
          Animate Frame
      </button>
      <button onClick={this.getPreviousFrame} disabled={this.isBoardEmpty()}>
          Previous Frame
      </button>
      <button onClick={this.getNextFrame} disabled={this.isBoardEmpty()}>
          Next Frame
      </button>
      <button onClick={this.clearBoard}>
          Clear Board
      </button>
      <Stage width={BOARD_WIDTH + PICKER_WIDTH} height={BOARD_HEIGHT + PICKER_HEIGHT} options={{ transparent: true }}>
        <Container x={0} y={0}>
            <Sprite
                image={this.state.backgroundImage}
                width={BOARD_WIDTH}
                height={BOARD_HEIGHT}
             />
            {card_arr}
            <BoardBuilder></BoardBuilder>
        </Container>
      </Stage>
      </>
    )
  }
}

export default Rollout;
