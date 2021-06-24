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
        "race": "NEUTRAL",
        "attack": 2,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": true,
        "has_taunt": true,
        "has_windfury": false
    },
    {
        "name": "Alleycat",
        "tier": 1,
        "race": "BEAST",
        "attack": 1,
        "health": 1,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Amalgam",
        "tier": 1,
        "race": "ALL",
        "attack": 0,
        "health": 1,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Deck Swabbie",
        "tier": 1,
        "race": "PIRATE",
        "attack": 2,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Dragonspawn Lieutenant",
        "tier": 1,
        "race": "DRAGON",
        "attack": 2,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": true,
        "has_windfury": false
    },
    {
        "name": "Fiendish Servant",
        "tier": 1,
        "race": "DEMON",
        "attack": 2,
        "health": 1,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Fish of N'Zoth",
        "tier": 1,
        "race": "BEAST",
        "attack": 0,
        "health": 0,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Micro Machine",
        "tier": 1,
        "race": "MECHANICAL",
        "attack": 1,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Micro Mummy",
        "tier": 1,
        "race": "MECHANICAL",
        "attack": 1,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": true,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Murloc Scout",
        "tier": 1,
        "race": "MURLOC",
        "attack": 1,
        "health": 1,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Murloc Tidecaller",
        "tier": 1,
        "race": "MURLOC",
        "attack": 1,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Murloc Tidehunter",
        "tier": 1,
        "race": "MURLOC",
        "attack": 2,
        "health": 1,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Razorfen Geomancer",
        "tier": 1,
        "race": "QUILBOAR",
        "attack": 3,
        "health": 1,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Red Whelp",
        "tier": 1,
        "race": "DRAGON",
        "attack": 1,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Refreshing Anomaly",
        "tier": 1,
        "race": "ELEMENTAL",
        "attack": 1,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Rockpool Hunter",
        "tier": 1,
        "race": "MURLOC",
        "attack": 2,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Scallywag",
        "tier": 1,
        "race": "PIRATE",
        "attack": 2,
        "health": 1,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Scavenging Hyena",
        "tier": 1,
        "race": "BEAST",
        "attack": 2,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Sellemental",
        "tier": 1,
        "race": "ELEMENTAL",
        "attack": 2,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Sun-Bacon Relaxer",
        "tier": 1,
        "race": "QUILBOAR",
        "attack": 1,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Vulgar Homunculus",
        "tier": 1,
        "race": "DEMON",
        "attack": 2,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": true,
        "has_windfury": false
    },
    {
        "name": "Wrath Weaver",
        "tier": 1,
        "race": "NEUTRAL",
        "attack": 1,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Freedealing Gambler",
        "tier": 2,
        "race": "PIRATE",
        "attack": 3,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Glyph Guardian",
        "tier": 2,
        "race": "DRAGON",
        "attack": 2,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Harvest Golem",
        "tier": 2,
        "race": "MECHANICAL",
        "attack": 2,
        "health": 3,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Imprisoner",
        "tier": 2,
        "race": "DEMON",
        "attack": 3,
        "health": 3,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": true,
        "has_windfury": false
    },
    {
        "name": "Kaboom Bot",
        "tier": 2,
        "race": "MECHANICAL",
        "attack": 2,
        "health": 2,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Menagerie Mug",
        "tier": 2,
        "race": "NEUTRAL",
        "attack": 2,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Metaltooth Leaper",
        "tier": 2,
        "race": "MECHANICAL",
        "attack": 3,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Molten Rock",
        "tier": 2,
        "race": "ELEMENTAL",
        "attack": 2,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": true,
        "has_windfury": false
    },
    {
        "name": "Murloc Warleader",
        "tier": 2,
        "race": "MURLOC",
        "attack": 3,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Nathrezim Overseer",
        "tier": 2,
        "race": "DEMON",
        "attack": 2,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Old Murk-Eye",
        "tier": 2,
        "race": "MURLOC",
        "attack": 2,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Pack Leader",
        "tier": 2,
        "race": "NEUTRAL",
        "attack": 3,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Party Elemental",
        "tier": 2,
        "race": "ELEMENTAL",
        "attack": 3,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Prophet of the Boar",
        "tier": 2,
        "race": "NEUTRAL",
        "attack": 2,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Rabid Saurolisk",
        "tier": 2,
        "race": "BEAST",
        "attack": 3,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Roadboar",
        "tier": 2,
        "race": "QUILBOAR",
        "attack": 1,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Selfless Hero",
        "tier": 2,
        "race": "NEUTRAL",
        "attack": 2,
        "health": 1,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Southsea Captain",
        "tier": 2,
        "race": "PIRATE",
        "attack": 3,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Spawn of N'Zoth",
        "tier": 2,
        "race": "NEUTRAL",
        "attack": 2,
        "health": 2,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Steward of Time",
        "tier": 2,
        "race": "DRAGON",
        "attack": 3,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Tormented Ritualist",
        "tier": 2,
        "race": "NEUTRAL",
        "attack": 2,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": true,
        "has_windfury": false
    },
    {
        "name": "Tough Tusk",
        "tier": 2,
        "race": "QUILBOAR",
        "attack": 4,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Unstable Ghoul",
        "tier": 2,
        "race": "NEUTRAL",
        "attack": 1,
        "health": 3,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": true,
        "has_windfury": false
    },
    {
        "name": "Waxrider Togwaggle",
        "tier": 2,
        "race": "NEUTRAL",
        "attack": 1,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Yo-Ho-Ogre",
        "tier": 2,
        "race": "PIRATE",
        "attack": 2,
        "health": 6,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Arcane Assistant",
        "tier": 3,
        "race": "ELEMENTAL",
        "attack": 3,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Arm of the Empire",
        "tier": 3,
        "race": "NEUTRAL",
        "attack": 4,
        "health": 5,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Bannerboar",
        "tier": 3,
        "race": "QUILBOAR",
        "attack": 1,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Bloodsail Cannoneer",
        "tier": 3,
        "race": "PIRATE",
        "attack": 4,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Bristleback Brute",
        "tier": 3,
        "race": "QUILBOAR",
        "attack": 3,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Bronze Warden",
        "tier": 3,
        "race": "DRAGON",
        "attack": 2,
        "health": 1,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": true,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Coldlight Seer",
        "tier": 3,
        "race": "MURLOC",
        "attack": 2,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Crackling Cyclone",
        "tier": 3,
        "race": "ELEMENTAL",
        "attack": 4,
        "health": 1,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": true
    },
    {
        "name": "Crystalweaver",
        "tier": 3,
        "race": "NEUTRAL",
        "attack": 5,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Deflect-o-Bot",
        "tier": 3,
        "race": "MECHANICAL",
        "attack": 3,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Felfin Navigator",
        "tier": 3,
        "race": "MURLOC",
        "attack": 4,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Hangry Dragon",
        "tier": 3,
        "race": "DRAGON",
        "attack": 4,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Houndmaster",
        "tier": 3,
        "race": "NEUTRAL",
        "attack": 4,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Imp Gang Boss",
        "tier": 3,
        "race": "DEMON",
        "attack": 2,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Infested Wolf",
        "tier": 3,
        "race": "BEAST",
        "attack": 3,
        "health": 3,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Iron Sensei",
        "tier": 3,
        "race": "MECHANICAL",
        "attack": 2,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Khadgar",
        "tier": 3,
        "race": "NEUTRAL",
        "attack": 2,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Monstrous Macaw",
        "tier": 3,
        "race": "BEAST",
        "attack": 5,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Necrolyte",
        "tier": 3,
        "race": "QUILBOAR",
        "attack": 3,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Rat Pack",
        "tier": 3,
        "race": "BEAST",
        "attack": 2,
        "health": 2,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Replicating Menace",
        "tier": 3,
        "race": "MECHANICAL",
        "attack": 3,
        "health": 1,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Salty Looter",
        "tier": 3,
        "race": "PIRATE",
        "attack": 4,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Screwjank Clunker",
        "tier": 3,
        "race": "MECHANICAL",
        "attack": 2,
        "health": 5,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Soul Devourer",
        "tier": 3,
        "race": "DEMON",
        "attack": 3,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Soul Juggler",
        "tier": 3,
        "race": "NEUTRAL",
        "attack": 3,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Southsea Strongarm",
        "tier": 3,
        "race": "PIRATE",
        "attack": 4,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Stasis Elemental",
        "tier": 3,
        "race": "ELEMENTAL",
        "attack": 4,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Thorncaller",
        "tier": 3,
        "race": "QUILBOAR",
        "attack": 4,
        "health": 3,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Twilight Emissary",
        "tier": 3,
        "race": "DRAGON",
        "attack": 4,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": true,
        "has_windfury": false
    },
    {
        "name": "Warden of Old",
        "tier": 3,
        "race": "NEUTRAL",
        "attack": 3,
        "health": 3,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Annoy-o-Module",
        "tier": 4,
        "race": "MECHANICAL",
        "attack": 2,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": true,
        "has_windfury": false
    },
    {
        "name": "Bigfernal",
        "tier": 4,
        "race": "DEMON",
        "attack": 4,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Bolvar, Fireblood",
        "tier": 4,
        "race": "NEUTRAL",
        "attack": 1,
        "health": 7,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Bonker",
        "tier": 4,
        "race": "QUILBOAR",
        "attack": 3,
        "health": 7,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": true
    },
    {
        "name": "Cave Hydra",
        "tier": 4,
        "race": "BEAST",
        "attack": 2,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Champion of Y'Shaarj",
        "tier": 4,
        "race": "NEUTRAL",
        "attack": 4,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Cobalt Scalebane",
        "tier": 4,
        "race": "DRAGON",
        "attack": 5,
        "health": 5,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Defender of Argus",
        "tier": 4,
        "race": "NEUTRAL",
        "attack": 3,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Drakonid Enforcer",
        "tier": 4,
        "race": "DRAGON",
        "attack": 3,
        "health": 6,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Dynamic Duo",
        "tier": 4,
        "race": "QUILBOAR",
        "attack": 3,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": true,
        "has_windfury": false
    },
    {
        "name": "Goldgrubber",
        "tier": 4,
        "race": "PIRATE",
        "attack": 2,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Groundshaker",
        "tier": 4,
        "race": "QUILBOAR",
        "attack": 2,
        "health": 6,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Herald of Flame",
        "tier": 4,
        "race": "DRAGON",
        "attack": 6,
        "health": 6,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Hexruin Marauder",
        "tier": 4,
        "race": "DEMON",
        "attack": 3,
        "health": 5,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Junkbot",
        "tier": 4,
        "race": "MECHANICAL",
        "attack": 1,
        "health": 5,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Majordomo Executus",
        "tier": 4,
        "race": "NEUTRAL",
        "attack": 6,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Mechano-Egg",
        "tier": 4,
        "race": "MECHANICAL",
        "attack": 0,
        "health": 5,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Menagerie Jug",
        "tier": 4,
        "race": "NEUTRAL",
        "attack": 3,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Primalfin",
        "tier": 4,
        "race": "MURLOC",
        "attack": 1,
        "health": 1,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Primalfin Lookout",
        "tier": 4,
        "race": "MURLOC",
        "attack": 3,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Qiraji Harbinger",
        "tier": 4,
        "race": "NEUTRAL",
        "attack": 5,
        "health": 5,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Ring Matron",
        "tier": 4,
        "race": "DEMON",
        "attack": 6,
        "health": 4,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": true,
        "has_windfury": false
    },
    {
        "name": "Ripsnarl Captain",
        "tier": 4,
        "race": "PIRATE",
        "attack": 4,
        "health": 5,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Savannah Highmane",
        "tier": 4,
        "race": "BEAST",
        "attack": 6,
        "health": 5,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Security Rover",
        "tier": 4,
        "race": "MECHANICAL",
        "attack": 2,
        "health": 6,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Siegebreaker",
        "tier": 4,
        "race": "DEMON",
        "attack": 5,
        "health": 8,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": true,
        "has_windfury": false
    },
    {
        "name": "Toxfin",
        "tier": 4,
        "race": "MURLOC",
        "attack": 1,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Virmen Sensei",
        "tier": 4,
        "race": "NEUTRAL",
        "attack": 4,
        "health": 5,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Wildfire Elemental",
        "tier": 4,
        "race": "ELEMENTAL",
        "attack": 7,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Agamaggan, the Great Boar",
        "tier": 5,
        "race": "BEAST",
        "attack": 6,
        "health": 6,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Aggem Thorncurse",
        "tier": 5,
        "race": "QUILBOAR",
        "attack": 3,
        "health": 6,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Annihilan Battlemaster",
        "tier": 5,
        "race": "DEMON",
        "attack": 3,
        "health": 1,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Baron Rivendare",
        "tier": 5,
        "race": "NEUTRAL",
        "attack": 1,
        "health": 7,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Brann Bronzebeard",
        "tier": 5,
        "race": "NEUTRAL",
        "attack": 2,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Bristleback Knight",
        "tier": 5,
        "race": "QUILBOAR",
        "attack": 4,
        "health": 8,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": true
    },
    {
        "name": "Cap'n Hoggarr",
        "tier": 5,
        "race": "PIRATE",
        "attack": 6,
        "health": 6,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Deadly Spore",
        "tier": 5,
        "race": "NEUTRAL",
        "attack": 1,
        "health": 1,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": true,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Faceless Taverngoer",
        "tier": 5,
        "race": "NEUTRAL",
        "attack": 4,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Ironhide Direhorn",
        "tier": 5,
        "race": "BEAST",
        "attack": 7,
        "health": 7,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Kangor's Apprentice",
        "tier": 5,
        "race": "NEUTRAL",
        "attack": 3,
        "health": 6,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "King Bagurgle",
        "tier": 5,
        "race": "MURLOC",
        "attack": 6,
        "health": 3,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Lightfang Enforcer",
        "tier": 5,
        "race": "NEUTRAL",
        "attack": 2,
        "health": 2,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Mal'Ganis",
        "tier": 5,
        "race": "DEMON",
        "attack": 9,
        "health": 7,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Mama Bear",
        "tier": 5,
        "race": "BEAST",
        "attack": 4,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Murozond",
        "tier": 5,
        "race": "DRAGON",
        "attack": 5,
        "health": 5,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Mythrax the Unraveler",
        "tier": 5,
        "race": "NEUTRAL",
        "attack": 4,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Nat Pagle, Extreme Angler",
        "tier": 5,
        "race": "PIRATE",
        "attack": 8,
        "health": 5,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Nomi, Kitchen Nightmare",
        "tier": 5,
        "race": "NEUTRAL",
        "attack": 4,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Razorgore, the Untamed",
        "tier": 5,
        "race": "DRAGON",
        "attack": 2,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Seabreaker Goliath",
        "tier": 5,
        "race": "PIRATE",
        "attack": 6,
        "health": 7,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Sneed's Old Shredder",
        "tier": 5,
        "race": "MECHANICAL",
        "attack": 5,
        "health": 7,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Strongshell Scavenger",
        "tier": 5,
        "race": "NEUTRAL",
        "attack": 2,
        "health": 3,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Tavern Tempest",
        "tier": 5,
        "race": "ELEMENTAL",
        "attack": 4,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Voidlord",
        "tier": 5,
        "race": "DEMON",
        "attack": 3,
        "health": 9,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": true,
        "has_windfury": false
    },
    {
        "name": "Amalgadon",
        "tier": 6,
        "race": "ALL",
        "attack": 6,
        "health": 6,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Captain Flat Tusk",
        "tier": 6,
        "race": "QUILBOAR",
        "attack": 9,
        "health": 6,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Charlga",
        "tier": 6,
        "race": "QUILBOAR",
        "attack": 4,
        "health": 4,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Dread Admiral Eliza",
        "tier": 6,
        "race": "PIRATE",
        "attack": 6,
        "health": 7,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Foe Reaper 4000",
        "tier": 6,
        "race": "MECHANICAL",
        "attack": 6,
        "health": 9,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Gentle Djinni",
        "tier": 6,
        "race": "ELEMENTAL",
        "attack": 4,
        "health": 5,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": true,
        "has_windfury": false
    },
    {
        "name": "Ghastcoiler",
        "tier": 6,
        "race": "BEAST",
        "attack": 7,
        "health": 7,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Goldrinn, the Great Wolf",
        "tier": 6,
        "race": "BEAST",
        "attack": 4,
        "health": 4,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Imp Mama",
        "tier": 6,
        "race": "DEMON",
        "attack": 6,
        "health": 10,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Kalecgos, Arcane Aspect",
        "tier": 6,
        "race": "DRAGON",
        "attack": 4,
        "health": 12,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Lieutenant Garr",
        "tier": 6,
        "race": "ELEMENTAL",
        "attack": 8,
        "health": 1,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": true,
        "has_windfury": false
    },
    {
        "name": "Lil' Rag",
        "tier": 6,
        "race": "ELEMENTAL",
        "attack": 6,
        "health": 6,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Maexxna",
        "tier": 6,
        "race": "BEAST",
        "attack": 2,
        "health": 8,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": true,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Nadina the Red",
        "tier": 6,
        "race": "NEUTRAL",
        "attack": 7,
        "health": 4,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "The Tide Razor",
        "tier": 6,
        "race": "NEUTRAL",
        "attack": 6,
        "health": 4,
        "has_deathrattle": true,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
    },
    {
        "name": "Zapp Slywick",
        "tier": 6,
        "race": "NEUTRAL",
        "attack": 7,
        "health": 10,
        "has_deathrattle": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": false,
        "has_taunt": false,
        "has_windfury": false
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
      	"cost": 1,
      	"type": "MINION_FOR_BOARD_PICKER",
        "texture": asset_folder + card_name.replace("'", "") + ".jpg"
    }, 256, is_golden, img, function() {
      	// console.log('done');
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
        "taunt": card_json['has_taunt'],
      	"type": "MINION_ON_BOARD",
        "reborn": card_json['has_reborn'],
        // "silenced": true,
      	// "texture": "../assets/" + card_name + ".jpg"
        // "texture": process.env.PUBLIC_URL + "/assets/Mecharoo.png"
        "texture": asset_folder + card_name.replace("'", "") + ".jpg"
    }, 256, is_golden, img, function() {
      	// console.log('done');
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
      scale: { x: .25, y: .2},
        card_img: get_board_picker_card(this.props.name),
    }
  }


  render(){

    const DrawBoardPickerCard = () => {
        // TODO: Calclulate these dynamically
        const CENTER_X = 500;
        const CENTER_Y = 316;
          
        const isDragging = React.useRef(false);
        const offset = React.useRef({ x: 0, y: 0 });
        const [position, setPosition] = React.useState({ x: this.props.xStart, y: this.props.yStart })
        // Looks like closure computed/serialized, can't access 'this' dynamically.
        // So, bind it up here...
        const cardDroppedOnBoardCallback = this.props.cardDroppedOnBoard;
        const card_name = this.props.name;
        
        function onStart(e) {
            isDragging.current = true;    
            offset.current = {
                x: e.data.global.x - position.x,
                y: e.data.global.y - position.y
            };
        }

        function onEnd(e) {
            isDragging.current = false;
            var which_board = "";
            if (position.y >= CENTER_Y) {
                which_board = "BOT";
            }
            else {
                which_board = "TOP";
            }
            cardDroppedOnBoardCallback(card_name, which_board);
        }

        function onMove(e) {
            if (isDragging.current) {
                setPosition({
                    x: e.data.global.x - offset.current.x,
                    y: e.data.global.y - offset.current.y,
                })
            }
        }

        return (
              <Sprite                
                // x={this.props.xStart}
                // y={this.props.yStart}
                position={position}
                anchor={[0.25, 0.25]}
                interactive={true}
                scale={this.state.scale}
                image={this.state.card_img}
                pointerdown={onStart}
                pointerup={onEnd}
                pointermove={onMove}                
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
    this.cardDroppedOnBoard = this.cardDroppedOnBoard.bind(this);
  }

  tierButtonPushed(tier) { this.setState({tierToDisplay: tier}); }

  cardDroppedOnBoard(name, which_board) { this.props.updateBoardCallback(name, which_board); }

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
                  yStart={100 + spacing * Math.floor(i / cards_per_row)} 
                  cardDroppedOnBoard={this.cardDroppedOnBoard}
                  /> );
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

  updateBoardCallback = (name, which_board) => { 
      let all_cards = getBgCardJson();
      let card = all_cards.filter(c => c['name'] == name)[0];

      var b1_cards = [];
      if (!this.isBoardEmpty()) {
          b1_cards = this.state.frames[0]["b1"];
      }

      var b2_cards = [];
      if (!this.isBoardEmpty()) {
          b2_cards = this.state.frames[0]["b2"];
      }

      if (which_board == "BOT") {
          b1_cards.push(card);
      }
      else {
          b2_cards.push(card);
      }
      
      let new_frames = [
          {
        "attacker_pos": -1,
        "defender_pos": -1,
        "b1": b1_cards,
        "b2": b2_cards,            
          }
      ]
      this.setState({frames: new_frames});
      this.setState({frameNum: 0});
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
    var frame;
    if (this.state.frameNum >= 0) {
        frame = this.state.frames[this.state.frameNum];
    }
    else {
        frame = [];
    }
    const card_arr = this.createCards(frame);
    
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
            <BoardBuilder
                updateBoardCallback={this.updateBoardCallback}
            />
        </Container>
      </Stage>
      </>
    )
  }
}

export default Rollout;
