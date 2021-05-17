import React, { useReducer, useRef } from 'react';

import { Container, Stage, Sprite, useTick, } from '@inlet/react-pixi';
import Sunwell from "../sunwell/sunwell_full/Sunwell.ts"

import './Rollout.css';

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
      scale: { x: .5, y: .5}
    }
  }

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
                  console.log("click");
                  this.setState({scale: {x: this.state.scale.x * 1.25, y: this.state.scale.y * 1.25}});
                }}
             />
      )
    }

    return (
      <DrawTavTier />
        )
    }
}

class BoardBuilder extends React.Component {
  constructor(props){
    super(props);
  }

  render(){
    return (
      [
        <TavernTier tier={1} xStart={BOARD_WIDTH} yStart={50} />,
        <TavernTier tier={2} xStart={BOARD_WIDTH + 20} yStart={50} />
      ]
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
  }

  toggleAnimation() {
    this.setState({toDraw: !this.state.toDraw});
  }

  getNextFrame() {
    this.setState({frameNum: Math.min(this.state.frameNum + 1, this.state.frames.length - 1)});
  }

  getPreviousFrame() {
    this.setState({frameNum: Math.max(this.state.frameNum - 1, 0)});
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
    const frame = this.state.frames[this.state.frameNum];
    // const frame = getTestCardFrames()[0];
    const card_arr = this.createCards(frame);
    
    return (
      <>
      <button onClick={this.toggleAnimation}>
          Animate Frame
      </button>
      <button onClick={this.getPreviousFrame}>
          Previous Frame
      </button>
      <button onClick={this.getNextFrame}>
          Next Frame
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
