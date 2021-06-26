"""
This script generates the JSON inlined in the Rollout.js page.
"""
import json
import os

import requests

THIS_DIR = os.path.dirname(__file__)

BUGGED_CARDS_TO_TIER = {
    "Alleycat": 1,
    "Amalgam": 1,
    "Fiendish Servant": 1,
    "Fish of N'Zoth": 1,
    "Micro Mummy": 1,
    "Murloc Tidehunter": 1,
    "Murloc Scout": 1,
    "Murloc Tidecaller": 1,
    "Rockpool Hunter": 1,
    "Scavenging Hyena": 1,
    "Vulgar Homunculus": 1,
    "Harvest Golem": 2,
    "Kaboom Bot": 2,
    "Kindly Grandmoth": 2,
    "Metaltooth Leaper": 2,
    "Murloc Warleader": 2,
    "Old Murk-Eye": 2,
    "Selfless Hero": 2,
    "Southsea Captain": 2,
    "Spawn of N'Zoth": 2,
    "Unstable Ghoul": 2,
    "Coldlight Seer": 3,
    "Crystalweaver": 3,
    "Felfin Navigator": 3,
    "Houndmaster": 3,
    "Imp Gang Boss": 3,
    "Infested Wolf": 3,
    "Iron Sensei": 3,
    "Khadgar": 3,
    "Rat Pack": 3,
    "Replicating Menace": 3,
    "Screwjank Clunker": 3,
    "Annoy-o-Module": 4,
    "Bolvar, Fireblood": 4,
    "Cave Hydra": 4,
    "Cobalt Scalebane": 4,
    "Defender of Argus": 4,
    "Junkbot": 4,
    "Mechano-Egg": 4,
    "Primalfin": 4,
    "Ring Matron": 4,
    "Savannah Highmane": 4,
    "Security Rover": 4,
    "Siegebreaker": 4,
    "Toxfin": 4,
    "Virmen Sensei": 4,
    "Baron Rivendare": 5,
    "Brann Bronzebeard": 5,
    "Ironhide Direhorn": 5,
    "Mal'Ganis": 5,
    "Strongshell Scavenger": 5,
    "Voidlord": 5,
    "Foe Reaper 4000": 6,
    "Maexxna": 6,
}

CARDS_TO_IGNORE = {
    'Piloted Shredder',
    'Piloted Sky Golem',
    'Mounted Raptor',
    'Pogo-Hopper',
    'Gentle Megasaur',
    'Sky Pirate',
    'Water Droplet',
    'Mecharoo',
    'Jo-E Bot',
    'Kindly Grandmother',
    'Big Bad Wolf',
    'Damaged Golem',
    'Mounted Raptor',
    'Spider',
    'Rat',
    'Imp',
    'Microbot',
    'Piloted Shredder',
    'Robosaur',
    'Safeguard',
    'Vault Safe',
    'Guard Bot',
    'The Beast',
    'Hyena',
    'Piloted Sky Golem',
    'Ironhide Runt',
    'Sated Threshadon',
    'Voidwalker',    
    'Zoobot',
    'Pogo-Hopper',
    'Gentle Megasaur',
    'Dire Wolf Alpha',
    'Tabbycat',
    'Floating Watcher',
    'Sky Pirate',
    'Whirlwind Tempest',
    'Fiery Imp',
    'Shifter Zerus',
    'Treasure Chest',
    'Arcane Cannon',
    'Elistra the Immortal',
    'Tortollan Shellraiser',
    'Festeroot Hulk',
    'Crowd Favorite',
    'Phalanx Commander',
    'The Boogeymonster',
    'Menagerie Magician',
    'Righteous Protector',
    'Argent Braggart',
    'Barrens Blacksmith',
}

def dump_bg_cards_for_rolloutjs():
    card_json = get_latest_card_json()
    dump_cards(card_json)

def get_latest_card_json():
    url = 'https://api.hearthstonejson.com/v1/latest/enUS/cards.json'
    res = requests.get(url)    
    return res.json()

def dump_cards(card_json):
    bgs_cards = [card for card in card_json if card['set'] == "BATTLEGROUNDS"]
    subbed_json = []
    names_done = set()
    for card in bgs_cards:
        if card['set'] != "BATTLEGROUNDS" or card['type'] != "MINION" or card['id'].endswith('_G'):
            continue
        if card['name'] in names_done or card['name'] in CARDS_TO_IGNORE:
            continue
        race = 'NEUTRAL' if 'race' not in card else card['race']
        if 'techLevel' not in card:
            # For some reason, the normal HS cards dont have a tier/techLevel associated with them...
            if card['name'] in BUGGED_CARDS_TO_TIER and card['name'] not in names_done:
                mechanics = [] if 'mechanics' not in card else card['mechanics']
                subbed_card_json = {'name': card['name'],
                                    'tier': BUGGED_CARDS_TO_TIER[card['name']],
                                    'race': race,
                                    'attack': card['attack'] // 2,  # Always golden
                                    'health': card['health'] // 2,  # Always golden
                                    'has_deathrattle': 'DEATHRATTLE' in mechanics,
                                    'has_divine_shield': 'DIVINESHIELD' in mechanics,
                                    'has_poison': 'POISONOUS' in mechanics,
                                    'has_reborn': 'REBORN' in mechanics,
                                    'has_taunt': 'TAUNT' in mechanics,
                                    'has_windfury': 'WINDFURY' in mechanics,
                }
            else:
                print("ERROR! Card " + card['name'] + " ignored, but not in the ignore list!")
                exit(1)
        else:
            if 'techLevel' not in card:
                continue
            mechanics = [] if 'mechanics' not in card else card['mechanics']
            subbed_card_json = {'name': card['name'],
                                'tier': card['techLevel'],
                                'race': race,
                                'attack': card['attack'],
                                'health': card['health'],
                                'has_deathrattle': 'DEATHRATTLE' in mechanics,
                                'has_divine_shield': 'DIVINESHIELD' in mechanics,
                                'has_poison': 'POISONOUS' in mechanics,
                                'has_reborn': 'REBORN' in mechanics,
                                'has_taunt': 'TAUNT' in mechanics,
                                'has_windfury': 'WINDFURY' in mechanics,
            }

                # "attack": 6,
                # "card_class": "MAGE",
                # "cost": 4,
                # "has_deathrattle": false,
                # "has_divine_shield": false,
                # "has_poison": false,
                # "has_reborn": true,
                # "has_taunt": false,
                # "has_triggered_effect": true,
                # "has_windfury": false,
                # "health": 10,
                # "mechanics": "",
                # "name": "Glyph Guardian (Golden)",
                # "race": "DRAGON",
                # "rarity": "",
                # "tavern_tier": 2,
                # "type": "MINION"
        names_done.add(card['name'])
        subbed_json.append(subbed_card_json)
    subbed_json_sorted = sorted(subbed_json, key=lambda card: (card['tier'], card['name']))
    print(json.dumps(subbed_json_sorted, indent=4))
    with open('card_default.json', 'w') as fp:
        fp.write(json.dumps(subbed_json_sorted, indent=4))

def main():
    dump_bg_cards_for_rolloutjs()

if __name__ == '__main__':
    main()
