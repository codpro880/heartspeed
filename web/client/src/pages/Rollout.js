import React, { useReducer, useRef } from 'react';

import { Container, Stage, Sprite, useTick, } from '@inlet/react-pixi';
// import { Sunwell } from "../sunwell/MinifiedSunwell"
// import Sunwell from "../sunwell/MinifiedSunwell"
import Sunwell from "../sunwell/sunwell_full/Sunwell.ts"
 
function getTestCardJson(attack, health, name) {
    var json = {
        "attack": attack,
        "has_cleave": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": true,
        "has_taunt": false,
        "has_windfury": false,
        "health": health,
        "name": name
    }
    return json;
}

function getTestCardFrames() {
  var json = [
{
  "attacker_pos": 0,
  "b1": [
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 10,
          "name": "Glyph Guardian (Golden)"
      },
      {
          "attack": 13,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 9,
          "name": "Wildfire Elemental"
      },
      {
          "attack": 7,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 3,
          "name": "Wildfire Elemental"
      },
      {
          "attack": 4,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Lil Rag"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 5,
          "name": "Bronze Warden"
      },
      {
          "attack": 4,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Nomi, Kitchen Nightmare"
      },
      {
          "attack": 10,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 10,
          "name": "Hangry Dragon (Golden)"
      }
  ],
  "b1_turn": 1,
  "b2": [
      {
          "attack": 8,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 11,
          "name": "Rockpool Hunter (Golden)"
      },
      {
          "attack": 13,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 10,
          "name": "Salty Looter"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 5,
          "name": "Bronze Warden"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Bronze Warden"
      },
      {
          "attack": 9,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 6,
          "name": "Salty Looter"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 2,
          "name": "Lightfang Enforcer"
      },
      {
          "attack": 7,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": true,
          "has_windfury": false,
          "health": 9,
          "name": "Yo-Ho-Ogre"
      }
  ],
  "defender_pos": 6
},
{
  "attacker_pos": 0,
  "b1": [
      {
          "attack": 18,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 3,
          "name": "Glyph Guardian (Golden)"
      },
      {
          "attack": 13,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 9,
          "name": "Wildfire Elemental"
      },
      {
          "attack": 7,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 3,
          "name": "Wildfire Elemental"
      },
      {
          "attack": 4,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Lil Rag"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 5,
          "name": "Bronze Warden"
      },
      {
          "attack": 4,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Nomi, Kitchen Nightmare"
      },
      {
          "attack": 10,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 10,
          "name": "Hangry Dragon (Golden)"
      }
  ],
  "b1_turn": 0,
  "b2": [
      {
          "attack": 8,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 11,
          "name": "Rockpool Hunter (Golden)"
      },
      {
          "attack": 13,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 10,
          "name": "Salty Looter"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 5,
          "name": "Bronze Warden"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Bronze Warden"
      },
      {
          "attack": 9,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 6,
          "name": "Salty Looter"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 2,
          "name": "Lightfang Enforcer"
      }
  ],
  "defender_pos": 5
},
{
  "attacker_pos": 1,
  "b1": [
      {
          "attack": 18,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 3,
          "name": "Glyph Guardian (Golden)"
      },
      {
          "attack": 13,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 9,
          "name": "Wildfire Elemental"
      },
      {
          "attack": 7,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 3,
          "name": "Wildfire Elemental"
      },
      {
          "attack": 4,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Lil Rag"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 5,
          "name": "Bronze Warden"
      },
      {
          "attack": 10,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 10,
          "name": "Hangry Dragon (Golden)"
      }
  ],
  "b1_turn": 1,
  "b2": [
      {
          "attack": 8,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 7,
          "name": "Rockpool Hunter (Golden)"
      },
      {
          "attack": 13,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 10,
          "name": "Salty Looter"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 5,
          "name": "Bronze Warden"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Bronze Warden"
      },
      {
          "attack": 9,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 6,
          "name": "Salty Looter"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 2,
          "name": "Lightfang Enforcer"
      }
  ],
  "defender_pos": 4
},
{
  "attacker_pos": 1,
  "b1": [
      {
          "attack": 18,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 3,
          "name": "Glyph Guardian (Golden)"
      },
      {
          "attack": 7,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 3,
          "name": "Wildfire Elemental"
      },
      {
          "attack": 4,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Lil Rag"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 5,
          "name": "Bronze Warden"
      },
      {
          "attack": 10,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 10,
          "name": "Hangry Dragon (Golden)"
      }
  ],
  "b1_turn": 0,
  "b2": [
      {
          "attack": 8,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 7,
          "name": "Rockpool Hunter (Golden)"
      },
      {
          "attack": 13,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 10,
          "name": "Salty Looter"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 5,
          "name": "Bronze Warden"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Bronze Warden"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 2,
          "name": "Lightfang Enforcer"
      }
  ],
  "defender_pos": 4
},
{
  "attacker_pos": 1,
  "b1": [
      {
          "attack": 18,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 3,
          "name": "Glyph Guardian (Golden)"
      },
      {
          "attack": 7,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 3,
          "name": "Wildfire Elemental"
      },
      {
          "attack": 4,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Lil Rag"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 5,
          "name": "Bronze Warden"
      }
  ],
  "b1_turn": 1,
  "b2": [
      {
          "attack": 8,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 7,
          "name": "Rockpool Hunter (Golden)"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 5,
          "name": "Bronze Warden"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Bronze Warden"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 2,
          "name": "Lightfang Enforcer"
      }
  ],
  "defender_pos": 1
},
{
  "attacker_pos": 1,
  "b1": [
      {
          "attack": 18,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 3,
          "name": "Glyph Guardian (Golden)"
      },
      {
          "attack": 4,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Lil Rag"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 5,
          "name": "Bronze Warden"
      }
  ],
  "b1_turn": 0,
  "b2": [
      {
          "attack": 8,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 7,
          "name": "Rockpool Hunter (Golden)"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 5,
          "name": "Bronze Warden"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Bronze Warden"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 2,
          "name": "Lightfang Enforcer"
      }
  ],
  "defender_pos": 0
},
{
  "attacker_pos": 1,
  "b1": [
      {
          "attack": 4,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Glyph Guardian (Golden)"
      },
      {
          "attack": 4,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Lil Rag"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 5,
          "name": "Bronze Warden"
      }
  ],
  "b1_turn": 1,
  "b2": [
      {
          "attack": 8,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 7,
          "name": "Rockpool Hunter (Golden)"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Bronze Warden"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 4,
          "name": "Bronze Warden"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 2,
          "name": "Lightfang Enforcer"
      }
  ],
  "defender_pos": 2
},
{
  "attacker_pos": 3,
  "b1": [
      {
          "attack": 4,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Glyph Guardian (Golden)"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 5,
          "name": "Bronze Warden"
      }
  ],
  "b1_turn": 0,
  "b2": [
      {
          "attack": 8,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 7,
          "name": "Rockpool Hunter (Golden)"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Bronze Warden"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Bronze Warden"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 2,
          "name": "Lightfang Enforcer"
      }
  ],
  "defender_pos": 1
},
{
  "attacker_pos": 1,
  "b1": [
      {
          "attack": 4,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Glyph Guardian (Golden)"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 5,
          "name": "Bronze Warden"
      }
  ],
  "b1_turn": 1,
  "b2": [
      {
          "attack": 8,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 7,
          "name": "Rockpool Hunter (Golden)"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Bronze Warden"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Bronze Warden"
      }
  ],
  "defender_pos": 2
},
{
  "attacker_pos": 0,
  "b1": [
      {
          "attack": 4,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Glyph Guardian (Golden)"
      },
      {
          "attack": 6,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": true,
          "has_taunt": false,
          "has_windfury": false,
          "health": 3,
          "name": "Bronze Warden"
      }
  ],
  "b1_turn": 0,
  "b2": [
      {
          "attack": 8,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 7,
          "name": "Rockpool Hunter (Golden)"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Bronze Warden"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Bronze Warden"
      }
  ],
  "defender_pos": 1
},
{
  "attacker_pos": 1,
  "b1": [
      {
          "attack": 4,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Glyph Guardian (Golden)"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Bronze Warden"
      }
  ],
  "b1_turn": 1,
  "b2": [
      {
          "attack": 8,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Rockpool Hunter (Golden)"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Bronze Warden"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Bronze Warden"
      }
  ],
  "defender_pos": 2
},
{
  "attacker_pos": 1,
  "b1": [
      {
          "attack": 4,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Glyph Guardian (Golden)"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Bronze Warden"
      }
  ],
  "b1_turn": 0,
  "b2": [
      {
          "attack": 8,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Rockpool Hunter (Golden)"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": true,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Bronze Warden"
      }
  ],
  "defender_pos": 0
},
{
  "attacker_pos": 0,
  "b1": [
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Bronze Warden"
      }
  ],
  "b1_turn": 1,
  "b2": [
      {
          "attack": 8,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Rockpool Hunter (Golden)"
      },
      {
          "attack": 2,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Bronze Warden"
      }
  ],
  "defender_pos": 1
},
{
  "attacker_pos": 2,
  "b1_turn": 5132105,
  "b2": [
      {
          "attack": 8,
          "has_cleave": false,
          "has_divine_shield": false,
          "has_poison": false,
          "has_reborn": false,
          "has_taunt": false,
          "has_windfury": false,
          "health": 1,
          "name": "Rockpool Hunter (Golden)"
      }
  ],
  "defender_pos": 5132103
}
];
  return json;
}

function getCardText(card_json) {
    // "has_cleave": false,
    // "has_divine_shield": false,
    // "has_poison": false,
    // "has_reborn": false,
    // "has_taunt": false,
    // "has_windfury": false,
    var text = "";
    if (card_json['has_divine_shield']) {
        text += "DIVINE ";
    }
    if (card_json['has_reborn']) {
        text += "REBORN ";
    }
    if (card_json['has_taunt']) {
        text += "TAUNT ";
    }
    return text;
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
    var card_text = getCardText(card_json);
    var card_name = card_name_raw.replace(" (Golden)", "");

    var is_golden = card_name_raw !== card_name;

    sunwell.createCard({
      	//"id": "EX1_116",
      	//"dbfId": 559,
      	"name": card_name,
      	// "text": "<b>Charge</b>. <b>Battlecry:</b> Summon two 1/1 Whelps for your opponent.",
	"text": card_text,
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
      	// "mechanics": [
      	// 	"BATTLECRY",
      	// 	"CHARGE"
      	// ],
      	"rarity": "COMMON",
      	// "set": "EXPERT1",
      	"type": "MINION_ON_BOARD",
      	// "texture": "../assets/" + card_name + ".jpg"
        // "texture": process.env.PUBLIC_URL + "/assets/Mecharoo.png"
        "texture": asset_folder + card_name + ".jpg"
    }, 256, is_golden, img, function() {
      	console.log('done');
    });

    return img;
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

const BOARD_WIDTH = 600 * 2;
const BOARD_HEIGHT = 350 * 2;
// const BOARD_WIDTH = 800;
// const BOARD_HEIGHT = 500;
const BOARD_WIDTH_FUDGE = -BOARD_WIDTH / 20.0; // -30 at 600x350
// const TOP_BOARD_HEIGHT_FUDGE = -BOARD_HEIGHT / 14.0; // -25 at 600x350
const TOP_BOARD_HEIGHT_FUDGE = -BOARD_HEIGHT / 12.0; // -25 at 600x350
// const BOTTOM_BOARD_HEIGHT_FUDGE = -BOARD_HEIGHT / 5.0; // -70 at 600x350
const BOTTOM_BOARD_HEIGHT_FUDGE = -BOARD_HEIGHT / 4.5; // -70 at 600x350
const CENTERING_FUDGE_FACTOR = -BOARD_WIDTH / 40.0; // Board image is not quite symetric (e.g. hero portrait not quite centered)
const CARD_WIDTH_DELTA = BOARD_WIDTH / 10.0;
const MAX_NUM_CARDS = 7;

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
      <Stage width={BOARD_WIDTH} height={BOARD_HEIGHT} options={{ transparent: true }}>
        <Container x={0} y={0}>
            <Sprite
                image={this.state.backgroundImage}
                width={BOARD_WIDTH}
                height={BOARD_HEIGHT}
             />
            {card_arr}
        </Container>
      </Stage>
      </>
    )
  }
}

export default Rollout;
